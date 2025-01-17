#include "stdafx.h"
#include "Player.h"
#include "MusicPlayer2.h"

CBASSMidiLibrary CPlayer::m_bass_midi_lib;
CPlayer CPlayer::m_instance;

CPlayer::CPlayer()
{
}

CPlayer & CPlayer::GetInstance()
{
	return m_instance;
}

CPlayer::~CPlayer()
{
	UnInitBASS();
}

void CPlayer::IniBASS()
{
	//获取当前的音频输出设备
	BASS_DEVICEINFO device_info;
	int rtn;
	int device_index{1};
	theApp.m_output_devices.clear();
	DeviceInfo device{};
	device.index = -1;
	device.name = CCommon::LoadText(IDS_DEFAULT_OUTPUT_DEVICE);
	theApp.m_output_devices.push_back(device);
	while (true)
	{
		device = DeviceInfo{};
		rtn = BASS_GetDeviceInfo(device_index, &device_info);
		if (rtn == 0)
			break;
		device.index = device_index;
		if(device_info.name!=nullptr)
			device.name = CCommon::StrToUnicode(string(device_info.name));
		if (device_info.driver != nullptr)
			device.driver = CCommon::StrToUnicode(string(device_info.driver));
		device.flags = device_info.flags;
		theApp.m_output_devices.push_back(device);
		device_index++;
	}

	for (size_t i{}; i < theApp.m_output_devices.size(); i++)
	{
		if (theApp.m_output_devices[i].name == theApp.m_play_setting_data.output_device)
		{
			theApp.m_play_setting_data.device_selected = i;
			break;
		}
	}

	//初始化BASE音频库
	BASS_Init(
		theApp.m_output_devices[theApp.m_play_setting_data.device_selected].index,		//播放设备
		44100,//输出采样率44100（常用值）
		BASS_DEVICE_CPSPEAKERS,//信号，BASS_DEVICE_CPSPEAKERS 注释原文如下：
							   /* Use the Windows control panel setting to detect the number of speakers.*/
							   /* Soundcards generally have their own control panel to set the speaker config,*/
							   /* so the Windows control panel setting may not be accurate unless it matches that.*/
							   /* This flag has no effect on Vista, as the speakers are already accurately detected.*/
		theApp.m_pMainWnd->m_hWnd,//程序窗口,0用于控制台程序
		NULL//类标识符,0使用默认值
	);

	//向支持的文件列表插入原生支持的文件格式
	CAudioCommon::m_surpported_format.clear();
	SupportedFormat format;
	format.description = CCommon::LoadText(IDS_BASIC_AUDIO_FORMAT);
	format.extensions.push_back(L"mp3");
	format.extensions.push_back(L"wma");
	format.extensions.push_back(L"wav");
	format.extensions.push_back(L"flac");
	format.extensions.push_back(L"ogg");
	format.extensions.push_back(L"oga");
	format.extensions.push_back(L"m4a");
	format.extensions.push_back(L"mp4");
	format.extensions.push_back(L"cue");
	format.extensions.push_back(L"mp2");
	format.extensions.push_back(L"mp1");
	format.extensions.push_back(L"aif");
	format.extensions_list = L"*.mp3;*.wma;*.wav;*.flac;*.ogg;*.oga;*.m4a;*.mp4;*.cue;*.mp2;*.mp1;*.aif";
	CAudioCommon::m_surpported_format.push_back(format);
	CAudioCommon::m_all_surpported_extensions = format.extensions;
	//载入BASS插件
	wstring plugin_dir;
	plugin_dir = theApp.m_local_dir + L"Plugins\\";
	vector<wstring> plugin_files;
	CCommon::GetFiles(plugin_dir + L"*.dll", plugin_files);		//获取Plugins目录下所有的dll文件的文件名
	m_plugin_handles.clear();
	for (const auto& plugin_file : plugin_files)
	{
		//加载插件
		HPLUGIN handle = BASS_PluginLoad((plugin_dir + plugin_file).c_str(), 0);
		m_plugin_handles.push_back(handle);
		//获取插件支持的音频文件类型
		const BASS_PLUGININFO* plugin_info = BASS_PluginGetInfo(handle);
		if (plugin_info == nullptr)
			continue;
		format.file_name = plugin_file;
		format.description = CCommon::ASCIIToUnicode(plugin_info->formats->name);	//插件支持文件类型的描述
		format.extensions_list = CCommon::ASCIIToUnicode(plugin_info->formats->exts);	//插件支持文件扩展名列表
		//解析扩展名列表到vector
		format.extensions.clear();
		size_t index = 0, last_index = 0;
		while (true)
		{
			index = format.extensions_list.find(L"*.", index + 1);
			wstring ext{ format.extensions_list.substr(last_index + 2, index - last_index - 2) };
			if (!ext.empty() && ext.back() == L';')
				ext.pop_back();
			format.extensions.push_back(ext);
			if(!CCommon::IsItemInVector(CAudioCommon::m_all_surpported_extensions, ext))
				CAudioCommon::m_all_surpported_extensions.push_back(ext);
			if (index == wstring::npos)
				break;
			last_index = index;
		}
		CAudioCommon::m_surpported_format.push_back(format);

		//载入MIDI音色库，用于播放MIDI
		if (format.description == L"MIDI")
		{
			m_bass_midi_lib.Init(plugin_dir + plugin_file);
			m_sfont_name = CCommon::LoadText(_T("<"), IDS_NONE, _T(">"));
			m_sfont.font = 0;
			if (m_bass_midi_lib.IsSuccessed())
			{
				wstring sf2_path = theApp.m_general_setting_data.sf2_path;
				if (!CCommon::FileExist(sf2_path))		//如果设置的音色库路径不存在，则从.\Plugins\soundfont\目录下查找音色库文件
				{
					vector<wstring> sf2s;
					CCommon::GetFiles(plugin_dir + L"soundfont\\*.sf2", sf2s);
					if (!sf2s.empty())
						sf2_path = plugin_dir + L"soundfont\\" + sf2s[0];
				}
				if (CCommon::FileExist(sf2_path))
				{
					m_sfont.font = m_bass_midi_lib.BASS_MIDI_FontInit(sf2_path.c_str(), BASS_UNICODE);
					if (m_sfont.font == 0)
					{
						CString info;
						info = CCommon::LoadTextFormat(IDS_SOUND_FONT_LOAD_FAILED, { sf2_path });
						theApp.WriteErrorLog(info.GetString());
						m_sfont_name = CCommon::LoadText(_T("<"), IDS_LOAD_FAILED, _T(">"));
					}
					else
					{
						//获取音色库信息
						BASS_MIDI_FONTINFO sfount_info;
						m_bass_midi_lib.BASS_MIDI_FontGetInfo(m_sfont.font, &sfount_info);
						m_sfont_name = CCommon::StrToUnicode(sfount_info.name);
					}
					m_sfont.preset = -1;
					m_sfont.bank = 0;
				}
			}
		}
	}

}

void CPlayer::UnInitBASS()
{
	BASS_Stop();	//停止输出
	BASS_Free();	//释放Bass资源
	if (m_bass_midi_lib.IsSuccessed() && m_sfont.font != 0)
		m_bass_midi_lib.BASS_MIDI_FontFree(m_sfont.font);
	m_bass_midi_lib.UnInit();
	for (const auto& handle : m_plugin_handles)		//释放插件句柄
	{
		BASS_PluginFree(handle);
	}
}

void CPlayer::Create()
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	IniPlayList();	//初始化播放列表
	//EmplaceCurrentPathToRecent();
	SetTitle();		//用当前正在播放的歌曲名作为窗口标题
}

void CPlayer::Create(const vector<wstring>& files)
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	size_t index;
	index = files[0].find_last_of(L'\\');
	m_path = files[0].substr(0, index + 1);
	SongInfo song_info;
	for (const auto& file : files)
	{
		index = file.find_last_of(L'\\');
		song_info.file_name = file.substr(index + 1);
		m_playlist.push_back(song_info);
	}
	IniPlayList(true);
	//EmplaceCurrentPathToRecent();
	m_current_position_int = 0;
	m_current_position = { 0,0,0 };
	m_index = 0;
	SetTitle();		//用当前正在播放的歌曲名作为窗口标题
}

void CPlayer::Create(const wstring& path)
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	//IniPlayList();	//初始化播放列表
	//EmplaceCurrentPathToRecent();
	OpenFolder(path);
	SetTitle();		//用当前正在播放的歌曲名作为窗口标题
}

