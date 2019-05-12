//��װ��BASS MIDI�е�����API������ͨ����̬���صķ�ʽ
#pragma once
typedef DWORD HSOUNDFONT;	// soundfont handle
struct BASS_MIDI_FONT 
{
	HSOUNDFONT font;	// soundfont
	int preset;			// preset number (-1=all)
	int bank;
};

struct BASS_MIDI_FONTINFO
{
	const char *name;
	const char *copyright;
	const char *comment;
	DWORD presets;		// number of presets/instruments
	DWORD samsize;		// total size (in bytes) of the sample data
	DWORD samload;		// amount of sample data currently loaded
	DWORD samtype;		// sample format (CTYPE) if packed
};

struct BASS_MIDI_MARK
{
	DWORD track;		// track containing marker
	DWORD pos;			// marker position
	const char *text;	// marker text
};

#define BASS_POS_MIDI_TICK		2		// tick position

#define MIDI_EVENT_TEMPO		62

// Marker types
#define BASS_MIDI_MARK_LYRIC	2	// lyric
#define BASS_MIDI_MARK_TEXT		3	// text
#define BASS_MIDI_MARK_TRACK	7	// track name

#define BASS_SYNC_MIDI_MARK		0x10000

#define BASS_ATTRIB_MIDI_PPQN		0x12000

class CBASSMidiLibrary
{
typedef HSOUNDFONT (WINAPI *_BASS_MIDI_FontInit)(const void* file, DWORD flags);
typedef BOOL (WINAPI *_BASS_MIDI_StreamSetFonts)(HSTREAM handle, const void* fonts, DWORD count);
typedef BOOL (WINAPI *_BASS_MIDI_FontGetInfo)(HSOUNDFONT handle, BASS_MIDI_FONTINFO *info );
typedef BOOL (WINAPI *_BASS_MIDI_FontFree)(HSOUNDFONT handle);
typedef DWORD (WINAPI *_BASS_MIDI_StreamGetEvent)(HSTREAM handle, DWORD chan, DWORD event);
typedef BOOL(WINAPI *_BASS_MIDI_StreamGetMark)(HSTREAM handle, DWORD type, DWORD index, BASS_MIDI_MARK *mark);
public:
	CBASSMidiLibrary();
	~CBASSMidiLibrary();

	void Init(const wstring& dll_path);		//����DLL�ļ�����ȡ�������
	void UnInit();
	bool IsSuccessed() const;		//�ж�DLL�еĺ����Ƿ��ȡ�ɹ�

	//BASS MIDI���еĺ���ָ��
	_BASS_MIDI_FontInit BASS_MIDI_FontInit;
	_BASS_MIDI_StreamSetFonts BASS_MIDI_StreamSetFonts;
	_BASS_MIDI_FontGetInfo BASS_MIDI_FontGetInfo;
	_BASS_MIDI_FontFree BASS_MIDI_FontFree;
	_BASS_MIDI_StreamGetEvent BASS_MIDI_StreamGetEvent;
	_BASS_MIDI_StreamGetMark BASS_MIDI_StreamGetMark;
private:
	HMODULE m_dll_module;
	bool m_successed{ false };
};

