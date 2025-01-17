#pragma once
#include "stdafx.h"
#include "ColorConvert.h"
#include "DrawCommon.h"
#include "Common.h"
#include "resource.h"

namespace CONSTVAL
{
	const COLORREF BACKGROUND_COLOR = GRAY(255);		//更改此颜色的值可以修改主窗口背景色
	const double FULL_SCREEN_ZOOM_FACTOR = 1.5;
}

struct DeviceInfo	//播放设备的信息
{
	int index;		//设备的索引
	wstring name;	//设备的名称
	wstring driver;	//设备的驱动程序
	DWORD flags;	//设备的状态
};


struct FontStyle
{
	bool bold{ false };			//粗体
	bool italic{ false };		//斜体
	bool underline{ false };	//下划线
	bool strike_out{ false };	//删除线

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
	wstring name;	//字体名称
	int size;		//字体大小
	FontStyle style;	//字体样式
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

	//将字号转成LOGFONT结构中的lfHeight
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
	UIFont normal;				//普通的字体
	UIFont time;				//显示播放时间
	UIFont title;				//界面2的歌曲标题

	UIFont lyric;				//歌词字体
	UIFont lyric_translate;		//歌词翻译的字体
	UIFont cortana;				//搜索框字体
	UIFont cortana_translate;	//搜索框翻译字体

	void Init()
	{
		normal.SetFont(9, CCommon::LoadText(IDS_DEFAULT_FONT));
		time.SetFont(8, CCommon::LoadText(IDS_DEFAULT_FONT));
		title.SetFont(10, CCommon::LoadText(IDS_DEFAULT_FONT));
	}
};


//选项设置数据
struct LyricSettingData
{
	bool lyric_karaoke_disp{ true };			//可以是否以卡拉OK样式显示
	bool lyric_fuzzy_match{ true };				//歌词模糊匹配
	bool save_lyric_in_offset{};				//是否将歌词保存在offset标签中，还是保存在每个时间标签中
	wstring lyric_path;							//歌词文件夹的路径
	bool show_lyric_in_cortana{};				//是否在Cortana的搜索框中显示歌词
	bool cortana_lyric_double_line{ true };		//是否在Cortana搜索中以双行显示歌词
	int cortana_color{ 0 };						//Cortana搜索框的背景颜色（0：跟随系统，1：黑色，2：白色）
	bool cortana_show_album_cover{ true };		//是否在Cortana搜索框显示专辑封面
	bool cortana_icon_beat{ true };				//Cortana图标随音乐节奏跳动
	bool cortana_lyric_compatible_mode{ false };	//Cortana搜索框歌词显示使用兼容模式
	FontInfo cortana_font;						//搜索框字体
	bool cortana_lyric_keep_display{ false };	//搜索框歌词是否在暂停时保持显示
	bool cortana_show_spectrum{ false };		//是否在搜索框显示频谱
};

struct ApperanceSettingData
{
	FontInfo lyric_font;						//歌词字体
	int lyric_line_space{ 2 };					//歌词的行间距
	int window_transparency{ 100 };				//窗口透明度
	ColorTable theme_color;						//主题颜色
	bool theme_color_follow_system{ true };		//主题颜色跟随系统（仅Win8以上支持）
	bool show_album_cover;						//显示专辑封面
	CDrawCommon::StretchMode album_cover_fit{ CDrawCommon::StretchMode::FILL };		//专辑封面契合度（拉伸模式）
	bool album_cover_as_background{ false };	//将专辑封面作为背景
	bool show_spectrum{ true };					//显示频谱分析
	int sprctrum_height{ 100 };					//频谱分析高度比例（%）
	int background_transparency{ 80 };			//背景的透明度
	bool use_out_image{ true };					//使用外部图片作为专辑封面
	bool use_inner_image_first{ true };			//优先使用内嵌专辑封面
	vector<wstring> default_album_name;			//默认的专辑封面文件名
	bool background_gauss_blur{ true };			//背景高斯模糊
	int gauss_blur_radius{ 60 };				//高斯模糊半径*10
	bool lyric_background{ true };				//歌词界面背景
	bool dark_mode{ false };					//深色模式
};

struct GeneralSettingData
{
	bool id3v2_first{ false };					//优先获取ID3V2标签
	bool auto_download_lyric{ false };			//是否自动下载歌词
	bool auto_download_album_cover{ true };		//是否自动下载专辑封面
	bool auto_download_only_tag_full{ true };	//仅在歌曲信息完整时自动下载
	bool check_update_when_start{ true };		//是否在程序启动时检查更新
	wstring sf2_path;							//MIDI音色库路径
	bool midi_use_inner_lyric{ false };			//播放MIDI音乐时显示内嵌歌词
	bool minimize_to_notify_icon{ false };		//是否最小到通知区图标

	Language language;
};

struct PlaySettingData
{
	bool stop_when_error{ true };				//出现错误时停止播放
	bool auto_play_when_start{ false };			//程序启动时自动播放
	bool show_taskbar_progress{ false };		//在任务栏按钮上显示播放进度
	bool show_playstate_icon{ true };			//在任务栏按钮上显示播放状态的角标
	wstring output_device;						//播放设备的名称
	int device_selected{};
};

struct GlobalHotKeySettingData
{
	bool hot_key_enable = true;
	bool global_multimedia_key_enable{ true };	//是否在全局范围内启用多媒体键
};

struct NonCategorizedSettingData
{
	int volum_step{ 3 };			//点击主界面中的音量调节时一次调整的步长
	int mouse_volum_step{ 2 };		//通过鼠标滚轮调节音量时的步长
	int volume_map{ 100 };			//音量映射（例如：如果将此值从100改为60，则当音量设置为最大（100%）时的音量大小为原来的60%）
	bool show_cover_tip{ true };	//是否显示专辑封面上的鼠标提示
	//wstring default_back_image_path{};	//没有专辑封面时的默认背景的路径
	bool no_sf2_warning{ true };	//是否在没有MIDI音色库时弹出提示信息
	bool show_hide_menu_bar_tip{ true };	//是隐藏菜单栏是否弹出提示信息
	bool always_on_top{ false };	//是否总是置顶
	bool cortana_opaque{ false };	//搜索框不透明
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


//播放列表中项目的显示格式
enum DisplayFormat
{
	DF_FILE_NAME,		//文件名
	DF_TITLE,			//标题
	DF_ARTIST_TITLE,	//艺术家 - 标题
	DF_TITLE_ARTIST		//标题 - 艺术家
};

//视图菜单中的一些选项
struct UIData
{
	bool show_translate{ true };		//歌词是否显示翻译
	bool narrow_mode;					//窄界面模式
	bool show_playlist{ true };
	bool show_menu_bar{ true };
	bool full_screen{ false };
	bool float_playlist{ false };		//浮动播放列表

	int client_width;					//窗口客户区宽度
	int client_height;					//窗口客户区高度
	CImage default_background;			//默认的背景
	DisplayFormat display_format{};		//播放列表中项目的显示样式
};