void CPlayer::IniPlayList(bool cmd_para, bool refresh_info)
{
	if (!m_loading)
	{
		if (!cmd_para)
		{
			CAudioCommon::GetAudioFiles(m_path, m_playlist, MAX_SONG_NUM);
		}
		//m_index = 0;
		//m_song_num = m_playlist.size();
		m_index_tmp = m_index;		//保存歌曲序号
		if (m_index < 0 || m_index >= GetSongNum()) m_index = 0;		//确保当前歌曲序号不会超过歌曲总数

		m_loading = true;
		//m_thread_info.playlist = &m_playlist;
		m_thread_info.refresh_info = refresh_info;
		m_thread_info.sort = !cmd_para;
		//m_thread_info.path = m_path;
		m_thread_info.player = this;
		//创建初始化播放列表的工作线程
		m_pThread = AfxBeginThread(IniPlaylistThreadFunc, &m_thread_info);

		m_song_length = { 0,0,0 };
		//m_current_position = {0,0,0};
		if (GetSongNum() == 0)
		{
			m_playlist.push_back(SongInfo{});		//没有歌曲时向播放列表插入一个空的SongInfo对象
		}
		//m_current_file_name = m_playlist[m_index].file_name;
	}
}

UINT CPlayer::IniPlaylistThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	SendMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_START, 0, 0);
	ThreadInfo* pInfo = (ThreadInfo*)lpParam;
	//获取播放列表中每一首歌曲的信息
	//最多只获取MAX_NUM_LENGTH首歌的长度，超过MAX_NUM_LENGTH数量的歌曲的长度在打开时获得。防止文件夹中音频文件过多导致等待时间过长
	int song_num = pInfo->player->m_playlist.size();
	int song_count = min(song_num, MAX_NUM_LENGTH);
	for (int i{}, count{}; count < song_count && i < song_num; i++)
	{
		pInfo->process_percent = i * 100 / song_count + 1;

		if (!pInfo->refresh_info)
		{
			wstring file_name{ pInfo->player->m_playlist[i].file_name };
			auto iter = theApp.m_song_data.find(pInfo->player->m_path + pInfo->player->m_playlist[i].file_name);
			if (iter != theApp.m_song_data.end())		//如果歌曲信息容器中已经包含该歌曲，则不需要再获取歌曲信息
			{
				pInfo->player->m_playlist[i] = iter->second;
				pInfo->player->m_playlist[i].file_name = file_name;
				continue;
			}
		}
		wstring file_path{ pInfo->player->m_path + pInfo->player->m_playlist[i].file_name };
		HSTREAM hStream;
		hStream = BASS_StreamCreateFile(FALSE, file_path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
		pInfo->player->AcquireSongInfo(hStream, file_path, pInfo->player->m_playlist[i]);
		BASS_StreamFree(hStream);
		count++;
	}
	pInfo->player->m_loading = false;
	pInfo->player->IniPlaylistComplate(pInfo->sort);
	//pInfo->player->IniLyrics();
	PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_COMPLATE, 0, 0);
	return 0;
}

void CPlayer::IniPlaylistComplate(bool sort)
{
	CAudioCommon::CheckCueFiles(m_playlist, m_path);
	CAudioCommon::GetCueTracks(m_playlist, m_path);
	//m_song_num = m_playlist.size();
	m_index = m_index_tmp;
	if (m_index < 0 || m_index >= GetSongNum()) m_index = 0;		//确保当前歌曲序号不会超过歌曲总数
	//统计列表总时长
	m_total_time = 0;
	for (const auto& somg : m_playlist)
	{
		m_total_time += somg.lengh.time2int();
	}

	//清除歌词和专辑封面
	m_album_cover.Destroy();
	m_album_cover_blur.Destroy();
	m_Lyrics = CLyrics();

	//对播放列表排序
	wstring current_file_name = GetCurrentFileName();		//排序前保存当前歌曲文件名
	if (sort && m_playlist.size() > 1)
		SortPlaylist(false);

	SearchLyrics();

	if (GetSongNum() > 0)
	{
		if (m_playing == 0)		//播放列表初始化完成，并排序完成后，如果此时没有在播放，就重新设置播放的文件
		{
			if (!m_current_file_name_tmp.empty())		//如果执行了ReloadPlaylist，m_current_file_name_tmp不为空，则查找m_current_file_name_tmp保存的曲目并播放
			{
				//重新载入播放列表后，查找正在播放项目的序号
				MusicControl(Command::CLOSE);
				for (int i{}; i < GetSongNum(); i++)
				{
					if (m_current_file_name_tmp == m_playlist[i].file_name)
					{
						m_index = i;
						//m_current_file_name = m_current_file_name_tmp;
						break;
					}
				}
				m_current_file_name_tmp.clear();
				MusicControl(Command::OPEN);
				MusicControl(Command::SEEK);
				//MusicControl(Command::PLAY);
			}
			else		//否则，直接打开播放第index首曲目
			{
				MusicControl(Command::CLOSE);
				//m_current_file_name = m_playlist[m_index].file_name;
				MusicControl(Command::OPEN);
				MusicControl(Command::SEEK);
				if (theApp.m_play_setting_data.auto_play_when_start)
					MusicControl(Command::PLAY);
			}
		}
		else		//如果用户在播放初始化的过程中进行了播放，则根据正在播放的文件名重新查找正在播放的序号
		{
			for (int i{}; i < GetSongNum(); i++)
			{
				if (current_file_name == m_playlist[i].file_name)
				{
					m_index = i;
					break;
				}
			}
		}
	}
	if(!sort)		//如果文件是通过命令行参数打开的，则sort会为false，此时打开后直接播放
		MusicControl(Command::PLAY);

	EmplaceCurrentPathToRecent();
	SetTitle();
	m_shuffle_list.clear();
	if (m_repeat_mode == RM_PLAY_SHUFFLE)
		m_shuffle_list.push_back(m_index);
}

void CPlayer::SearchLyrics(/*bool refresh*/)
{
	//检索歌词文件
	//如果允许歌词模糊匹配，先将所有的歌词文件的文件名保存到容器中以供模糊匹配时检索
	if (theApp.m_lyric_setting_data.lyric_fuzzy_match)
	{
		m_current_path_lyrics.clear();
		m_lyric_path_lyrics.clear();
		CAudioCommon::GetLyricFiles(m_path, m_current_path_lyrics);
		CAudioCommon::GetLyricFiles(theApp.m_lyric_setting_data.lyric_path, m_lyric_path_lyrics);
	}

	//检索播放列表中每一首歌曲的歌词文件，并将歌词文件路径保存到列表中
	for (auto& song : m_playlist)
	{
		if (song.file_name.size() < 3) continue;
		song.lyric_file.clear();		//检索歌词前先清除之前已经关联过的歌词
		//if (!song.lyric_file.empty() && CCommon::FileExist(song.lyric_file))		//如果歌曲信息中有歌词文件，且歌词文件存在，则不需要再获取歌词
		//	continue;
		CFilePathHelper lyric_path{ m_path + song.file_name };		//得到路径+文件名的字符串
		lyric_path.ReplaceFileExtension(L"lrc");		//将文件扩展替换成lrc
		CFilePathHelper lyric_path2{ theApp.m_lyric_setting_data.lyric_path + song.file_name };
		lyric_path2.ReplaceFileExtension(L"lrc");
		//查找歌词文件名和歌曲文件名完全匹配的歌词
		if (CCommon::FileExist(lyric_path.GetFilePath()))
		{
			song.lyric_file = lyric_path.GetFilePath();
		}
		else if (CCommon::FileExist(lyric_path2.GetFilePath()))		//当前目录下没有对应的歌词文件时，就在theApp.m_lyric_setting_data.m_lyric_path目录下寻找歌词文件
		{
			song.lyric_file = lyric_path2.GetFilePath();
		}
		else if (theApp.m_lyric_setting_data.lyric_fuzzy_match)
		{
			wstring matched_lyric;		//匹配的歌词的路径
			//先寻找歌词文件中同时包含歌曲标题和艺术家的歌词文件
			for (const auto& str : m_current_path_lyrics)	//在当前目录下寻找
			{
				//if (str.find(song.artist) != string::npos && str.find(song.title) != string::npos)
				if (CCommon::StringNatchWholeWord(str, song.artist) != -1 && CCommon::StringNatchWholeWord(str, song.title) != -1)
				{
					matched_lyric = m_path + str;
					break;
				}
			}

			if (matched_lyric.empty())		//如果当前目录下没找到
			{
				for (const auto& str : m_lyric_path_lyrics)	//在歌词目录下寻找
				{
					//if (str.find(song.artist) != string::npos && str.find(song.title) != string::npos)
					if (CCommon::StringNatchWholeWord(str, song.artist) != -1 && CCommon::StringNatchWholeWord(str, song.title) != -1)
					{
						matched_lyric = theApp.m_lyric_setting_data.lyric_path + str;
						break;
					}
				}
			}

			//没有找到的话就寻找歌词文件中只包含歌曲标题的歌词文件
			if (matched_lyric.empty())
			{
				for (const auto& str : m_current_path_lyrics)	//在当前目录下寻找
				{
					//if (str.find(song.title) != string::npos)
					if (CCommon::StringNatchWholeWord(str, song.title) != -1)
					{
						matched_lyric = m_path + str;
						break;
					}
				}
			}

			if (matched_lyric.empty())
			{
				for (const auto& str : m_lyric_path_lyrics)	//在歌词目录下寻找
				{
					//if (str.find(song.title) != string::npos)
					if (CCommon::StringNatchWholeWord(str, song.title) != -1)
					{
						matched_lyric = theApp.m_lyric_setting_data.lyric_path + str;
						break;
					}
				}
			}

			if (!matched_lyric.empty())
				song.lyric_file = matched_lyric;
		}
		////如果已经获取到了歌词，则将歌词路径保存到所有歌曲信息容器中
		//auto iter = theApp.m_song_data.find(m_path + song.file_name);
		//if (iter != theApp.m_song_data.end())
		//	iter->second.lyric_file = song.lyric_file;
	}
}

