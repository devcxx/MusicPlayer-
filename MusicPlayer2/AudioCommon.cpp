#include "stdafx.h"
#include "AudioCommon.h"

vector<SupportedFormat> CAudioCommon::m_surpported_format;
vector<wstring> CAudioCommon::m_all_surpported_extensions;

CAudioCommon::CAudioCommon()
{
}


CAudioCommon::~CAudioCommon()
{
}

bool CAudioCommon::FileIsAudio(const wstring & file_name)
{
	CFilePathHelper file_path(file_name);
	wstring extension{ file_path.GetFileExtension() };		//��ȡ�ļ���չ��
	for (const auto& ext : m_all_surpported_extensions)		//�ж��ļ���չ�Ƿ���֧�ֵ���չ���б���
	{
		if (ext == extension)
			return true;
	}
	return false;
}

AudioType CAudioCommon::GetAudioTypeByExtension(const wstring & file_name)
{
	CFilePathHelper file_path(file_name);
	wstring type{ file_path.GetFileExtension() };		//��ȡ�ļ���չ��
	if (type == L"mp3")
		return AU_MP3;
	else if (type == L"wma")
		return AU_WMA;
	else if (type == L"ogg" || type == L"oga")
		return AU_OGG;
	else if (type == L"m4a")
		return AU_MP4;
	else if (type == L"mp4")
		return AU_MP4;
	else if (type == L"flac" || type == L"fla")
		return AU_FLAC;
	else if (type == L"cue")
		return AU_CUE;
	else if (type == L"ape")
		return AU_APE;
	else if (type == L"mid" || type == L"midi" || type == L"rmi" || type == L"kar")
		return AU_MIDI;
	else
		return AU_OTHER;
}

wstring CAudioCommon::GetAudioDescriptionByExtension(wstring extension)
{
	CCommon::StringTransform(extension, false);
	if (extension == L"mp3")
		return wstring(CCommon::LoadText(_T("MP3 "), IDS_AUDIO_FILE));
	else if (extension == L"wma")
		return wstring(CCommon::LoadText(_T("Windows Media "), IDS_AUDIO_FILE));
	else if (extension == L"wav")
		return wstring(CCommon::LoadText(_T("WAV "), IDS_AUDIO_FILE));
	else if (extension == L"mid" || extension == L"midi")
		return wstring(CCommon::LoadText(IDS_MINI_SEQUENCE));
	else if (extension == L"ogg")
		return wstring(CCommon::LoadText(_T("OGG "), IDS_AUDIO_FILE));
	else if (extension == L"m4a")
		return wstring(CCommon::LoadText(_T("MPEG-4 "), IDS_AUDIO_FILE));
	else if (extension == L"ape")
		return wstring(L"Monkey's Audio (APE)");
	else if (extension == L"aac")
		return wstring(L"Advanced Audio Coding (AAC)");
	else if (extension == L"cda")
		return wstring(CCommon::LoadText(_T("CD "), IDS_AUDIO_FILE, _T(" (CDA)")));
	else
		return wstring(extension + CCommon::LoadText(_T(" "), IDS_AUDIO_FILE).GetString());
}

