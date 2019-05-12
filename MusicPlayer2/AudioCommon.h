//�������ڶ�����Ƶ��Ϣ��ص�ȫ�ֺ���
#pragma once
#include "Time.h"
#include "Common.h"
#include "FilePathHelper.h"
#include "Resource.h"
#include "SongInfo.h"

//��Ƶ�ļ�����
enum AudioType
{
	AU_MP3,
	AU_WMA,
	AU_OGG,
	AU_MP4,
	AU_APE,
	AU_FLAC,
	AU_CUE,
	AU_MIDI,
	AU_OTHER
};

//����ʽ
enum SortMode
{
	SM_FILE,
	SM_TITLE,
	SM_ARTIST,
	SM_ALBUM,
	SM_TRACK
};


// ID3v1 tag structure
struct TAG_ID3V1
{
	char id[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	BYTE track[2];
	BYTE genre;
};

//����·����Ϣ
struct PathInfo
{
	wstring path;		//·��
	int track{};			//��󲥷ŵ�����Ŀ��
	int position{};		//��󲥷ŵ���λ��
	SortMode sort_mode{};	//·�����ļ�������ʽ
	int track_num{};		//·������Ƶ�ļ�������
	int total_time{};		//·������Ƶ�ļ�����ʱ��

	//PathInfo(wstring _path, int _track, int _position, SortMode _sort_mode) :
	//	path{ _path }, track{ _track }, position{ _position }, sort_mode{ _sort_mode }
	//{}
};

//ѭ��ģʽ
enum RepeatMode
{
	RM_PLAY_ORDER,		//˳�򲥷�
	RM_PLAY_SHUFFLE,	//�������
	RM_LOOP_PLAYLIST,	//�б�ѭ��
	RM_LOOP_TRACK		//����ѭ��
};

#define GENRE_MAX 148
//�����б�
const wchar_t GENRE_TABLE[GENRE_MAX][24]
{
	L"Blues",
	L"ClassicRock",
	L"Country",
	L"Dance",
	L"Disco",
	L"Funk",
	L"Grunge",
	L"Hip-Hop",
	L"Jazz",
	L"Metal",
	L"NewAge",
	L"Oldies",
	L"Other",
	L"Pop",
	L"R&B",
	L"Rap",
	L"Reggae",
	L"Rock",
	L"Techno",
	L"Industrial",
	L"Alternative",
	L"Ska",
	L"DeathMetal",
	L"Pranks",
	L"Soundtrack",
	L"Euro-Techno",
	L"Ambient",
	L"Trip-Hop",
	L"Vocal",
	L"Jazz+Funk",
	L"Fusion",
	L"Trance",
	L"Classical",
	L"Instrumental",
	L"Acid",
	L"House",
	L"Game",
	L"SoundClip",
	L"Gospel",
	L"Noise",
	L"AlternRock",
	L"Bass",
	L"Soul",
	L"Punk",
	L"Space",
	L"Meditative",
	L"InstrumentalPop",
	L"InstrumentalRock",
	L"Ethnic",
	L"Gothic",
	L"Darkwave",
	L"Techno-Industrial",
	L"Electronic",
	L"Pop-Folk",
	L"Eurodance",
	L"Dream",
	L"SouthernRock",
	L"Comedy",
	L"Cult",
	L"Gangsta",
	L"Top40",
	L"ChristianRap",
	L"Pop/Funk",
	L"Jungle",
	L"NativeAmerican",
	L"Cabaret",
	L"NewWave",
	L"Psychadelic",
	L"Rave",
	L"Showtunes",
	L"Trailer",
	L"Lo-Fi",
	L"Tribal",
	L"AcidPunk",
	L"AcidJazz",
	L"Polka",
	L"Retro",
	L"Musical",
	L"Rock&Roll",
	L"HardRock",
	L"Folk",
	L"Folk-Rock",
	L"NationalFolk",
	L"Swing",
	L"FastFusion",
	L"Bebob",
	L"Latin",
	L"Revival",
	L"Celtic",
	L"Bluegrass",
	L"Avantgarde",
	L"GothicRock",
	L"ProgessiveRock",
	L"PsychedelicRock",
	L"SymphonicRock",
	L"SlowRock",
	L"BigBand",
	L"Chorus",
	L"EasyListening",
	L"Acoustic",
	L"Humour",
	L"Speech",
	L"Chanson",
	L"Opera",
	L"ChamberMusic",
	L"Sonata",
	L"Symphony",
	L"BootyBass",
	L"Primus",
	L"PornGroove",
	L"Satire",
	L"SlowJam",
	L"Club",
	L"Tango",
	L"Samba",
	L"Folklore",
	L"Ballad",
	L"PowerBallad",
	L"RhythmicSoul",
	L"Freestyle",
	L"Duet",
	L"PunkRock",
	L"DrumSolo",
	L"Acapella",
	L"Euro-House",
	L"DanceHall",
	L"Goa",
	L"Drum&Bass",
	L"Club-House",
	L"Hardcore",
	L"Terror",
	L"Indie",
	L"BritPop",
	L"Negerpunk",
	L"PolskPunk",
	L"Beat",
	L"ChristianGangstaRap",
	L"HeavyMetal",
	L"BlackMetal",
	L"Crossover",
	L"ContemporaryChristian",
	L"ChristianRock",
	L"Merengue",
	L"Salsa",
	L"TrashMetal",
	L"Anime",
	L"JPop",
	L"Synthpop",
};

struct SupportedFormat		//һ��֧�ֵ���Ƶ�ļ���ʽ
{
	wstring file_name;
	wstring description;		//�ļ���ʽ������
	wstring extensions_list;
	vector<wstring> extensions;		//�ļ���ʽ����չ��
};

class CAudioCommon
{
public:
	CAudioCommon();
	~CAudioCommon();