void CPlayer::IniLyrics()
{
	if (!m_playlist.empty() && !m_playlist[m_index].lyric_file.empty())
		m_Lyrics = CLyrics{ m_playlist[m_index].lyric_file };
	else
		m_Lyrics = CLyrics{};
}

void CPlayer::IniLyrics(const wstring& lyric_path)
{
	m_Lyrics = CLyrics{ lyric_path };
	m_playlist[m_index].lyric_file = lyric_path;
}

void CPlayer::MidiLyricSync(HSYNC handle, DWORD channel, DWORD data, void * user)
{
	if (!CPlayer::m_bass_midi_lib.IsSuccessed())
		return;
	CPlayer::GetInstance().m_midi_no_lyric = false;
	BASS_MIDI_MARK mark;
	CPlayer::m_bass_midi_lib.BASS_MIDI_StreamGetMark(channel, (DWORD)user, data, &mark); // get the lyric/text
	if (mark.text[0] == '@') return; // skip info
	if (mark.text[0] == '\\')
	{ // clear display
		CPlayer::GetInstance().m_midi_lyric.clear();
	}
	else if (mark.text[0] == '/')
	{
		CPlayer::GetInstance().m_midi_lyric += L"\r\n";
		const char* text = mark.text + 1;
		CPlayer::GetInstance().m_midi_lyric += CCommon::StrToUnicode(text, CodeType::ANSI);
	}
	else
	{
		CPlayer::GetInstance().m_midi_lyric += CCommon::StrToUnicode(mark.text, CodeType::ANSI);
	}
}

void CPlayer::MidiEndSync(HSYNC handle, DWORD channel, DWORD data, void * user)
{
	CPlayer::GetInstance().m_midi_lyric.clear();
}

void CPlayer::MusicControl(Command command, int volume_step)
{
// 	if (!CCommon::FileExist(m_path + GetCurrentFileName()))
// 		return;

	switch (command)
	{
	case Command::OPEN:
		m_error_code = 0;
		if (!GetCurrentFileName().empty()) {
			std::wstring filename = GetCurrentFileName();
			if (PathIsRelative(filename.c_str()))
				filename = m_path + filename;
			m_musicStream = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
		} else if (!GetCurrentPlayURL().empty()){
			m_musicStream = BASS_StreamCreateURL(GetCurrentPlayURL().c_str(), 0, 0, NULL, NULL);
		}
		
		BASS_ChannelGetInfo(m_musicStream, &m_channel_info);
		m_is_midi = (CAudioCommon::GetAudioTypeByBassChannel(m_channel_info.ctype) == AudioType::AU_MIDI);
		if (m_bass_midi_lib.IsSuccessed() && m_is_midi && m_sfont.font != 0)
			m_bass_midi_lib.BASS_MIDI_StreamSetFonts(m_musicStream, &m_sfont, 1);
		//获取音频类型
		m_current_file_type = CAudioCommon::GetBASSChannelDescription(m_channel_info.ctype);		//根据通道信息获取当前音频文件的类型
		if (m_current_file_type.empty())		//如果获取不到音频文件的类型，则将其文件扩展名作为文件类型
		{
			CFilePathHelper file_path{ GetCurrentFileName() };
			m_current_file_type = file_path.GetFileExtension(true);
		}
		if (GetSongNum() > 0)
		{
			if (!m_playlist[m_index].info_acquired)	//如果当前打开的文件没有在初始化播放列表时获得信息，则打开时重新获取
				AcquireSongInfo(m_musicStream, m_path + GetCurrentFileName(), m_playlist[m_index]);
			m_song_length = m_playlist[m_index].lengh;
			m_song_length_int = m_song_length.time2int();
			//如果文件是MIDI音乐，则打开时获取MIDI音乐的信息
			if (m_is_midi && m_bass_midi_lib.IsSuccessed())
			{
				//获取MIDI音乐信息
				BASS_ChannelGetAttribute(m_musicStream, BASS_ATTRIB_MIDI_PPQN, &m_midi_info.ppqn); // get PPQN value
				m_midi_info.midi_length = static_cast<int>(BASS_ChannelGetLength(m_musicStream, BASS_POS_MIDI_TICK) / m_midi_info.ppqn);
				m_midi_info.tempo = m_bass_midi_lib.BASS_MIDI_StreamGetEvent(m_musicStream, 0, MIDI_EVENT_TEMPO);
				m_midi_info.speed = 60000000 / m_midi_info.tempo;
				//获取MIDI音乐内嵌歌词
				BASS_MIDI_MARK mark;
				m_midi_lyric.clear();
				if (m_bass_midi_lib.BASS_MIDI_StreamGetMark(m_musicStream, BASS_MIDI_MARK_LYRIC, 0, &mark)) // got lyrics
					BASS_ChannelSetSync(m_musicStream, BASS_SYNC_MIDI_MARK, BASS_MIDI_MARK_LYRIC, MidiLyricSync, (void*)BASS_MIDI_MARK_LYRIC);
				else if (m_bass_midi_lib.BASS_MIDI_StreamGetMark(m_musicStream, BASS_MIDI_MARK_TEXT, 20, &mark)) // got text instead (over 20 of them)
					BASS_ChannelSetSync(m_musicStream, BASS_SYNC_MIDI_MARK, BASS_MIDI_MARK_TEXT, MidiLyricSync, (void*)BASS_MIDI_MARK_TEXT);
				BASS_ChannelSetSync(m_musicStream, BASS_SYNC_END, 0, MidiEndSync, 0);
				m_midi_no_lyric = true;
			}
			//打开时获取专辑封面
			SearchAlbumCover();
			//初始化歌词
			IniLyrics();
		}
		if (m_playlist[m_index].is_cue)
		{
			//SeekTo(0);
			m_song_length = GetCurrentSongInfo().lengh;
			m_song_length_int = m_song_length.time2int();
		}
		SetVolume();
		memset(m_spectral_data, 0, sizeof(m_spectral_data));		//打开文件时清除频谱分析的数据
		SetFXHandle();
		if (m_equ_enable)
			SetAllEqualizer();
		if (m_reverb_enable)
			SetReverb(m_reverb_mix, m_reverb_time);
		else
			ClearReverb();
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_MUSIC_STREAM_OPENED, 0, 0);
		break;
	case Command::PLAY:
		ConnotPlayWarning();
		BASS_ChannelPlay(m_musicStream, FALSE); m_playing = 2;
		break;
	case Command::CLOSE:
		RemoveFXHandle();
		BASS_StreamFree(m_musicStream);
		m_playing = 0;
		break;
	case Command::PAUSE: BASS_ChannelPause(m_musicStream); m_playing = 1; break;
	case Command::STOP:
		BASS_ChannelStop(m_musicStream);
		m_playing = 0;
		SeekTo(0);
		memset(m_spectral_data, 0, sizeof(m_spectral_data));		//停止时清除频谱分析的数据
		//GetBASSCurrentPosition();
		break;
	case Command::FF:		//快进
		GetBASSCurrentPosition();		//获取当前位置（毫秒）
		m_current_position_int += 5000;		//每次快进5000毫秒
		if (m_current_position_int > m_song_length_int) m_current_position_int -= 5000;
		SeekTo(m_current_position_int);
		break;
	case Command::REW:		//快退
		GetBASSCurrentPosition();		//获取当前位置（毫秒）
		m_current_position_int -= 5000;		//每次快退5000毫秒
		if (m_current_position_int < 0) m_current_position_int = 0;		//防止快退到负的位置
		SeekTo(m_current_position_int);
		break;
	case Command::PLAY_PAUSE:
		if (m_playing == 2)
		{
			BASS_ChannelPause(m_musicStream);
			m_playing = 1;
		}
		else
		{
			ConnotPlayWarning();
			BASS_ChannelPlay(m_musicStream, FALSE);
			m_playing = 2;
		}
		break;
	case Command::VOLUME_UP:
		if (m_volume < 100)
		{
			m_volume += volume_step;
			if (m_volume > 100) m_volume = 100;
			SetVolume();
			SaveConfig();
		}
		break;
	case Command::VOLUME_DOWN:
		if (m_volume > 0)
		{
			m_volume -= volume_step;
			if (m_volume < 0) m_volume = 0;
			SetVolume();
			SaveConfig();
		}
		break;
	case Command::SEEK:		//定位到m_current_position的位置
		if (m_current_position_int > m_song_length_int)
		{
			m_current_position_int = 0;
			m_current_position = Time{ 0, 0, 0 };
		}
		SeekTo(m_current_position_int);
		break;
	default: break;
	}
}

