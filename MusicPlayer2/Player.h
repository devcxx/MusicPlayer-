#pragma once
#include"Common.h"
#include"Lyric.h"
#include"SetPathDlg.h"
#include"AudioTag.h"
#include "FilePathHelper.h"
#include "BASSMidiLibrary.h"
//#include"MusicPlayerDlg.h"
#include "GaussBlur.h"

#define WM_PLAYLIST_INI_START (WM_USER+104)			//�����б�ʼ����ʱ����Ϣ
#define WM_PLAYLIST_INI_COMPLATE (WM_USER+105)		//�����б���������Ϣ
#define WM_SET_TITLE (WM_USER+106)					//���ô��ڱ������Ϣ
#define WM_CONNOT_PLAY_WARNING (WM_USER+108)		//�޷������ļ�ʱ����������ʾ�����Ϣ
#define WM_MUSIC_STREAM_OPENED (WM_USER+109)		//����Ƶ�ļ���ʱ����Ϣ
#define WM_PLAY_SELECTED_ITEM (WM_USER+200)		//����Ƶ�ļ���ʱ����Ϣ

struct MidiInfo
{
	int midi_position;
	int midi_length;
	int speed;		//�ٶȣ�bpm
	int tempo;		//ÿ���ķ�������΢����
	float ppqn;
};

class CPlayer
{
public:
	//�������ʼ�������б�����Ϣ�Ľṹ��
	struct ThreadInfo
	{
		bool refresh_info;
		bool sort;		//ָʾ�����겥���б���Ƿ�Ҫ����
		int process_percent;
		CPlayer* player;
	};
	//��ʼ�������б�Ĺ����̺߳���
	static UINT IniPlaylistThreadFunc(LPVOID lpParam);
	ThreadInfo m_thread_info;
	static CBASSMidiLibrary m_bass_midi_lib;

	//��ȡMidi������Ƕ��ʵĻص�����
	static void CALLBACK MidiLyricSync(HSYNC handle, DWORD channel, DWORD data, void *user);
	static void CALLBACK MidiEndSync(HSYNC handle, DWORD channel, DWORD data, void *user);

private:
	CWinThread* m_pThread{};		//��ʼ�������б���߳�

	HSTREAM m_musicStream{};		//��ǰ����Ƶ���
	vector<HPLUGIN> m_plugin_handles;		//����ľ��
	BASS_CHANNELINFO m_channel_info;	//��Ƶͨ������Ϣ

	BASS_MIDI_FONT m_sfont{};	//MIDI��ɫ��
	wstring m_sfont_name;		//MIDI��ɫ�������
	MidiInfo m_midi_info;
	bool m_is_midi;
	wstring m_midi_lyric;
	bool m_midi_no_lyric;

	vector<SongInfo> m_playlist;		//�����б�����ÿ�������ļ��ĸ�����Ϣ
	wstring m_path;		//��ǰ�����ļ���·��
	//wstring m_current_file_name;		//���ڲ��ŵ��ļ���
	wstring m_current_file_name_tmp;	//�򿪵�����Ƶʱ������ʱ�����ļ���
	wstring m_current_file_type;
	deque<PathInfo> m_recent_path;		//����򿪹���·��

	Time m_song_length;		//���ڲ��ŵ��ļ��ĳ���
	Time m_current_position;		//��ǰ���ŵ���λ��
	int m_song_length_int;		//���ڲ��ŵ��ļ��ĳ��ȣ�int���ͣ�
	int m_current_position_int;		//��ǰ���ŵ���λ�ã�int���ͣ�
	int m_total_time;		//�����б���������Ŀ��ʱ�䣨���룩

	int m_index{ 0 };	//��ǰ�������ֵ����
	int m_index_tmp{ 0 };
	//int m_song_num{ 0 };	//�����б��еĸ�������
	int m_error_code{ 0 };	//����������
	int m_playing{ 0 };		//���ڲ��ű�־��0����ֹͣ��1������ͣ��2�����ڲ��ţ�
	RepeatMode m_repeat_mode;		//ѭ��ģʽ��0��˳�򲥷ţ�1��������ţ�2���б�ѭ����3������ѭ����
	int m_volume{ 100 };		//�������ٷֱȣ�

