#pragma once
#include "stdafx.h"
#include "ColorConvert.h"
#include "DrawCommon.h"
#include "Common.h"
#include "resource.h"

namespace CONSTVAL
{
	const COLORREF BACKGROUND_COLOR = GRAY(255);		//���Ĵ���ɫ��ֵ�����޸������ڱ���ɫ
	const double FULL_SCREEN_ZOOM_FACTOR = 1.5;
}

struct DeviceInfo	//�����豸����Ϣ
{
	int index;		//�豸������
	wstring name;	//�豸������
	wstring driver;	//�豸����������
	DWORD flags;	//�豸��״̬
};


struct FontStyle
{
	bool bold{ false };			//����
	bool italic{ false };		//б��
	bool underline{ false };	//�»���
	bool strike_out{ false };	//ɾ����

	int ToInt()
	{
		int value = 0;
		if (bold)
			value |= 0x01;
		if (italic)
			value |= 0x02;
		if (underline)
			value |= 0x04;
		if (strike_out)
			value |= 0x08;
		return value;
	}

	void FromInt(int value)
	{
		bold = (value % 2 != 0);
		italic = ((value >> 1) % 2 != 0);
		underline = ((value >> 2) % 2 != 0);
		strike_out = ((value >> 3) % 2 != 0);
	}
};

struct FontInfo
{
	wstring name;	//��������
	int size;		//�����С
	FontStyle style;	//������ʽ
};


struct UIFont
{
private:
	CFont font;
	CFont font_l;

public:
	CFont& GetFont(bool large = false)
	{
		return (large ? font_l : font);
	}

	void SetFont(int font_size, LPCTSTR font_name, FontStyle style = FontStyle())
	{
		if (font_size < 5)
			font_size = 5;

		if (font.m_hObject)
			font.DeleteObject();
		CreateFontSimple(font, font_size, font_name, style);

		if (font_l.m_hObject)
			font_l.DeleteObject();
		CreateFontSimple(font_l, static_cast<int>(font_size * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR), font_name, style);
	}

	void SetFont(FontInfo font_info)
	{
		SetFont(font_info.size, font_info.name.c_str(), font_info.style);
	}