bool CPlayer::SongIsOver() const
{
	if (GetCurrentSongInfo().is_cue)
	{
		return (m_playing == 2 && m_current_position_int >= m_song_length_int);
	}
	else
	{
		bool song_is_over;
		static int last_pos;
		if ((m_playing == 2 && m_current_position_int == last_pos && m_current_position_int != 0	//如果正在播放且当前播放的位置没有发生变化且当前播放位置不为0，
			&& m_current_position_int > m_song_length_int-2000)		//且播放进度到了最后2秒
			|| m_error_code == BASS_ERROR_ENDED)	//或者出现BASS_ERROR_ENDED错误，则判断当前歌曲播放完了
			//有时候会出现识别的歌曲长度超过实际歌曲长度的问题，这样会导致歌曲播放进度超过实际歌曲结尾时会出现BASS_ERROR_ENDED错误，
			//检测到这个错误时直接判断歌曲已经播放完了。
			song_is_over = true;
		else
			song_is_over = false;
		last_pos = m_current_position_int;
		return song_is_over;
		//这里本来直接使用return m_current_position_int>=m_song_length_int来判断歌曲播放完了，
		//但是BASS音频库在播放时可能会出现当前播放位置一直无法到达歌曲长度位置的问题，
		//这样函数就会一直返回false。
	}
}

void CPlayer::GetBASSSongLength()
{
	QWORD lenght_bytes;
	lenght_bytes = BASS_ChannelGetLength(m_musicStream, BASS_POS_BYTE);
	double length_sec;
	length_sec = BASS_ChannelBytes2Seconds(m_musicStream, lenght_bytes);
	m_song_length_int = static_cast<int>(length_sec * 1000);
	if (m_song_length_int == -1000) m_song_length_int = 0;
	m_song_length.int2time(m_song_length_int);		//将长度转换成Time结构
}

Time CPlayer::GetBASSSongLength(HSTREAM hStream)
{
	QWORD lenght_bytes;
	lenght_bytes = BASS_ChannelGetLength(hStream, BASS_POS_BYTE);
	double length_sec;
	length_sec = BASS_ChannelBytes2Seconds(hStream, lenght_bytes);
	int song_length_int = static_cast<int>(length_sec * 1000);
	if (song_length_int == -1000) song_length_int = 0;
	return Time(song_length_int);		//将长度转换成Time结构
}

void CPlayer::GetBASSCurrentPosition()
{
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelGetPosition(m_musicStream, BASS_POS_BYTE);
	double pos_sec;
	pos_sec = BASS_ChannelBytes2Seconds(m_musicStream, pos_bytes);
	m_current_position_int = static_cast<int>(pos_sec * 1000);
	if (m_current_position_int == -1000) m_current_position_int = 0;
	if (m_playlist[m_index].is_cue)
	{
		m_current_position_int -= m_playlist[m_index].start_pos.time2int();
	}
	m_current_position.int2time(m_current_position_int);
	GetMidiPosition();
}

int CPlayer::GetBASSCurrentPosition(HSTREAM hStream)
{
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelGetPosition(hStream, BASS_POS_BYTE);
	double pos_sec;
	pos_sec = BASS_ChannelBytes2Seconds(hStream, pos_bytes);
	return static_cast<int>(pos_sec * 1000);
}


void CPlayer::SetVolume()
{
	float volume = static_cast<float>(m_volume) / 100.0f;
	volume = volume * theApp.m_nc_setting_data.volume_map / 100;
	BASS_ChannelSetAttribute(m_musicStream, BASS_ATTRIB_VOL, volume);
}


void CPlayer::GetBASSSpectral()
{
	if (m_musicStream && m_playing != 0 && m_current_position_int < m_song_length_int - 500)	//确保音频句柄不为空，并且歌曲最后500毫秒不显示频谱，以防止歌曲到达末尾无法获取频谱的错误
	{
		BASS_ChannelGetData(m_musicStream, m_fft, BASS_DATA_FFT256);
		memset(m_spectral_data, 0, sizeof(m_spectral_data));
		for (int i{}; i < FFT_SAMPLE; i++)
		{
			m_spectral_data[i / (FFT_SAMPLE / SPECTRUM_ROW)] += m_fft[i];
		}

		for (int i{}; i < SPECTRUM_ROW; i++)
		{
			m_spectral_data[i] /= (FFT_SAMPLE / SPECTRUM_ROW);
			m_spectral_data[i] = std::sqrtf(m_spectral_data[i]);		//对每个频谱柱形的值取平方根，以减少不同频率频谱值的差异
			m_spectral_data[i] *= 60;			//调整这里的乘数可以调整频谱分析柱形图整体的高度
		}
	}
	else
	{
		memset(m_spectral_data, 0, sizeof(m_spectral_data));
	}
	//计算频谱顶端的高度
	if (m_playing != 1)
	{
		static int fall_count;
		for (int i{}; i < SPECTRUM_ROW; i++)
		{
			if (m_spectral_data[i] > m_last_spectral_data[i])
			{
				m_spectral_peak[i] = m_spectral_data[i];		//如果当前的频谱比上一次的频谱高，则频谱顶端高度则为当前频谱的高度
				fall_count = 0;
			}
			else
			{
				fall_count++;
				m_spectral_peak[i] -= (fall_count*0.2f);		//如果当前频谱比上一次的频谱主低，则频谱顶端的高度逐渐下降
			}
		}
	}

	memcpy_s(m_last_spectral_data, sizeof(m_last_spectral_data), m_spectral_data, sizeof(m_spectral_data));
}


int CPlayer::GetCurrentSecond()
{
	return m_current_position.sec;
}

bool CPlayer::IsPlaying() const
{
	return m_playing == 2;
}

bool CPlayer::PlayTrack(int song_track)
{
	switch (m_repeat_mode)
	{
	case RM_PLAY_ORDER:		//顺序播放
		if (song_track == NEXT)		//播放下一曲
			song_track = m_index + 1;
		if (song_track == PREVIOUS)		//播放上一曲
			song_track = m_index - 1;
		break;
	case RM_PLAY_SHUFFLE:		//随机播放
		if (song_track == NEXT)
		{
			SYSTEMTIME current_time;
			GetLocalTime(&current_time);			//获取当前时间
			srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
			song_track = rand() % GetSongNum();
			m_shuffle_list.push_back(song_track);	//保存随机播放过的曲目
		}
		else if (song_track == PREVIOUS)		//回溯上一个随机播放曲目
		{
			if (m_shuffle_list.size() >= 2)
			{
				if (m_index == m_shuffle_list.back())
					m_shuffle_list.pop_back();
				song_track = m_shuffle_list.back();
			}
			else
			{
				MusicControl(Command::STOP);	//无法回溯时停止播放
				return true;
			}
		}
		//else if (song_track >= 0 && song_track < m_song_num)
		//{
		//	m_shuffle_list.push_back(song_track);	//保存随机播放过的曲目
		//}
		break;
	case RM_LOOP_PLAYLIST:		//列表循环
		if (song_track == NEXT)		//播放下一曲
		{
			song_track = m_index + 1;
			if (song_track >= GetSongNum()) song_track = 0;
			if (song_track < 0) song_track = GetSongNum() - 1;
		}
		if (song_track == PREVIOUS)		//播放上一曲
		{
			song_track = m_index - 1;
			if (song_track >= GetSongNum()) song_track = 0;
			if (song_track < 0) song_track = GetSongNum() - 1;
		}
		break;
	case RM_LOOP_TRACK:		//单曲循环
		if (song_track == NEXT || song_track == PREVIOUS)
			song_track = m_index;
	}

	if (song_track >= 0 && song_track < GetSongNum())
	{
		MusicControl(Command::CLOSE);
		m_index = song_track;
		//m_current_file_name = m_playlist[m_index].file_name;
		MusicControl(Command::OPEN);
		//IniLyrics();
		if (m_playlist[m_index].is_cue)
			SeekTo(0);
		MusicControl(Command::PLAY);
		GetBASSCurrentPosition();
		SetTitle();
		SaveConfig();
		EmplaceCurrentPathToRecent();
		SaveRecentPath();
		return true;
	}
	else
	{
		MusicControl(Command::CLOSE);
		m_index = 0;
		//m_current_file_name = m_playlist[m_index].file_name;
		MusicControl(Command::OPEN);
		//IniLyrics();
		GetBASSCurrentPosition();
		SetTitle();
		SaveConfig();
		EmplaceCurrentPathToRecent();
		SaveRecentPath();
	}
	return false;
}

void CPlayer::ChangePath(const wstring& path, int track)
{
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	m_path = path;
	if (m_path.empty() || (m_path.back() != L'/' && m_path.back() != L'\\'))		//如果输入的新路径为空或末尾没有斜杠，则在末尾加上一个
		m_path.append(1, L'\\');
	m_playlist.clear();		//清空播放列表
	m_index = track;
	//初始化播放列表
	IniPlayList();		//根据新路径重新初始化播放列表
	m_current_position_int = 0;
	m_current_position = { 0, 0, 0 };
	SaveConfig();
	SetTitle();
	//MusicControl(Command::OPEN);
	//IniLyrics();
}