	float m_fft[FFT_SAMPLE];		//����Ƶ�׷���������
	float m_spectral_data[SPECTRUM_ROW]{};	//������ʾ��ÿ��Ƶ�����εĸ߶�
	float m_last_spectral_data[SPECTRUM_ROW]{};	//��һ�ε�Ƶ������
	float m_spectral_peak[SPECTRUM_ROW]{};		//Ƶ�׶��˵ĸ߶�

	int m_equ_handle[EQU_CH_NUM]{};		//������ͨ���ľ��
	const float FREQ_TABLE[EQU_CH_NUM]{ 80, 125, 250, 500, 1000, 1500, 2000, 4000, 8000, 1600};		//ÿ��������ͨ��������Ƶ��
	int m_equalizer_gain[EQU_CH_NUM]{};		//���ڱ������úõ�ÿ��ͨ��������
	bool m_equ_enable{ false };		//ָʾ�Ƿ����������

	int m_reverb_handle{};		//����ľ��
	int m_equ_style{};
	int m_reverb_mix{};		//����ǿ�ȣ�0~100��
	int m_reverb_time{1};	//����ʱ�䣨��λ10ms��1~300��
	bool m_reverb_enable{ false };		//ָʾ�Ƿ��������

	CImage m_album_cover;			//ר������
	CImage m_album_cover_blur;		//��˹ģ�����ר������
	wstring m_album_cover_path;		//ר�������ļ���·��
	int m_album_cover_type;			//ר������ĸ�ʽ
	bool m_inner_cover{ false };			//���ר����������ǶͼƬ����Ϊtrue

	//bool m_no_ape_plugin{ false };		//����޷�����bass_ape.dll����Ϊtrue

	SongInfo m_no_use{ false };
public:
	CLyrics m_Lyrics;		//���
	//wstring m_lyric_path;	//����ļ��е�·��

	//bool lyric_karaoke_disp{ true };	//�����Ƿ��Կ���OK��ʽ��ʾ
	//bool lyric_fuzzy_match{ true };	//���ģ��ƥ��

	SortMode m_sort_mode;		//����ʽ

	bool m_loading{ false };		//����������벥���б���Ϊtrue

private:
	vector<wstring> m_current_path_lyrics;	//���浱ǰ·���µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������
	vector<wstring> m_lyric_path_lyrics;		//�������ļ����µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������

	vector<int> m_shuffle_list;			//����������Ź�����Ŀ���

private:
	void IniBASS();			//��ʼ��BASS��Ƶ��
	void UnInitBASS();
	void IniPlayList(bool cmd_para = false, bool refresh_info = false);	//��ʼ�������б�(�������cmd_paraΪtrue����ʾ��������ֱ�ӻ�ȡ�����ļ��������Ǵ�ָ��·����������
																		//���refresh_infoΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ��������Ϣ�������ļ����»�ȡ��Ϣ)
	void IniPlaylistComplate(bool sort);		//�����б�������ʱ�Ĵ���

	void ChangePath(const wstring& path, int track = 0);		//�ı䵱ǰ·��

	void SetFXHandle();		//������Ч���
	void RemoveFXHandle();		//�Ƴ���Ч���
	void ApplyEqualizer(int channel, int gain);		//Ӧ��һ��������ͨ��������

	void LoadRecentPath();		//���ļ��������·���б�
public:
	void EmplaceCurrentPathToRecent();		//����ǰ·�����뵽���·����
	void SaveRecentPath() const;		//�����·���б��浽�ļ�
	void OnExit();		//�˳�ʱ�Ĵ���

	void SetEqualizer(int channel, int gain);		//���þ�������channelΪͨ����ȡֵΪ0~9��gainΪ���棬ȡֵΪ-15~15��
	int GeEqualizer(int channel);		//��ȡָ��������ͨ��������
	void SetAllEqualizer();			//������õ�ÿ��ͨ�������棨m_equalizer_gain�����õ�������
	void ClearAllEqulizer();		//��ÿ��������ͨ�������渴λ
	void EnableEqualizer(bool enable);			//����������
	bool GetEqualizerEnable() const { return m_equ_enable; }