void CAudioCommon::GetAudioFiles(wstring path, vector<SongInfo>& files, size_t max_file)
{
	//�ļ���� 
	intptr_t hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	SongInfo song_info;
	if ((hFile = _wfindfirst(path.append(L"\\*.*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (files.size() >= max_file) break;
			if (FileIsAudio(wstring(fileinfo.name)))	//����ҵ����ļ�����Ƶ�ļ����򱣴浽������
			{
				song_info.file_name = fileinfo.name;
				files.push_back(song_info);
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CAudioCommon::GetLyricFiles(wstring path, vector<wstring>& files)
{
	//�ļ���� 
	intptr_t hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	//wstring file_path;
	if ((hFile = _wfindfirst(path.append(L"\\*.lrc").c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(fileinfo.name);  //���ļ�������
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CAudioCommon::GetCueTracks(vector<SongInfo>& files, wstring path)
{
	for (size_t i{}; i < files.size(); i++)
	{
		//���μ���б��е�ÿ�׸����Ƿ�Ϊcue�ļ�
		if (GetAudioTypeByExtension(files[i].file_name) == AU_CUE)
		{
			wstring cue_file_name{ files[i].file_name };		//cue�ļ����ļ���
			files.erase(files.begin() + i);		//���б���ɾ��cue�ļ�
			wstring cue_file_name2;			//cue�ļ����ļ�����������չ����
			size_t index = cue_file_name.rfind(L'.');
			cue_file_name2 = cue_file_name.substr(0, index);
			//���Һ�cue�ļ�ͬ������Ƶ�ļ�
			wstring play_file_name;		//���ҵ��ĺ�cue�ļ�ͬ�����ļ���
			wstring play_file_name2;		//���ҵ��ĺ�cue�ļ�ͬ�����ļ�����������չ����
			int bitrate;
			Time total_length;
			bool matched_file_found{ false };		//������ҵ��˺�cue�ļ���ͬ���ļ�������Ϊtrue
			for (size_t j{}; j < files.size(); j++)
			{
				if (GetAudioTypeByExtension(files[j].file_name) != AU_CUE && !files[j].is_cue)	//ȷ�����ļ�����cue�ļ����Ҳ����Ѿ���������cue����
				{
					play_file_name = files[j].file_name;		//�ű����ļ���
					bitrate = files[j].bitrate;			//�����ȡ���ı�����
					total_length = files[j].lengh;
					size_t index2 = play_file_name.rfind(L'.');
					play_file_name2 = play_file_name.substr(0, index2);
					if (play_file_name2 == cue_file_name2)
					{
						files.erase(files.begin() + j);		//���б���ɾ�����ļ�
						matched_file_found = true;
						break;
					}
				}
			}
			if (!matched_file_found)		//���û���ҵ���cueͬ�����ļ��������������һ��cue�ļ�
				continue;

			//����cue�ļ�
			string cue_file_contents;
			ifstream OpenFile{ path + cue_file_name };
			if (OpenFile.fail())
				return;
			string current_line;
			char ch;
			while (!OpenFile.eof())
			{
				//std::getline(OpenFile, current_line);
				//cue_file_contents += current_line;
				OpenFile.get(ch);
				cue_file_contents.push_back(ch);
				if (cue_file_contents.size() > 102400) break;	//����cue�ļ����Ϊ100KB
			}
			CodeType code_type{ CodeType::AUTO };		//cue�ļ��ı�������
			if (cue_file_contents.size() >= 3 && cue_file_contents[0] == -17 && cue_file_contents[1] == -69 && cue_file_contents[2] == -65)
				code_type = CodeType::UTF8;
			//��ȡcue�ļ���ר������
			string album_name;
			size_t index1 = cue_file_contents.find("TITLE");
			size_t index2 = cue_file_contents.find('\"', index1);
			size_t index3 = cue_file_contents.find('\"', index2 + 1);
			album_name = cue_file_contents.substr(index2 + 1, index3 - index2 - 1);

			SongInfo song_info{};
			song_info.album = CCommon::StrToUnicode(album_name, code_type);
			song_info.file_name = play_file_name;
			song_info.bitrate = bitrate;
			song_info.is_cue = true;
			song_info.info_acquired = true;

			size_t index_track{};
			size_t index_title{};
			size_t index_artist{};
			size_t index_pos{};
			while (true)
			{
				//������Ŀ���
				index_track = cue_file_contents.find("TRACK ", index_track + 6);
				if (index_track == string::npos)
					break;
				string track_str = cue_file_contents.substr(index_track + 6, 3);
				song_info.track = atoi(track_str.c_str());
				size_t next_track_index = cue_file_contents.find("TRACK ", index_track + 6);
				//������Ŀ����
				index_title = cue_file_contents.find("TITLE ", index_track + 6);
				if (index_title < next_track_index)
				{
					index2 = cue_file_contents.find('\"', index_title);
					index3 = cue_file_contents.find('\"', index2 + 1);
					song_info.title = CCommon::StrToUnicode(cue_file_contents.substr(index2 + 1, index3 - index2 - 1), code_type);
				}
				else
				{
					song_info.title = CCommon::LoadText(IDS_DEFAULT_TITLE);
				}
				//������Ŀ������
				index_artist = cue_file_contents.find("PERFORMER ", index_track + 6);
				if (index_artist < next_track_index)
				{
					index2 = cue_file_contents.find('\"', index_artist);
					index3 = cue_file_contents.find('\"', index2 + 1);
					song_info.artist = CCommon::StrToUnicode(cue_file_contents.substr(index2 + 1, index3 - index2 - 1), code_type);
				}
				else
				{
					song_info.artist = CCommon::LoadText(IDS_DEFAULT_ARTIST);
				}
				//������Ŀλ��
				index_pos = cue_file_contents.find("INDEX ", index_track + 6);
				index1 = cue_file_contents.find(":", index_pos + 6);
				index2 = cue_file_contents.rfind(" ", index1);
				string tmp;
				Time time;
				//��ȡ����
				tmp = cue_file_contents.substr(index2 + 1, index1 - index2 - 1);
				time.min = atoi(tmp.c_str());
				//��ȡ����
				tmp = cue_file_contents.substr(index1 + 1, 2);
				time.sec = atoi(tmp.c_str());
				//��ȡ����
				tmp = cue_file_contents.substr(index1 + 4, 2);
				time.msec = atoi(tmp.c_str()) * 10;

				song_info.start_pos = time;
				if (!time.isZero() && !files.empty())
				{
					files.back().end_pos = time;
					files.back().lengh = Time(time - files.back().start_pos);
				}

				files.push_back(song_info);
			}
			files.back().lengh = Time(total_length - files.back().start_pos);
			i--;		//������һ��cue�ļ������ڸ�cue�ļ��Ѿ����Ƴ������Խ�ѭ��������1
		}
	}
}

void CAudioCommon::CheckCueFiles(vector<SongInfo>& files, wstring path)
{
	bool audio_exist;
	int size = files.size();
	for (int i{}; i < size; i++)
	{
		if (GetAudioTypeByExtension(files[i].file_name) == AU_CUE)		//�����б��е�cue�ļ�
		{
			audio_exist = false;
			wstring file_name;
			size_t index;
			index = files[i].file_name.rfind(L'.');
			file_name = files[i].file_name.substr(0, index);		//��ȡ�ļ�����������չ����
			//���Һ�cue�ļ�ƥ�����Ƶ�ļ�
			for (int j{}; j < size; j++)
			{
				if (GetAudioTypeByExtension(files[j].file_name) != AU_CUE)
				{
					wstring audio_file_name;
					index = files[j].file_name.rfind(L'.');
					audio_file_name = files[j].file_name.substr(0, index);
					if (file_name == audio_file_name)
					{
						audio_exist = true;
						break;
					}
				}
			}
			//û���ҵ�ƥ�����Ƶ�ļ�������Ŀ¼������ƥ�����Ƶ�ļ�
			if (!audio_exist)
			{
				vector<wstring> audio_files;
				CString find_file_name;
				find_file_name.Format(_T("%s%s.*"), path.c_str(), file_name.c_str());
				CCommon::GetFiles(wstring(find_file_name), audio_files);
				for (const auto& file : audio_files)
				{
					if (GetAudioTypeByExtension(file) != AU_CUE)
					{
						SongInfo song_info;
						song_info.file_name = file;
						files.push_back(song_info);
						return;
					}
				}
			}
		}
	}
}


wstring CAudioCommon::GetGenre(BYTE genre)
{
	if (genre < GENRE_MAX)
		return GENRE_TABLE[genre];
	else
		return CCommon::LoadText(IDS_DEFAULT_GENRE).GetString();
}

wstring CAudioCommon::GenreConvert(wstring genre)
{
	if(genre.empty())
		return wstring();
	if (genre[0] == L'(')		//���ǰ�������ţ���ɾ������
		genre = genre.substr(1);
	if (genre.back() == L')')
		genre.pop_back();
	if (CCommon::StrIsNumber(genre))		//���������Ϣ�����֣���ת��Ϊ��׼������Ϣ
	{
		int n_genre = _wtoi(genre.c_str());
		if(n_genre<256)
			return GetGenre(static_cast<BYTE>(n_genre));
	}
	return genre;
}


void CAudioCommon::TagStrNormalize(wstring & str)
{
	for (size_t i{}; i < str.size(); i++)
	{
		if (str[i] < 32 || str[i] >= static_cast<wchar_t>(0xfff0))
		{
			str = str.substr(0, i);
			return;
		}
	}

}

wstring CAudioCommon::GetBASSChannelDescription(DWORD ctype)
{
	switch (ctype)
	{
	case 0: return CCommon::LoadText(IDS_UNKNOW).GetString();
	case 1: return L"SAMPLE";
	case 2: return L"RECORD";
	case 0x10000: return L"STREAM";
	case 0x10002: return L"OGG";
	case 0x10003: return L"MP1";
	case 0x10004: return L"MP2";
	case 0x10005: return L"MP3";
	case 0x10006: return L"AIFF";
	case 0x10007: return L"CA";
	case 0x10008: return L"MF";
	case 0x10009: return L"AM";
	case 0x18000: return L"DUMMY";
	case 0x18001: return L"DEVICE";
	case 0x40000: return L"WAV";
	case 0x50001: return L"WAV_PCM";
	case 0x50003: return L"WAV_FLOAT";
	case 0x20000: return L"MOD";
	case 0x20001: return L"MTM";
	case 0x20002: return L"S3M";
	case 0x20003: return L"XM";
	case 0x20004: return L"IT";
	case 0x00100: return L"MO3";
	case 0x10e00: return L"ALAC";
	case 0x10200: return L"CD";
	case 0x10900: return L"FLAC";
	case 0x10901: return L"FLAC_OGG";
	case 0x10d00: return L"MIDI";
	case 0x10300: return L"WMA";
	case 0x10301: return L"WMA_MP3";
	case 0x10500: return L"WV";
	case 0x10b00: return L"AAC";
	case 0x10b01: return L"MP4";
	case 0x10700: return L"APE";
	default: return wstring();
	}
}

AudioType CAudioCommon::GetAudioTypeByBassChannel(DWORD ctype)
{
	AudioType type;
	switch (ctype)
	{
	case BASS_CTYPE_STREAM_MP1: case BASS_CTYPE_STREAM_MP2: case BASS_CTYPE_STREAM_MP3:
		type = AudioType::AU_MP3;
		break;
	case 0x10300: case 0x10301:
		type = AudioType::AU_WMA;
		break;
	case BASS_CTYPE_STREAM_OGG:
		type = AudioType::AU_OGG;
		break;
	case 0x10b01:
		type = AudioType::AU_MP4;
		break;
	case 0x10900: case 0x10901:
		type = AudioType::AU_FLAC;
		break;
	case 0x10d00:
		type = AudioType::AU_MIDI;
		break;
	case 0x10700:
		type = AudioType::AU_APE;
		break;
	default:
		type = AudioType::AU_OTHER;
		break;
	}
	return type;
}

CString CAudioCommon::TrackToString(BYTE track)
{
	CString str;
	if (track != 0)
	{
		str.Format(_T("%d"), track);
		return str;
	}
	else
	{
		return CString();
	}
}

void CAudioCommon::ClearDefaultTagStr(SongInfo & song_info)
{
	if (song_info.title == CCommon::LoadText(IDS_DEFAULT_TITLE).GetString())
		song_info.title.clear();
	if (song_info.artist == CCommon::LoadText(IDS_DEFAULT_ARTIST).GetString())
		song_info.artist.clear();
	if (song_info.album == CCommon::LoadText(IDS_DEFAULT_ALBUM).GetString())
		song_info.album.clear();
	if (song_info.year == CCommon::LoadText(IDS_DEFAULT_YEAR).GetString())
		song_info.year.clear();
	if (song_info.genre == CCommon::LoadText(IDS_DEFAULT_GENRE).GetString())
		song_info.genre.clear();
}