void CPlayer::SetPath(const wstring& path, int track, int position, SortMode sort_mode)
{
	//if (m_song_num>0 && !m_playlist[0].file_name.empty())		//如果当前路径有歌曲，就保存当前路径到最近路径
	EmplaceCurrentPathToRecent();
	m_sort_mode = sort_mode;
	ChangePath(path, track);
	m_current_position_int = position;
	m_current_position.int2time(m_current_position_int);
	//MusicControl(Command::SEEK);
	EmplaceCurrentPathToRecent();		//保存新的路径到最近路径
	
}

void CPlayer::OpenFolder(wstring path)
{
	if (m_loading) return;
	if (path.empty() || (path.back() != L'/' && path.back() != L'\\'))		//如果打开的新路径为空或末尾没有斜杠，则在末尾加上一个
		path.append(1, L'\\');
	bool path_exist{ false };
	int track;
	int position;
	if (GetSongNum() > 0) EmplaceCurrentPathToRecent();		//如果当前路径有歌曲，就保存当前路径到最近路径
	//检查打开的路径是否已经存在于最近路径中
	for (const auto& a_path_info : m_recent_path)
	{
		if (path == a_path_info.path)
		{
			path_exist = true;
			track = a_path_info.track;
			position = a_path_info.position;
			m_sort_mode = a_path_info.sort_mode;
			break;
		}
	}
	if (path_exist)			//如果打开的路径已经存在于最近路径中
	{
		ChangePath(path, track);
		m_current_position_int = position;
		m_current_position.int2time(m_current_position_int);
		MusicControl(Command::SEEK);
		EmplaceCurrentPathToRecent();		//保存打开的路径到最近路径
		SaveRecentPath();
	}
	else		//如果打开的路径是新的路径
	{
		m_sort_mode = SM_FILE;
		ChangePath(path);
		EmplaceCurrentPathToRecent();		//保存新的路径到最近路径
		SaveRecentPath();
	}
}

void CPlayer::OpenFiles(const vector<wstring>& files, bool play)
{
	if (files.empty()) return;
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	if (GetSongNum() > 0) EmplaceCurrentPathToRecent();		//先保存当前路径和播放进度到最近路径
	size_t index;
	wstring path;
	index = files[0].find_last_of(L'\\');
	path = files[0].substr(0, index + 1);		//获取路径
	if (path != m_path)		//如果打开的文件在新的路径中，就清除播放列表，否则，在原有列表中添加
	{
		m_path = path;
		m_playlist.clear();
		m_current_position_int = 0;
		m_current_position = { 0,0,0 };
		m_index = 0;
	}
	//EmplaceCurrentPathToRecent();
	SongInfo song_info;
	for (const auto& file : files)
	{
		index = file.find_last_of(L'\\');
		song_info.file_name = file.substr(index + 1);
		m_playlist.push_back(song_info);	//将文件名储存到播放列表
	}
	IniPlayList(true);
	MusicControl(Command::OPEN);
	MusicControl(Command::SEEK);
	if (play)
		//MusicControl(Command::PLAY);
		PlayTrack(GetSongNum() - files.size());	//打开文件后播放添加的第1首曲目
	//IniLyrics();
	SetTitle();		//用当前正在播放的歌曲名作为窗口标题
	//SetVolume();
}

void CPlayer::OpenAFile(wstring file)
{
	if (file.empty()) return;
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	if (GetSongNum() > 0) EmplaceCurrentPathToRecent();		//先保存当前路径和播放进度到最近路径

	CFilePathHelper file_path(file);
	m_path = file_path.GetDir();
	m_playlist.clear();
	m_current_position_int = 0;
	m_current_position = { 0,0,0 };
	m_index = 0;
	//m_current_file_name = file.substr(index + 1);
	//m_song_num = 1;

	//获取打开路径的排序方式
	m_sort_mode = SortMode::SM_FILE;
	for (const auto& path_info : m_recent_path)
	{
		if (m_path == path_info.path)
			m_sort_mode = path_info.sort_mode;
	}

	//初始化播放列表
	m_current_file_name_tmp = GetCurrentFileName();
	IniPlayList(false, false);		//根据新路径重新初始化播放列表
}

void CPlayer::SetRepeatMode()
{
	int repeat_mode{ static_cast<int>(m_repeat_mode) };
	repeat_mode++;
	if (repeat_mode > 3)
		repeat_mode = 0;
	m_repeat_mode = static_cast<RepeatMode>(repeat_mode);
	SaveConfig();
}

void CPlayer::SetRepeatMode(RepeatMode repeat_mode)
{
	m_repeat_mode = repeat_mode;
	SaveConfig();
}

RepeatMode CPlayer::GetRepeatMode() const
{
	return m_repeat_mode;
}

bool CPlayer::GetBASSError()
{
	if (m_loading)
		return false;
	int error_code_tmp = BASS_ErrorGetCode();
	if (error_code_tmp && error_code_tmp != m_error_code)
	{
		CString info = CCommon::LoadTextFormat(IDS_BASS_ERROR_LOG_INFO, {error_code_tmp, GetCurrentFilePath()});
		theApp.WriteErrorLog(wstring{ info });
	}
	m_error_code = error_code_tmp;
	return true;
}

bool CPlayer::IsError() const
{
	if (m_loading)		//如果播放列表正在加载，则不检测错误
		return false;
	else
		return (m_error_code != 0 || m_musicStream == 0);
}

void CPlayer::SetTitle() const
{
//#ifdef _DEBUG
//	SetWindowText(theApp.m_pMainWnd->m_hWnd, (m_current_file_name + L" - MusicPlayer2(DEBUG模式)").c_str());		//用当前正在播放的歌曲名作为窗口标题
//#else
//	SetWindowText(theApp.m_pMainWnd->m_hWnd, (m_current_file_name + L" - MusicPlayer2").c_str());		//用当前正在播放的歌曲名作为窗口标题
//#endif
	SendMessage(theApp.m_pMainWnd->m_hWnd, WM_SET_TITLE, 0, 0);
}

void CPlayer::SaveConfig() const
{
	CIniHelper ini(theApp.m_config_path);

	//ini.WriteString(L"config", L"path", m_path.c_str());
	//ini.WriteInt(L"config", L"track", m_index);
	ini.WriteInt(L"config", L"volume", m_volume);
	//ini.WriteInt(L"config", L"position", m_current_position_int);
	ini.WriteInt(L"config", L"repeat_mode", static_cast<int>(m_repeat_mode));
	ini.WriteBool(L"config", L"lyric_karaoke_disp", theApp.m_lyric_setting_data.lyric_karaoke_disp);
	ini.WriteString(L"config",L"lyric_path", theApp.m_lyric_setting_data.lyric_path);
	ini.WriteInt(L"config", L"sort_mode", static_cast<int>(m_sort_mode));
	ini.WriteBool(L"config", L"lyric_fuzzy_match", theApp.m_lyric_setting_data.lyric_fuzzy_match);
	ini.WriteString(L"config",L"default_album_file_name", CCommon::StringMerge(theApp.m_app_setting_data.default_album_name, L','));

	//保存均衡器设定
	ini.WriteBool(L"equalizer", L"equalizer_enable", m_equ_enable);
	//保存每个均衡器通道的增益
	//if (m_equ_style == 9)
	//{
	//	wchar_t buff[16];
	//	for (int i{}; i < EQU_CH_NUM; i++)
	//	{
	//		swprintf_s(buff, L"channel%d", i + 1);
	//		ini.WriteInt(L"equalizer", buff, m_equalizer_gain[i]);
	//	}
	//}
	//保存混响设定
	ini.WriteInt(L"reverb", L"reverb_enable", m_reverb_enable);
	ini.WriteInt(L"reverb", L"reverb_mix", m_reverb_mix);
	ini.WriteInt(L"reverb", L"reverb_time", m_reverb_time);

	ini.Save();
}