	void SetReverb(int mix, int time);		//���û��죨mixΪ����ǿ�ȣ�ȡֵΪ0~100��timeΪ����ʱ�䣬ȡֵΪ1~300����λΪ10ms��
	void ClearReverb();				//�رջ���
	int GetReverbMix() const { return m_reverb_mix; }
	int GetReverbTime() const { return m_reverb_time; }
	void EnableReverb(bool enable);		//���쿪��
	bool GetReverbEnable() const { return m_reverb_enable; }

private:
	CPlayer();

private:
	static CPlayer m_instance;		//CPlayer��Ψһ�Ķ���
	 
public:
	static CPlayer& GetInstance();		//��ȡCPlayer���Ψһ�Ķ���
	~CPlayer();
	void Create();		//��ʼ��CPlayer��
	void Create(const vector<wstring>& files);	//��ʼ��CPlayer��
	void Create(const wstring & path);
	void MusicControl(Command command, int volume_step = 2);		//�������ֲ���
	bool SongIsOver() const;			//�жϵ�ǰ�����Ƿ񲥷����
	void GetBASSCurrentPosition();		//��BASS��Ƶ���ȡ��ǰ���ŵ���λ��
	static int GetBASSCurrentPosition(HSTREAM hStream);
	void GetBASSSongLength();			//��BASS��Ƶ���ȡ���ڲ����ļ��ĳ���
	static Time GetBASSSongLength(HSTREAM hStream);

	void GetBASSSpectral();		//Ƶ�׷���
	int GetCurrentSecond();		//��ȡ��ǰ���ŵ���λ�õ�����
	bool IsPlaying() const;			//�жϵ�ǰ�Ƿ����ڲ���

	bool PlayTrack(int song_track);		//����ָ����ŵĸ���

	void SetVolume();		//��m_volume��ֵ��������
	void SetPath(const wstring& path, int track, int position, SortMode sort_mode);		//����·��
	void OpenFolder(wstring path);	//ͨ�������ļ��С�������·���Ĵ���
	void OpenFiles(const vector<wstring>& files, bool play = true);	//�򿪶���ļ���play���������Ƿ���������
	void OpenAFile(wstring file);	//��һ����Ƶ�ļ�������Ϊ�ļ��ľ���·��
	void SetRepeatMode();		//����ѭ��ģʽ
	void SetRepeatMode(RepeatMode repeat_mode);	//����ѭ��ģʽ
	RepeatMode GetRepeatMode() const;

	bool GetBASSError();		//��ȡBASS��Ƶ��Ĵ���
	bool IsError() const;				//�д���ʱ����ture�����򷵻�false

	void SetTitle() const;		//���ô��ڱ���

	void SaveConfig() const;		//�������õ�ini�ļ�
	void LoadConfig();		//��ini�ļ���ȡ����

	void SearchLyrics(/*bool refresh = false*/);		//��������ļ�(������refreshΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ�������ļ�·���������ļ����¼������)
	void IniLyrics();		//��ʼ�����
	void IniLyrics(const wstring& lyric_path);

	void ExplorePath(int track = -1) const;	//����Դ�������򿪵�ǰ·����ѡ��ָ��ָ���ļ�����trackС��0ʱѡ�е�ǰ���ڲ��ŵ��ļ���
	void ExplorePath(const std::wstring& path);
	void ExploreLyric() const;		//����Դ�������򿪸���ļ����ڵ�λ��

	vector<SongInfo>& GetPlayList() { return m_playlist; }	//��ȡ�����б������
	Time GetAllSongLength(int track) const;				//��ȡָ����ŵĸ����ĳ���
	int GetSongNum() const;			//��ȡ��������
	const wstring& GetCurrentDir() const { return m_path; }	//��ȡ��ǰĿ¼
	int m_current_page_idx{ 1 };
	int& GetCurrentPageIndex() { return m_current_page_idx; }

	std::wstring GetAppPath();
	std::wstring GetLyricPath();
	std::wstring GetSongPath();
	std::wstring GetAlbumPath();

	wstring GetCurrentFilePath() const { return m_path + GetCurrentFileName(); }		//��ȡ���ڲ����ļ���·��
	int GetIndex() const { return m_index; }		//��ȡ��ǰ���ŵ���Ŀ���
	wstring GetFileName() const;
	wstring GetLyricName() const { return m_Lyrics.GetPathName(); }
	int GetVolume() const { return m_volume; }
	CImage& GetAlbumCover() { return m_album_cover; }
	CImage& GetAlbumCoverBlur() { return m_album_cover_blur; }
	bool AlbumCoverExist() { return !m_album_cover.IsNull(); }
	wstring GetAlbumCoverPath() const { return m_album_cover_path; }
	int GetAlbumCoverType() const { return m_album_cover_type; }
	void DeleteAlbumCover();
	