	//�ж��ļ������Ƿ�Ϊ��Ƶ�ļ�
	static bool FileIsAudio(const wstring& file_name);

	//�����ļ����ж��ļ�������
	static AudioType GetAudioTypeByExtension(const wstring& file_name);

	//����һ���ļ���չ���ж���Ƶ���͵�����
	static wstring GetAudioDescriptionByExtension(wstring extension);

	//����pathĿ¼�µ�������Ƶ�ļ��������ļ������浽files�����У����޶�����ļ���Ϊmax_file
	static void GetAudioFiles(wstring path, vector<SongInfo>& files, size_t max_file);

	//����pathĿ¼�µ����и���ļ��������ļ������浽files������
	static void GetLyricFiles(wstring path, vector<wstring>& files);

	//����files�����е�cue�ļ�������ÿ�ηֹ���Ϊһ����Ŀ��ӵ�files�����У�pathΪ�ļ���·��
	static void GetCueTracks(vector<SongInfo>& files, wstring path);

	//���files�������Ƿ��к�cue�ļ�ƥ�����Ƶ�ļ������û�У������´�·���в���ƥ�����Ƶ�ļ�
	static void CheckCueFiles(vector<SongInfo>& files, wstring path);

	//��ñ�׼������Ϣ
	static wstring GetGenre(BYTE genre);

	//����ǩ�����ֱ�ʾ��������Ϣת���ɱ�׼������Ϣ
	static wstring GenreConvert(wstring genre);

	//ɾ��һ���ַ����зǴ�ӡ�ַ���ʼ�ĺ���ȫ���ַ�
	static void TagStrNormalize(wstring& str);

	//��ȡһ��BASSͨ�����͵�����
	static wstring GetBASSChannelDescription(DWORD ctype);

	//����BASSͨ�����ͻ�ȡ��Ƶ����
	static AudioType GetAudioTypeByBassChannel(DWORD ctype);

	//���������ת��������
	static CString TrackToString(BYTE track);

	//���������Ϣ�е�<>�ڵ�Ĭ���ַ���
	static void ClearDefaultTagStr(SongInfo& song_info);

public:
	static vector<SupportedFormat> m_surpported_format;		//֧�ֵ��ļ���ʽ
	static vector<wstring> m_all_surpported_extensions;		//ȫ��֧�ֵ��ļ���ʽ��չ��

protected:

};