void CPlayer::LoadConfig()
{
	CIniHelper ini(theApp.m_config_path);

	//ini.GetString(L"config", L"path", L".\\songs\\");
	//m_path = buff;
	//m_index =ini.GetInt(L"config", L"track", 0);
	m_volume =ini.GetInt(L"config", L"volume", 60);
	//m_current_position_int =ini.GetInt(L"config", L"position", 0);
	//m_current_position.int2time(m_current_position_int);
	m_repeat_mode = static_cast<RepeatMode>(ini.GetInt(L"config", L"repeat_mode", 0));
	theApp.m_lyric_setting_data.lyric_path = ini.GetString(L"config", L"lyric_path", L".\\lyrics\\");
	if (!theApp.m_lyric_setting_data.lyric_path.empty() && theApp.m_lyric_setting_data.lyric_path.back() != L'/' && theApp.m_lyric_setting_data.lyric_path.back() != L'\\')
		theApp.m_lyric_setting_data.lyric_path.append(1, L'\\');
	theApp.m_lyric_setting_data.lyric_karaoke_disp =ini.GetBool(L"config", L"lyric_karaoke_disp", true);
	m_sort_mode = static_cast<SortMode>(ini.GetInt(L"config", L"sort_mode", 0));
	theApp.m_lyric_setting_data.lyric_fuzzy_match =ini.GetBool(L"config", L"lyric_fuzzy_match", true);
	wstring default_album_name = ini.GetString(L"config", L"default_album_file_name", L"cover");
	CCommon::StringSplit(default_album_name, L',', theApp.m_app_setting_data.default_album_name);

	//读取均衡器设定
	m_equ_enable =ini.GetBool(L"equalizer", L"equalizer_enable", false);
	m_equ_style =ini.GetInt(L"equalizer", L"equalizer_style", 0);	//读取均衡器预设
	if (m_equ_style == 9)		//如果均衡器预设为“自定义”
	{
		//读取每个均衡器通道的增益
		for (int i{}; i < EQU_CH_NUM; i++)
		{
			wchar_t buff[16];
			swprintf_s(buff, L"channel%d", i + 1);
			m_equalizer_gain[i] =ini.GetInt(L"equalizer", buff, 0);
		}
	}
	else if (m_equ_style >= 0 && m_equ_style < 9)		//否则，根据均衡器预设设置每个通道的增益
	{
		for (int i{}; i < EQU_CH_NUM; i++)
		{
			m_equalizer_gain[i] = EQU_STYLE_TABLE[m_equ_style][i];
		}
	}
	//读取混响设定
	m_reverb_enable =ini.GetBool(L"reverb", L"reverb_enable", 0);
	m_reverb_mix =ini.GetInt(L"reverb", L"reverb_mix", 45);		//混响强度默认为50
	m_reverb_time =ini.GetInt(L"reverb", L"reverb_time", 100);	//混响时间默认为1s
}

void CPlayer::ExplorePath(int track) const
{
	if (GetSongNum() > 0)
	{
		CString str;
		if (track < 0)		//track小于0，打开资源管理器后选中当前播放的文件
			str.Format(_T("/select,\"%s%s\""), m_path.c_str(), GetCurrentFileName().c_str());
		else if (track < GetSongNum())		//track为播放列表中的一个序号，打开资源管理器后选中指定的文件
			str.Format(_T("/select,\"%s%s\""), m_path.c_str(), m_playlist[track].file_name.c_str());
		else								//track超过播放列表中文件的数量，打开资源管理器后不选中任何文件
			str = m_path.c_str();
		ShellExecute(NULL, _T("open"),_T("explorer"), str, NULL, SW_SHOWNORMAL);
	}
}


void CPlayer::ExplorePath(const std::wstring& path)
{
	CString str;
	str.Format(_T("/select,\"%s\""), path.c_str());
	ShellExecute(NULL, _T("open"), _T("explorer"), str, NULL, SW_SHOWNORMAL);
}

void CPlayer::ExploreLyric() const
{
	if (!m_Lyrics.IsEmpty())
	{
		CString str;
		str.Format(_T("/select,\"%s\""), m_Lyrics.GetPathName().c_str());
		ShellExecute(NULL, _T("open"), _T("explorer"), str, NULL, SW_SHOWNORMAL);
	}
}


Time CPlayer::GetAllSongLength(int track) const
{
	if (m_playlist[track].duration) {
		return Time(m_playlist[track].duration);
	}

	if (track >= 0 && track < GetSongNum())
		return m_playlist[track].lengh;
	else
		return Time();
}

int CPlayer::GetSongNum() const
{
	return static_cast<int>(m_playlist.size());
}


std::wstring CPlayer::GetAppPath()
{
	wchar_t szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };

	GetModuleFileNameW(NULL, szFilePath, sizeof(szFilePath));
	_wsplitpath(szFilePath, szDrive, szDir, szFileName, szExt);

	wstring str(szDrive);
	str.append(szDir);
	return str;
}


std::wstring CPlayer::GetLyricPath()
{
	std::wstring lyricPath = GetAppPath() + L"lyrics\\";
	CFilePathHelper helper(lyricPath);
	if (!helper.DirectoryExist())
		helper.CreateDirectory();
	return lyricPath;
}


std::wstring CPlayer::GetSongPath()
{
	std::wstring songsPath = GetAppPath() + L"songs\\";
	CFilePathHelper helper(songsPath);
	if (!helper.DirectoryExist())
		helper.CreateDirectory();
	return songsPath;
}


std::wstring CPlayer::GetAlbumPath()
{
	std::wstring albumsPath = GetAppPath() + L"albums\\";
	CFilePathHelper helper(albumsPath);
	if (!helper.DirectoryExist())
		helper.CreateDirectory();
	return albumsPath;
}

wstring CPlayer::GetFileName() const
{
	return (GetCurrentFileName().empty() ? CCommon::LoadText(IDS_FILE_NOT_FOUND).GetString() : GetCurrentFileName());
}

void CPlayer::DeleteAlbumCover()
{
	if (!m_inner_cover)
	{
		if (CCommon::DeleteAFile(theApp.m_pMainWnd->GetSafeHwnd(), m_album_cover_path.c_str()) == 0)
			m_album_cover.Destroy();
	}
}

void CPlayer::ReloadPlaylist()
{
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	m_current_file_name_tmp = GetCurrentFileName();	//保存当前播放的曲目的文件名，用于在播放列表初始化结束时确保播放的还是之前播放的曲目
	m_playlist.clear();		//清空播放列表
	//初始化播放列表
	IniPlayList(false, true);		//根据新路径重新初始化播放列表
}

void CPlayer::RemoveSong(int index)
{
	if (m_loading) return;
	if (index >= 0 && index < GetSongNum())
	{
		m_playlist.erase(m_playlist.begin() + index);
		//m_song_num--;
		if (index == m_index)		//如果要删除的曲目是正在播放的曲目，重新播放当前曲目
		{
			if(GetSongNum() > 0)
				PlayTrack(m_index);
		}
		else if (index < m_index)	//如果要删除的曲目在正在播放的曲目之前，则正在播放的曲目序号减1
		{
			m_index--;
		}
		if (GetSongNum() == 0)
			m_playlist.push_back(SongInfo());
	}
}

void CPlayer::RemoveSongs(vector<int> indexes)
{
	int size = indexes.size();
	for (int i{}; i<size; i++)
	{
		RemoveSong(indexes[i]);
		if (i <= size - 2 && indexes[i + 1] > indexes[i])
		{
			for (int j{ i + 1 }; j < size; j++)
				indexes[j]--;
		}
	}
}

void CPlayer::ClearPlaylist()
{
	if (m_loading) return;
	m_playlist.clear();
	//m_song_num = 0;
	MusicControl(Command::STOP);
}

void CPlayer::SeekTo(int position)
{
	if (position > m_song_length_int)
		position = m_song_length_int;
	m_current_position_int = position;
	m_current_position.int2time(position);
	if (m_playlist[m_index].is_cue)
	{
		position += m_playlist[m_index].start_pos.time2int();
	}
	double pos_sec = static_cast<double>(position) / 1000.0;
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelSeconds2Bytes(m_musicStream, pos_sec);
	BASS_ChannelSetPosition(m_musicStream, pos_bytes, BASS_POS_BYTE);
	m_midi_lyric.clear();
	GetMidiPosition();
}

void CPlayer::SeekTo(double position)
{
	int pos = static_cast<int>(m_song_length_int*position);
	SeekTo(pos);
}

void CPlayer::SeekTo(HSTREAM hStream, int position)
{
	double pos_sec = static_cast<double>(position) / 1000.0;
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelSeconds2Bytes(hStream, pos_sec);
	BASS_ChannelSetPosition(hStream, pos_bytes, BASS_POS_BYTE);
}

void CPlayer::ClearLyric()
{
	m_Lyrics = CLyrics{};
	m_playlist[m_index].lyric_file.clear();
}

wstring CPlayer::GetTimeString() const
{
	wchar_t buff[16];
	swprintf_s(buff, L"%d:%.2d/%d:%.2d", m_current_position.min, m_current_position.sec, m_song_length.min, m_song_length.sec);
	return wstring(buff);
}

wstring CPlayer::GetPlayingState() const
{
	if (m_error_code != 0)
		return CCommon::LoadText(IDS_PLAY_ERROR).GetString();
	switch (m_playing)
	{
	case 0: return CCommon::LoadText(IDS_STOPED).GetString();
	case 1: return CCommon::LoadText(IDS_PAUSED).GetString();
	case 2: return CCommon::LoadText(IDS_NOW_PLAYING).GetString();
	}
	return wstring();
}

const SongInfo & CPlayer::GetCurrentSongInfo() const
{
	if (m_index >= 0 && m_index < GetSongNum())
		return m_playlist[m_index];
	else return m_no_use;
}