	void ReloadPlaylist();		//�������벥���б�

	void RemoveSong(int index);		//�Ӳ����б���ɾ��ָ������Ŀ
	void RemoveSongs(vector<int> indexes);	//�Ӳ����б���ɾ�����ָ������Ŀ
	void ClearPlaylist();			//��ղ����б�
	void SeekTo(int position);		//��λ��ָ��λ��
	void SeekTo(double position);	//��λ��ָ��λ��(��Χ0~1)
	static void SeekTo(HSTREAM hStream, int position);
	void ClearLyric();		//�����ǰ�ļ��ĸ�ʹ���

	int GetCurrentPosition() const { return m_current_position_int; }		//���ص�ǰ���ŵ���λ��
	int GetSongLength() const { return m_song_length_int; }				//�������ڲ����ļ��ĳ���
	wstring GetTimeString() const;				//���ص�ǰ����ʱ����ַ�����ʽ
	const float* GetSpectralData() const { return m_spectral_data; }	//����Ƶ�׷���ÿ�����εĸ߶ȵ�����
	const float* GetSpectralPeakData() const { return m_spectral_peak; }
	const float* GetFFTData() const { return m_fft; }			//����Ƶ�׷�����ԭʼ����
	deque<PathInfo>& GetRecentPath() { return m_recent_path; }	//�����������·���б������
	wstring GetPlayingState() const;		//��ȡ����״̬���ַ���
	int GetPlayingState2() const { return m_playing; }	//��ȡ���ڲ���״̬��0����ֹͣ��1������ͣ��2�����ڲ��ţ�
	const SongInfo& GetCurrentSongInfo() const;
	void SetRelatedSongID(wstring song_id);		//Ϊ��ǰ�������ù��������������ָ���ID
	void SetRelatedSongID(int index, wstring song_id);		//Ϊ��index�׸������ù��������������ָ���ID
	bool IsInnerCover() const { return m_inner_cover; }		//�жϵ�ǰר�������Ƿ�����ǶͼƬ

	void AddListenTime(int sec);		//Ϊ��ǰ���������ۼ��Ѳ���ʱ��

	bool IsMidi() const { return m_is_midi; }
	const MidiInfo& GetMidiInfo() const { return m_midi_info; }
	const wstring& GetMidiLyric() const { return m_midi_lyric; }
	bool MidiNoLyric() const { return m_midi_no_lyric; }
	const wstring& GetSoundFontName() const { return m_sfont_name; }
	const BASS_MIDI_FONT& GetSoundFont() const { return m_sfont; }

	const BASS_CHANNELINFO& GetChannelInfo() const { return m_channel_info; }

	void ReIniBASS(bool replay = false);		//���³�ʼ��BASS����replayΪtrueʱ�����ԭ�����ڲ��ţ������³�ʼ�����������

	void SortPlaylist(bool change_index = true);	//�����б���m_sort_mode���򣨵�change_indexΪtrueʱ����������²������ڲ��ŵĸ�����

private:
	void ConnotPlayWarning() const;		//���޷�����ʱ������ʾ��Ϣ
	void SearchAlbumCover();		//��ȡר������
	void GetMidiPosition();			//��ȡMIDI���ֵĲ��Ž���
	wstring GetCurrentFileName() const;
	wstring GetCurrentPlayURL() const;

public:
	static void AcquireSongInfo(HSTREAM hStream, const wstring& file_path, SongInfo& song_info);		//��ȡ������ǩ����Ϣ
	void SearchOutAlbumCover();		//����ƥ����ⲿר�����棬������ר������
	void AlbumCoverGaussBlur();		//ר�������˹ģ��
	static wstring GetRelatedAlbumCover(const wstring& file_path, const SongInfo& song_info);		//��ȡ�������ⲿר������ͼƬ�������ļ�·��
	wstring GetCurrentFileType() { return m_current_file_type; }
};