	static void CreateFontSimple(CFont& font, int font_size, LPCTSTR font_name, FontStyle style = FontStyle())
	{
		font.CreateFont(
			FontSizeToLfHeight(font_size), // nHeight
			0, // nWidth
			0, // nEscapement
			0, // nOrientation
			(style.bold ? FW_BOLD : FW_NORMAL), // nWeight
			style.italic, // bItalic
			style.underline, // bUnderline
			style.strike_out, // cStrikeOut
			DEFAULT_CHARSET, // nCharSet
			OUT_DEFAULT_PRECIS, // nOutPrecision
			CLIP_DEFAULT_PRECIS, // nClipPrecision
			DEFAULT_QUALITY, // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			font_name);
	}

	//���ֺ�ת��LOGFONT�ṹ�е�lfHeight
	static int FontSizeToLfHeight(int font_size)
	{
		HDC hDC = ::GetDC(HWND_DESKTOP);
		int lfHeight = -MulDiv(font_size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		::ReleaseDC(HWND_DESKTOP, hDC);
		return lfHeight;
	}

};

struct FontSet
{
	UIFont normal;				//��ͨ������
	UIFont time;				//��ʾ����ʱ��
	UIFont title;				//����2�ĸ�������

	UIFont lyric;				//�������
	UIFont lyric_translate;		//��ʷ��������
	UIFont cortana;				//����������
	UIFont cortana_translate;	//������������

	void Init()
	{
		normal.SetFont(9, CCommon::LoadText(IDS_DEFAULT_FONT));
		time.SetFont(8, CCommon::LoadText(IDS_DEFAULT_FONT));
		title.SetFont(10, CCommon::LoadText(IDS_DEFAULT_FONT));
	}
};


//ѡ����������
struct LyricSettingData
{
	bool lyric_karaoke_disp{ true };			//�����Ƿ��Կ���OK��ʽ��ʾ
	bool lyric_fuzzy_match{ true };				//���ģ��ƥ��
	bool save_lyric_in_offset{};				//�Ƿ񽫸�ʱ�����offset��ǩ�У����Ǳ�����ÿ��ʱ���ǩ��
	wstring lyric_path;							//����ļ��е�·��
	bool show_lyric_in_cortana{};				//�Ƿ���Cortana������������ʾ���
	bool cortana_lyric_double_line{ true };		//�Ƿ���Cortana��������˫����ʾ���
	int cortana_color{ 0 };						//Cortana������ı�����ɫ��0������ϵͳ��1����ɫ��2����ɫ��
	bool cortana_show_album_cover{ true };		//�Ƿ���Cortana��������ʾר������
	bool cortana_icon_beat{ true };				//Cortanaͼ�������ֽ�������
	bool cortana_lyric_compatible_mode{ false };	//Cortana����������ʾʹ�ü���ģʽ
	FontInfo cortana_font;						//����������
	bool cortana_lyric_keep_display{ false };	//���������Ƿ�����ͣʱ������ʾ
	bool cortana_show_spectrum{ false };		//�Ƿ�����������ʾƵ��
};

struct ApperanceSettingData
{
	FontInfo lyric_font;						//�������
	int lyric_line_space{ 2 };					//��ʵ��м��
	int window_transparency{ 100 };				//����͸����
	ColorTable theme_color;						//������ɫ
	bool theme_color_follow_system{ true };		//������ɫ����ϵͳ����Win8����֧�֣�
	bool show_album_cover;						//��ʾר������
	CDrawCommon::StretchMode album_cover_fit{ CDrawCommon::StretchMode::FILL };		//ר���������϶ȣ�����ģʽ��
	bool album_cover_as_background{ false };	//��ר��������Ϊ����
	bool show_spectrum{ true };					//��ʾƵ�׷���
	int sprctrum_height{ 100 };					//Ƶ�׷����߶ȱ�����%��
	int background_transparency{ 80 };			//������͸����
	bool use_out_image{ true };					//ʹ���ⲿͼƬ��Ϊר������
	bool use_inner_image_first{ true };			//����ʹ����Ƕר������
	vector<wstring> default_album_name;			//Ĭ�ϵ�ר�������ļ���
	bool background_gauss_blur{ true };			//������˹ģ��
	int gauss_blur_radius{ 60 };				//��˹ģ���뾶*10
	bool lyric_background{ true };				//��ʽ��汳��
	bool dark_mode{ false };					//��ɫģʽ
};

struct GeneralSettingData
{
	bool id3v2_first{ false };					//���Ȼ�ȡID3V2��ǩ
	bool auto_download_lyric{ false };			//�Ƿ��Զ����ظ��
	bool auto_download_album_cover{ true };		//�Ƿ��Զ�����ר������
	bool auto_download_only_tag_full{ true };	//���ڸ�����Ϣ����ʱ�Զ�����
	bool check_update_when_start{ true };		//�Ƿ��ڳ�������ʱ������
	wstring sf2_path;							//MIDI��ɫ��·��
	bool midi_use_inner_lyric{ false };			//����MIDI����ʱ��ʾ��Ƕ���
	bool minimize_to_notify_icon{ false };		//�Ƿ���С��֪ͨ��ͼ��

	Language language;
};

struct PlaySettingData
{
	bool stop_when_error{ true };				//���ִ���ʱֹͣ����
	bool auto_play_when_start{ false };			//��������ʱ�Զ�����
	bool show_taskbar_progress{ false };		//����������ť����ʾ���Ž���
	bool show_playstate_icon{ true };			//����������ť����ʾ����״̬�ĽǱ�
	wstring output_device;						//�����豸������
	int device_selected{};
};

struct GlobalHotKeySettingData
{
	bool hot_key_enable = true;
	bool global_multimedia_key_enable{ true };	//�Ƿ���ȫ�ַ�Χ�����ö�ý���
};

struct NonCategorizedSettingData
{
	int volum_step{ 3 };			//����������е���������ʱһ�ε����Ĳ���
	int mouse_volum_step{ 2 };		//ͨ�������ֵ�������ʱ�Ĳ���
	int volume_map{ 100 };			//����ӳ�䣨���磺�������ֵ��100��Ϊ60������������Ϊ���100%��ʱ��������СΪԭ����60%��
	bool show_cover_tip{ true };	//�Ƿ���ʾר�������ϵ������ʾ
	//wstring default_back_image_path{};	//û��ר������ʱ��Ĭ�ϱ�����·��
	bool no_sf2_warning{ true };	//�Ƿ���û��MIDI��ɫ��ʱ������ʾ��Ϣ
	bool show_hide_menu_bar_tip{ true };	//�����ز˵����Ƿ񵯳���ʾ��Ϣ
	bool always_on_top{ false };	//�Ƿ������ö�
	bool cortana_opaque{ false };	//������͸��
	COLORREF cortana_transparent_color{};
};

struct IconRes
{
private:
	HICON hIcon;
	HICON hIconDark;
	HICON hIconLarge;
	HICON hIconDarkLarge;
	CSize iconSize;
	CSize iconSizeLarge;

public:
	const HICON& GetIcon(bool dark = false, bool large = false) const
	{
		if(large)
			return (dark && hIconDarkLarge != NULL ? hIconDarkLarge : hIconLarge);
		else
			return (dark && hIconDark != NULL ? hIconDark : hIcon);
	}

	void Load(UINT id, UINT id_dark, int size)
	{
		int size_large = static_cast<int>(size * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR);

		if(size < 32)
			size = CCommon::IconSizeNormalize(size);
		if (size_large < 32)
			size_large = CCommon::IconSizeNormalize(size_large);

		if (id != 0)
		{
			hIcon = CDrawCommon::LoadIconResource(id, size, size);
			hIconLarge = CDrawCommon::LoadIconResource(id, size_large, size_large);
		}
		if (id_dark != 0)
		{
			hIconDark = CDrawCommon::LoadIconResource(id_dark, size, size);
			hIconDarkLarge = CDrawCommon::LoadIconResource(id_dark, size_large, size_large);
		}
		iconSize.cx = iconSize.cy = size;
		iconSizeLarge.cx = iconSizeLarge.cy = size_large;
	}

	const CSize& GetSize(bool large = false) const
	{
		return (large? iconSizeLarge : iconSize);
	}
};

struct IconSet
{
	IconRes default_cover;
	IconRes default_cover_small;
	IconRes skin;
	IconRes eq;
	IconRes setting;
	IconRes mini;
	IconRes play_oder;
	IconRes play_shuffle;
	IconRes loop_playlist;
	IconRes loop_track;
	IconRes previous;
	IconRes play;
	IconRes pause;
	IconRes next;
	IconRes info;
	IconRes select_folder;
	IconRes show_playlist;
	IconRes find_songs;
	IconRes full_screen;
	IconRes full_screen1;
	IconRes menu;

	IconRes stop_l;
	IconRes previous_l;
	IconRes play_l;
	IconRes pause_l;
	IconRes next_l;
};


//�����б�����Ŀ����ʾ��ʽ
enum DisplayFormat
{
	DF_FILE_NAME,		//�ļ���
	DF_TITLE,			//����
	DF_ARTIST_TITLE,	//������ - ����
	DF_TITLE_ARTIST		//���� - ������
};

//��ͼ�˵��е�һЩѡ��
struct UIData
{
	bool show_translate{ true };		//����Ƿ���ʾ����
	bool narrow_mode;					//խ����ģʽ
	bool show_playlist{ true };
	bool show_menu_bar{ true };
	bool full_screen{ false };
	bool float_playlist{ false };		//���������б�

	int client_width;					//���ڿͻ������
	int client_height;					//���ڿͻ����߶�
	CImage default_background;			//Ĭ�ϵı���
	DisplayFormat display_format{};		//�����б�����Ŀ����ʾ��ʽ
};