void CPlayer::SetRelatedSongID(wstring song_id)
{
	if (m_index >= 0 && m_index < GetSongNum())
	{
		m_playlist[m_index].song_id = song_id;
		if(!m_playlist[m_index].is_cue)
		{
			theApp.m_song_data[m_path + m_playlist[m_index].file_name] = m_playlist[m_index];
			theApp.SetSongDataModified();
		}
	}
}

void CPlayer::SetRelatedSongID(int index, wstring song_id)
{
	if (index >= 0 && index < GetSongNum())
	{
		m_playlist[index].song_id = song_id;
		if (!m_playlist[index].is_cue)
		{
			theApp.m_song_data[m_path + m_playlist[index].file_name] = m_playlist[index];
			theApp.SetSongDataModified();
		}
	}
}

void CPlayer::AddListenTime(int sec)
{
	if (m_index >= 0 && m_index < GetSongNum())
	{
		m_playlist[m_index].listen_time += sec;
		if (!m_playlist[m_index].is_cue)
		{
			theApp.m_song_data[m_path + m_playlist[m_index].file_name] = m_playlist[m_index];
			theApp.SetSongDataModified();
		}
	}
}

void CPlayer::ReIniBASS(bool replay)
{
	int playing = m_playing;
	UnInitBASS();
	IniBASS();
	MusicControl(Command::OPEN);
	MusicControl(Command::SEEK);
	if (replay && playing == 2)
	{
		MusicControl(Command::PLAY);
	}
	else
	{
		m_playing = 0;
	}
}

void CPlayer::SortPlaylist(bool change_index)
{
	if (m_loading) return;
	int track_number = m_playlist[m_index].track;
	wstring current_file_name = GetCurrentFileName();
	switch (m_sort_mode)
	{
	case SM_FILE: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByFileName);
		break;
	case SM_TITLE: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTitle);
		break;
	case SM_ARTIST: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByArtist);
		break;
	case SM_ALBUM: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByAlbum);
		break;
	case SM_TRACK: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTrack);
		break;
	default:
		break;
	}

	if (change_index)
	{
		//播放列表排序后，查找正在播放项目的序号
		if (!m_playlist[m_index].is_cue)
		{
			for (int i{}; i < GetSongNum(); i++)
			{
				if (current_file_name == m_playlist[i].file_name)
				{
					m_index = i;
					break;
				}
			}
		}
		else
		{
			for (int i{}; i < GetSongNum(); i++)
			{
				if (track_number == m_playlist[i].track)
				{
					m_index = i;
					break;
				}
			}
		}
	}
}


void CPlayer::SaveRecentPath() const
{
	// 打开或者新建文件
	CFile file;
	BOOL bRet = file.Open(theApp.m_recent_path_dat_path.c_str(),
		CFile::modeCreate | CFile::modeWrite);
	if (!bRet)		//打开文件失败
	{
		return;
	}
	// 构造CArchive对象
	CArchive ar(&file, CArchive::store);
	// 写数据
	ar << m_recent_path.size();		//写入m_recent_path容器的大小
	for (auto& path_info : m_recent_path)
	{
		ar << CString(path_info.path.c_str())
			<< path_info.track
			<< path_info.position
			<< static_cast<int>(path_info.sort_mode)
			<< path_info.track_num
			<< path_info.total_time;
	}
	// 关闭CArchive对象
	ar.Close();
	// 关闭文件
	file.Close();

}

void CPlayer::OnExit()
{
	SaveConfig();
	//退出时保存最后播放的曲目和位置
	if (!m_recent_path.empty() && GetSongNum() > 0 && !m_playlist[0].file_name.empty())
	{
		m_recent_path[0].track = m_index;
		m_recent_path[0].position = m_current_position_int;
	}
	SaveRecentPath();
}

void CPlayer::LoadRecentPath()
{
	// 打开文件
	CFile file;
	BOOL bRet = file.Open(theApp.m_recent_path_dat_path.c_str(), CFile::modeRead);
	if (!bRet)		//文件不存在
	{
		m_path = L".\\songs\\";		//默认的路径
		return;
	}
	// 构造CArchive对象
	CArchive ar(&file, CArchive::load);
	// 读数据
	size_t size{};
	PathInfo path_info;
	CString temp;
	int sort_mode;
	try
	{
		ar >> size;		//读取映射容器的长度
		for (size_t i{}; i < size; i++)
		{
			ar >> temp;
			path_info.path = temp;
			ar >> path_info.track;
			ar >> path_info.position;
			ar >> sort_mode;
			path_info.sort_mode = static_cast<SortMode>(sort_mode);
			ar >> path_info.track_num;
			ar >> path_info.total_time;
			if (path_info.path.empty() || path_info.path.size() < 2) continue;		//如果路径为空或路径太短，就忽略它
			if (path_info.path.back() != L'/' && path_info.path.back() != L'\\')	//如果读取到的路径末尾没有斜杠，则在末尾加上一个
				path_info.path.push_back(L'\\');
			m_recent_path.push_back(path_info);
		}
	}
	catch (CArchiveException* exception)
	{
		//捕获序列化时出现的异常
		CString info;
		info = CCommon::LoadTextFormat(IDS_RECENT_PATH_SERIALIZE_ERROR_LOG, { exception->m_cause });
		theApp.WriteErrorLog(wstring{ info });
	}
	// 关闭对象
	ar.Close();
	// 关闭文件
	file.Close();

	//从recent_path文件中获取路径、播放到的曲目和位置
	if (!m_recent_path.empty())
	{
		m_path = m_recent_path[0].path;
		if (!m_path.empty() && m_path.back() != L'/' && m_path.back() != L'\\')		//如果读取到的新路径末尾没有斜杠，则在末尾加上一个
			m_path.push_back(L'\\');

		m_index = m_recent_path[0].track;
		m_current_position_int = m_recent_path[0].position;
		m_current_position.int2time(m_current_position_int);
	}
	else
	{
		m_path = L".\\songs\\";		//默认的路径
	}
}

void CPlayer::EmplaceCurrentPathToRecent()
{
	for (size_t i{ 0 }; i < m_recent_path.size(); i++)
	{
		if (m_path == m_recent_path[i].path)
			m_recent_path.erase(m_recent_path.begin() + i);		//如果当前路径已经在最近路径中，就把它最近路径中删除
	}
	if (GetSongNum() == 0 || m_playlist[0].file_name.empty()) return;		//如果当前路径中没有文件，就不保存
	PathInfo path_info;
	path_info.path = m_path;
	path_info.track = m_index;
	path_info.position = m_current_position_int;
	path_info.sort_mode = m_sort_mode;
	path_info.track_num = GetSongNum();
	path_info.total_time = m_total_time;
	if (GetSongNum() > 0)
		m_recent_path.push_front(path_info);		//当前路径插入到m_recent_path的前面
}


void CPlayer::SetFXHandle()
{
	if (m_musicStream == 0) return;
	//if (!m_equ_enable) return;
	//设置每个均衡器通道的句柄
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		m_equ_handle[i] = BASS_ChannelSetFX(m_musicStream, BASS_FX_DX8_PARAMEQ, 1);
	}
	//设置混响的句柄
	m_reverb_handle = BASS_ChannelSetFX(m_musicStream, BASS_FX_DX8_REVERB, 1);
}

void CPlayer::RemoveFXHandle()
{
	if (m_musicStream == 0) return;
	//移除每个均衡器通道的句柄
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		if (m_equ_handle[i] != 0)
		{
			BASS_ChannelRemoveFX(m_musicStream, m_equ_handle[i]);
			m_equ_handle[i] = 0;
		}
	}
	//移除混响的句柄
	if (m_reverb_handle != 0)
	{
		BASS_ChannelRemoveFX(m_musicStream, m_reverb_handle);
		m_reverb_handle = 0;
	}
}

void CPlayer::ApplyEqualizer(int channel, int gain)
{
	if (channel < 0 || channel >= EQU_CH_NUM) return;
	//if (!m_equ_enable) return;
	if (gain < -15) gain = -15;
	if (gain > 15) gain = 15;
	BASS_DX8_PARAMEQ parameq;
	parameq.fBandwidth = 30;
	parameq.fCenter = FREQ_TABLE[channel];
	parameq.fGain = static_cast<float>(gain);
	BASS_FXSetParameters(m_equ_handle[channel], &parameq);
}

void CPlayer::SetEqualizer(int channel, int gain)
{
	if (channel < 0 || channel >= EQU_CH_NUM) return;
	m_equalizer_gain[channel] = gain;
	ApplyEqualizer(channel, gain);
}

int CPlayer::GeEqualizer(int channel)
{
	if (channel < 0 || channel >= EQU_CH_NUM) return 0;
	//BASS_DX8_PARAMEQ parameq;
	//int rtn;
	//rtn = BASS_FXGetParameters(m_equ_handle[channel], &parameq);
	//return static_cast<int>(parameq.fGain);
	return m_equalizer_gain[channel];
}

void CPlayer::SetAllEqualizer()
{
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		ApplyEqualizer(i, m_equalizer_gain[i]);
	}
}

void CPlayer::ClearAllEqulizer()
{
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		ApplyEqualizer(i, 0);
	}
}

void CPlayer::EnableEqualizer(bool enable)
{
	if (enable)
		SetAllEqualizer();
	else
		ClearAllEqulizer();
	m_equ_enable = enable;
}

void CPlayer::SetReverb(int mix, int time)
{
	if (mix < 0) mix = 0;
	if (mix > 100) mix = 100;
	if (time < 1) time = 1;
	if (time > 300) time = 300;
	m_reverb_mix = mix;
	m_reverb_time = time;
	BASS_DX8_REVERB parareverb;
	parareverb.fInGain = 0;
	//parareverb.fReverbMix = static_cast<float>(mix) / 100 * 96 - 96;
	parareverb.fReverbMix = static_cast<float>(std::pow(static_cast<double>(mix) / 100, 0.1) * 96 - 96);
	parareverb.fReverbTime = static_cast<float>(time * 10);
	parareverb.fHighFreqRTRatio = 0.001f;
	BASS_FXSetParameters(m_reverb_handle, &parareverb);
}

void CPlayer::ClearReverb()
{
	BASS_DX8_REVERB parareverb;
	parareverb.fInGain = 0;
	parareverb.fReverbMix = -96;
	parareverb.fReverbTime = 0.001f;
	parareverb.fHighFreqRTRatio = 0.001f;
	BASS_FXSetParameters(m_reverb_handle, &parareverb);
}

void CPlayer::EnableReverb(bool enable)
{
	if (enable)
		SetReverb(m_reverb_mix, m_reverb_time);
	else
		ClearReverb();
	m_reverb_enable = enable;
}


void CPlayer::ConnotPlayWarning() const
{
	if (m_is_midi && m_sfont.font == 0)
		PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_CONNOT_PLAY_WARNING, 0, 0);
}

void CPlayer::SearchAlbumCover()
{
	//static wstring last_file_path;
	//if (last_file_path != m_path + GetCurrentFileName())		//防止同一个文件多次获取专辑封面
	//{
	m_album_cover.Destroy();
	if (!theApp.m_app_setting_data.use_out_image || theApp.m_app_setting_data.use_inner_image_first)
	{
		//从文件获取专辑封面
		CAudioTag audio_tag(m_musicStream, m_path + GetCurrentFileName(), m_playlist[m_index]);
		m_album_cover_path = audio_tag.GetAlbumCover(m_album_cover_type);
		m_album_cover.Load(m_album_cover_path.c_str());
	}
	m_inner_cover = !m_album_cover.IsNull();

	if (/*theApp.m_app_setting_data.use_out_image && */m_album_cover.IsNull())
	{
		//获取不到专辑封面时尝试使用外部图片作为封面
		SearchOutAlbumCover();
	}
	//AlbumCoverGaussBlur();
	//}
	//last_file_path = m_path + GetCurrentFileName();
}

void CPlayer::AlbumCoverGaussBlur()
{
	if (!theApp.m_app_setting_data.background_gauss_blur || !theApp.m_app_setting_data.album_cover_as_background)
		return;
	if (m_album_cover.IsNull())
	{
		m_album_cover_blur.Destroy();
	}
	else
	{
		CImage image_tmp;
		CSize image_size(m_album_cover.GetWidth(), m_album_cover.GetHeight());
		//将图片缩小以减小高斯模糊的计算量
		CCommon::SizeZoom(image_size, 300);		//图片大小按比例缩放，使长边等于300
		if (!CDrawCommon::BitmapStretch(&m_album_cover, &image_tmp, image_size))		//拉伸图片
			return;
#ifdef _DEBUG
		image_tmp.Save(_T("..\\Debug\\image_tmp.bmp"), Gdiplus::ImageFormatBMP);
#endif // _DEBUG

		//执行高斯模糊
		CGaussBlur gauss_blur;
		gauss_blur.SetSigma(static_cast<double>(theApp.m_app_setting_data.gauss_blur_radius) / 10);		//设置高斯模糊半径
		gauss_blur.DoGaussBlur(image_tmp, m_album_cover_blur);
	}
}

void CPlayer::GetMidiPosition()
{
	if (m_is_midi)
	{
		//获取midi音乐的进度并转换成节拍数。（其中+ (m_midi_info.ppqn / 4)的目的是修正显示的节拍不准确的问题）
		m_midi_info.midi_position = static_cast<int>((BASS_ChannelGetPosition(m_musicStream, BASS_POS_MIDI_TICK) + (m_midi_info.ppqn / 4)) / m_midi_info.ppqn);
	}
}

wstring CPlayer::GetCurrentFileName() const
{
	if (m_index >= 0 && m_index < GetSongNum())
		return m_playlist[m_index].file_name;
	else
		return wstring();
}


wstring CPlayer::GetCurrentPlayURL() const
{
	if (m_index >= 0 && m_index < GetSongNum())
		return m_playlist[m_index].play_url;
	else
		return wstring();
}

void CPlayer::AcquireSongInfo(HSTREAM hStream, const wstring& file_path, SongInfo & song_info)
{
	//获取长度
	song_info.lengh = GetBASSSongLength(hStream);
	//获取比特率
	float bitrate{};
	BASS_ChannelGetAttribute(hStream, BASS_ATTRIB_BITRATE, &bitrate);
	song_info.bitrate = static_cast<int>(bitrate + 0.5f);
	//获取音频标签
	CAudioTag audio_tag(hStream, file_path, song_info);
	audio_tag.GetAudioTag(theApp.m_general_setting_data.id3v2_first);
	//获取midi音乐的标题
	if (m_bass_midi_lib.IsSuccessed() && audio_tag.GetAudioType() == AU_MIDI)
	{
		BASS_MIDI_MARK mark;
		if (m_bass_midi_lib.BASS_MIDI_StreamGetMark(hStream, BASS_MIDI_MARK_TRACK, 0, &mark) && !mark.track)
		{
			song_info.title = CCommon::StrToUnicode(mark.text);
			song_info.info_acquired = true;
		}
	}
	CFilePathHelper c_file_path(file_path);
	song_info.file_name = c_file_path.GetFileName();
	//保存歌曲信息
	SongInfo& song = theApp.m_song_data[file_path];
	song.CopyAudioTag(song_info);
	song.lengh = song_info.lengh;
	song.bitrate = song_info.bitrate;
}

void CPlayer::SearchOutAlbumCover()
{
	m_album_cover_path = GetRelatedAlbumCover(m_path + GetCurrentFileName(), GetCurrentSongInfo());
	if (!m_album_cover.IsNull())
		m_album_cover.Destroy();
	m_album_cover.Load(m_album_cover_path.c_str());
}

wstring CPlayer::GetRelatedAlbumCover(const wstring& file_path, const SongInfo& song_info)
{
	vector<wstring> files;
	wstring file_name;
	//查找文件和歌曲名一致的图片文件
	CFilePathHelper c_file_path(file_path);
	//file_name = m_path + c_file_name.GetFileNameWithoutExtension() + L".*";
	c_file_path.ReplaceFileExtension(L"*");
	wstring dir{ c_file_path.GetDir() };
	CCommon::GetImageFiles(c_file_path.GetFilePath(), files);
	if (files.empty() && !song_info.album.empty())
	{
		//没有找到和歌曲名一致的图片文件，则查找文件名为“唱片集”的文件
		wstring album_name{ song_info.album };
		CCommon::FileNameNormalize(album_name);
		file_name = dir + album_name + L".*";
		CCommon::GetImageFiles(file_name, files);
	}
	//if (files.empty() && !theApp.m_app_setting_data.default_album_name.empty())
	//{
	//	//没有找到唱片集为文件名的文件，查找文件名为DEFAULT_ALBUM_NAME的文件
	//	file_name = m_path + theApp.m_app_setting_data.default_album_name + L".*";
	//	CCommon::GetImageFiles(file_name, files);
	//}
	//没有找到唱片集为文件名的文件，查找文件名为设置的专辑封面名的文件
	if (theApp.m_app_setting_data.use_out_image)
	{
		for (const auto& album_name : theApp.m_app_setting_data.default_album_name)
		{
			if (!files.empty())
				break;
			if (!album_name.empty())
			{
				file_name = dir + album_name + L".*";
				CCommon::GetImageFiles(file_name, files);
			}
		}
	}
	//if (files.empty())
	//{
	//	//没有找到文件名为DEFAULT_ALBUM_NAME的文件，查找名为“Folder的文件”
	//	file_name = m_path + L"Folder" + L".*";
	//	CCommon::GetImageFiles(file_name, files);
	//}
	if (!files.empty())
	{
		//m_album_cover_path = m_path + files[0];
		//if (!m_album_cover.IsNull())
		//	m_album_cover.Destroy();
		//m_album_cover.Load(m_album_cover_path.c_str());
		return wstring(dir + files[0]);
	}
	else
	{
		return wstring();
	}
}
