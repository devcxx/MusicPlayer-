
// MusicPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SupportedFormatDlg.h"
#include "AboutDlg.h"
#include "CTest.h"
#include "CListenTimeStatisticsDlg.h"
#include "CFloatPlaylistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMusicPlayerDlg �Ի���

const UINT WM_TASKBARCREATED{ ::RegisterWindowMessage(_T("TaskbarCreated")) };	//ע����������������Ϣ

CMusicPlayerDlg::CMusicPlayerDlg(wstring cmdLine, CWnd* pParent /*=NULL*/)
	: m_cmdLine{cmdLine}, CMainDialogBase(IDD_MUSICPLAYER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMusicPlayerDlg::~CMusicPlayerDlg()
{
	CCommon::DeleteModelessDialog(m_pLyricEdit);
	CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
	CCommon::DeleteModelessDialog(m_pSetPathDlg);
	CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
	CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
	CCommon::DeleteModelessDialog(m_pDownloadMgrDlg);

}

bool CMusicPlayerDlg::IsTaskbarListEnable() const
{
	return CWinVersionHelper::IsWindows7OrLater() && m_pTaskbar != nullptr;
}

void CMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CMainDialogBase::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
	DDX_Control(pDX, IDC_PLAYLIST_LIST, m_playlist_list);
	DDX_Control(pDX, IDC_PATH_STATIC, m_path_static);
	DDX_Control(pDX, IDC_PATH_EDIT, m_path_edit);
	DDX_Control(pDX, ID_SET_PATH, m_set_path_button);
	DDX_Control(pDX, IDC_SEARCH_EDIT, m_search_edit);
// 	DDX_Control(pDX, IDC_CLEAR_SEARCH_BUTTON, m_clear_search_button);
	DDX_Control(pDX, IDC_SEARCH_PREV_PAGE_BTN, m_search_prev_page_button);
	DDX_Control(pDX, IDC_SEARCH_NEXT_PAGE_BTN, m_search_next_page_button);
}

BEGIN_MESSAGE_MAP(CMusicPlayerDlg, CMainDialogBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PLAY_PAUSE, &CMusicPlayerDlg::OnPlayPause)
	ON_COMMAND(ID_STOP, &CMusicPlayerDlg::OnStop)
	ON_COMMAND(ID_PREVIOUS, &CMusicPlayerDlg::OnPrevious)
	ON_COMMAND(ID_NEXT, &CMusicPlayerDlg::OnNext)
	ON_COMMAND(ID_REW, &CMusicPlayerDlg::OnRew)
	ON_COMMAND(ID_FF, &CMusicPlayerDlg::OnFF)
	ON_COMMAND(ID_SET_PATH, &CMusicPlayerDlg::OnSetPath)
	ON_COMMAND(ID_DOWNLOAD_MGR, &CMusicPlayerDlg::OnDownMgr)
	ON_COMMAND(ID_FIND, &CMusicPlayerDlg::OnFind)
	ON_COMMAND(ID_EXPLORE_PATH, &CMusicPlayerDlg::OnExplorePath)
	ON_WM_DESTROY()
	ON_COMMAND(ID_APP_ABOUT, &CMusicPlayerDlg::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, &CMusicPlayerDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_FOLDER, &CMusicPlayerDlg::OnFileOpenFolder)
	ON_WM_DROPFILES()
	ON_WM_INITMENU()
	ON_COMMAND(ID_PLAY_ORDER, &CMusicPlayerDlg::OnPlayOrder)
	ON_COMMAND(ID_PLAY_SHUFFLE, &CMusicPlayerDlg::OnPlayShuffle)
	ON_COMMAND(ID_LOOP_PLAYLIST, &CMusicPlayerDlg::OnLoopPlaylist)
	ON_COMMAND(ID_LOOP_TRACK, &CMusicPlayerDlg::OnLoopTrack)
	ON_WM_MOUSEWHEEL()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMDblclkPlaylistList)
	ON_COMMAND(ID_OPTION_SETTINGS, &CMusicPlayerDlg::OnOptionSettings)
	ON_COMMAND(ID_RELOAD_PLAYLIST, &CMusicPlayerDlg::OnReloadPlaylist)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMRClickPlaylistList)
	ON_COMMAND(ID_PLAY_ITEM, &CMusicPlayerDlg::OnPlayItem)
	ON_COMMAND(ID_DOWNLOAD_ITEM, &CMusicPlayerDlg::OnDownloadItem)
	ON_COMMAND(ID_ITEM_PROPERTY, &CMusicPlayerDlg::OnItemProperty)
	ON_COMMAND(ID_EXPLORE_TRACK, &CMusicPlayerDlg::OnExploreTrack)
	ON_WM_HOTKEY()
	ON_COMMAND(ID_RE_INI_BASS, &CMusicPlayerDlg::OnReIniBass)
	ON_COMMAND(ID_SORT_BY_FILE, &CMusicPlayerDlg::OnSortByFile)
	ON_COMMAND(ID_SORT_BY_TITLE, &CMusicPlayerDlg::OnSortByTitle)
	ON_COMMAND(ID_SORT_BY_ARTIST, &CMusicPlayerDlg::OnSortByArtist)
	ON_COMMAND(ID_SORT_BY_ALBUM, &CMusicPlayerDlg::OnSortByAlbum)
	ON_COMMAND(ID_SORT_BY_TRACK, &CMusicPlayerDlg::OnSortByTrack)
	ON_COMMAND(ID_DELETE_FROM_DISK, &CMusicPlayerDlg::OnDeleteFromDisk)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CMusicPlayerDlg::OnTaskbarcreated)
	ON_COMMAND(ID_DISP_FILE_NAME, &CMusicPlayerDlg::OnDispFileName)
	ON_COMMAND(ID_DISP_TITLE, &CMusicPlayerDlg::OnDispTitle)
	ON_COMMAND(ID_DISP_ARTIST_TITLE, &CMusicPlayerDlg::OnDispArtistTitle)
	ON_COMMAND(ID_DISP_TITLE_ARTIST, &CMusicPlayerDlg::OnDispTitleArtist)
	ON_COMMAND(ID_MINI_MODE, &CMusicPlayerDlg::OnMiniMode)
	ON_BN_CLICKED(IDC_STOP, &CMusicPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMusicPlayerDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_PLAY_PAUSE, &CMusicPlayerDlg::OnBnClickedPlayPause)
	ON_BN_CLICKED(IDC_NEXT, &CMusicPlayerDlg::OnBnClickedNext)
	ON_COMMAND(ID_RELOAD_LYRIC, &CMusicPlayerDlg::OnReloadLyric)
	ON_COMMAND(ID_SONG_INFO, &CMusicPlayerDlg::OnSongInfo)
	ON_COMMAND(ID_COPY_CURRENT_LYRIC, &CMusicPlayerDlg::OnCopyCurrentLyric)
	ON_COMMAND(ID_COPY_ALL_LYRIC, &CMusicPlayerDlg::OnCopyAllLyric)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LYRIC_FORWARD, &CMusicPlayerDlg::OnLyricForward)
	ON_COMMAND(ID_LYRIC_DELAY, &CMusicPlayerDlg::OnLyricDelay)
	ON_COMMAND(ID_SAVE_MODIFIED_LYRIC, &CMusicPlayerDlg::OnSaveModifiedLyric)
	ON_COMMAND(ID_EDIT_LYRIC, &CMusicPlayerDlg::OnEditLyric)
	ON_COMMAND(ID_DOWNLOAD_LYRIC, &CMusicPlayerDlg::OnDownloadLyric)
	ON_COMMAND(ID_LYRIC_BATCH_DOWNLOAD, &CMusicPlayerDlg::OnLyricBatchDownload)
	ON_COMMAND(ID_DELETE_LYRIC, &CMusicPlayerDlg::OnDeleteLyric)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_PLAYLIST_INI_COMPLATE, &CMusicPlayerDlg::OnPlaylistIniComplate)
	ON_MESSAGE(WM_SET_TITLE, &CMusicPlayerDlg::OnSetTitle)
	ON_COMMAND(ID_EQUALIZER, &CMusicPlayerDlg::OnEqualizer)
	ON_COMMAND(ID_EXPLORE_ONLINE, &CMusicPlayerDlg::OnExploreOnline)
	ON_MESSAGE(WM_PLAYLIST_INI_START, &CMusicPlayerDlg::OnPlaylistIniStart)
	ON_COMMAND(ID_BROWSE_LYRIC, &CMusicPlayerDlg::OnBrowseLyric)
	ON_COMMAND(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, &CMusicPlayerDlg::OnTranslateToSimplifiedChinese)
	ON_COMMAND(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, &CMusicPlayerDlg::OnTranslateToTranditionalChinese)
	ON_COMMAND(ID_ALBUM_COVER_SAVE_AS, &CMusicPlayerDlg::OnAlbumCoverSaveAs)
	ON_MESSAGE(WM_PATH_SELECTED, &CMusicPlayerDlg::OnPathSelected)
	ON_MESSAGE(WM_CONNOT_PLAY_WARNING, &CMusicPlayerDlg::OnConnotPlayWarning)
	ON_EN_CHANGE(IDC_SEARCH_EDIT, &CMusicPlayerDlg::OnEnChangeSearchEdit)
// 	ON_BN_CLICKED(IDC_CLEAR_SEARCH_BUTTON, &CMusicPlayerDlg::OnBnClickedClearSearchButton)
	ON_BN_CLICKED(IDC_SEARCH_PREV_PAGE_BTN, &CMusicPlayerDlg::OnBnClickedSearchPrevPageButton)
	ON_BN_CLICKED(IDC_SEARCH_NEXT_PAGE_BTN, &CMusicPlayerDlg::OnBnClickedSearchNextPageButton)
	ON_COMMAND(ID_DOWNLOAD_ALBUM_COVER, &CMusicPlayerDlg::OnDownloadAlbumCover)
	ON_MESSAGE(WM_MUSIC_STREAM_OPENED, &CMusicPlayerDlg::OnMusicStreamOpened)
	ON_COMMAND(ID_CURRENT_EXPLORE_ONLINE, &CMusicPlayerDlg::OnCurrentExploreOnline)
	ON_COMMAND(ID_DELETE_ALBUM_COVER, &CMusicPlayerDlg::OnDeleteAlbumCover)
	ON_COMMAND(ID_COPY_FILE_TO, &CMusicPlayerDlg::OnCopyFileTo)
	ON_COMMAND(ID_MOVE_FILE_TO, &CMusicPlayerDlg::OnMoveFileTo)
	ON_MESSAGE(WM_OPEN_FILE_COMMAND_LINE, &CMusicPlayerDlg::OnOpenFileCommandLine)
	ON_COMMAND(ID_FORMAT_CONVERT, &CMusicPlayerDlg::OnFormatConvert)
	ON_COMMAND(ID_FORMAT_CONVERT1, &CMusicPlayerDlg::OnFormatConvert1)
	ON_MESSAGE(WM_SETTINGS_APPLIED, &CMusicPlayerDlg::OnSettingsApplied)
	ON_COMMAND(ID_RECORDER, &CMusicPlayerDlg::OnRecorder)
	ON_MESSAGE(WM_ALBUM_COVER_DOWNLOAD_COMPLETE, &CMusicPlayerDlg::OnAlbumCoverDownloadComplete)
	ON_WM_DWMCOLORIZATIONCOLORCHANGED()
	ON_COMMAND(ID_SUPPORTED_FORMAT, &CMusicPlayerDlg::OnSupportedFormat)
	ON_COMMAND(ID_SWITCH_UI, &CMusicPlayerDlg::OnSwitchUi)
	ON_COMMAND(ID_VOLUME_UP, &CMusicPlayerDlg::OnVolumeUp)
	ON_COMMAND(ID_VOLUME_DOWN, &CMusicPlayerDlg::OnVolumeDown)
	ON_MESSAGE(MY_WM_NOTIFYICON, &CMusicPlayerDlg::OnNotifyicon)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_EXIT, &CMusicPlayerDlg::OnMenuExit)
	ON_COMMAND(ID_MINIMODE_RESTORE, &CMusicPlayerDlg::OnMinimodeRestore)
	ON_WM_APPCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SHOW_PLAYLIST, &CMusicPlayerDlg::OnShowPlaylist)
	ON_WM_SETCURSOR()
	ON_WM_MOUSELEAVE()
	ON_COMMAND(ID_SHOW_MENU_BAR, &CMusicPlayerDlg::OnShowMenuBar)
	ON_COMMAND(ID_FULL_SCREEN, &CMusicPlayerDlg::OnFullScreen)
	ON_COMMAND(ID_CREATE_PLAY_SHORTCUT, &CMusicPlayerDlg::OnCreatePlayShortcut)
	ON_COMMAND(ID_LISTEN_STATISTICS, &CMusicPlayerDlg::OnListenStatistics)
	ON_COMMAND(ID_DARK_MODE, &CMusicPlayerDlg::OnDarkMode)
	ON_MESSAGE(WM_MAIN_MENU_POPEDUP, &CMusicPlayerDlg::OnMainMenuPopup)
	ON_COMMAND(ID_ALWAYS_ON_TOP, &CMusicPlayerDlg::OnAlwaysOnTop)
	ON_COMMAND(ID_FLOAT_PLAYLIST, &CMusicPlayerDlg::OnFloatPlaylist)
	ON_MESSAGE(WM_PLAY_SELECTED_ITEM, &CMusicPlayerDlg::OnPlaySelectedItem)
END_MESSAGE_MAP()


// CMusicPlayerDlg ��Ϣ�������

void CMusicPlayerDlg::SaveConfig()
{
	CIniHelper ini(theApp.m_config_path);

	ini.WriteInt(L"config", L"window_width", m_window_width);
	ini.WriteInt(L"config", L"window_hight", m_window_height);
	ini.WriteInt(L"config", L"transparency", theApp.m_app_setting_data.window_transparency);
	ini.WriteBool(L"config", L"narrow_mode", theApp.m_ui_data.narrow_mode);
	ini.WriteBool(L"config", L"show_translate", theApp.m_ui_data.show_translate);
	ini.WriteBool(L"config", L"show_playlist", theApp.m_ui_data.show_playlist);
	ini.WriteBool(L"config", L"show_menu_bar", theApp.m_ui_data.show_menu_bar);
	ini.WriteBool(L"config", L"float_playlist", theApp.m_ui_data.float_playlist);

	ini.WriteInt(L"config", L"theme_color", theApp.m_app_setting_data.theme_color.original_color);
	ini.WriteBool(L"config", L"theme_color_follow_system", theApp.m_app_setting_data.theme_color_follow_system);
	ini.WriteInt(L"config", L"playlist_display_format", static_cast<int>(theApp.m_ui_data.display_format));
	ini.WriteBool(L"config", L"show_lyric_in_cortana", theApp.m_lyric_setting_data.show_lyric_in_cortana);
	ini.WriteBool(L"config", L"save_lyric_in_offset", theApp.m_lyric_setting_data.save_lyric_in_offset);
	ini.WriteString(L"config",L"font", theApp.m_app_setting_data.lyric_font.name);
	ini.WriteInt(L"config", L"font_size", theApp.m_app_setting_data.lyric_font.size);
	ini.WriteInt(L"config", L"font_style", theApp.m_app_setting_data.lyric_font.style.ToInt());
	ini.WriteInt(L"config", L"lyric_line_space", theApp.m_app_setting_data.lyric_line_space);
	ini.WriteInt(L"config", L"spectrum_height", theApp.m_app_setting_data.sprctrum_height);
	ini.WriteBool(L"config", L"cortana_lyric_double_line", theApp.m_lyric_setting_data.cortana_lyric_double_line);
	ini.WriteBool(L"config", L"show_spectrum", theApp.m_app_setting_data.show_spectrum);
	ini.WriteBool(L"config", L"show_album_cover", theApp.m_app_setting_data.show_album_cover);
	ini.WriteInt(L"config", L"album_cover_fit", static_cast<int>(theApp.m_app_setting_data.album_cover_fit));
	ini.WriteBool(L"config", L"album_cover_as_background", theApp.m_app_setting_data.album_cover_as_background);
	ini.WriteBool(L"config", L"cortana_show_album_cover", theApp.m_lyric_setting_data.cortana_show_album_cover);
	ini.WriteBool(L"config", L"cortana_icon_beat", theApp.m_lyric_setting_data.cortana_icon_beat);
	ini.WriteBool(L"config", L"cortana_lyric_compatible_mode", theApp.m_lyric_setting_data.cortana_lyric_compatible_mode);
	ini.WriteString(L"config", L"cortana_font", theApp.m_lyric_setting_data.cortana_font.name);
	ini.WriteInt(L"config", L"cortana_font_size", theApp.m_lyric_setting_data.cortana_font.size);
	ini.WriteInt(L"config", L"cortana_font_style", theApp.m_lyric_setting_data.cortana_font.style.ToInt());
	ini.WriteBool(L"config", L"cortana_lyric_keep_display", theApp.m_lyric_setting_data.cortana_lyric_keep_display);
	ini.WriteBool(L"config", L"cortana_show_spectrum", theApp.m_lyric_setting_data.cortana_show_spectrum);

	ini.WriteBool(L"config", L"background_gauss_blur", theApp.m_app_setting_data.background_gauss_blur);
	ini.WriteInt(L"config", L"gauss_blur_radius", theApp.m_app_setting_data.gauss_blur_radius);
	ini.WriteBool(L"config", L"lyric_background", theApp.m_app_setting_data.lyric_background);
	ini.WriteBool(L"config", L"dark_mode", theApp.m_app_setting_data.dark_mode);
	ini.WriteInt(L"config", L"background_transparency", theApp.m_app_setting_data.background_transparency);
	ini.WriteBool(L"config", L"use_out_image", theApp.m_app_setting_data.use_out_image);
	ini.WriteBool(L"config", L"use_inner_image_first", theApp.m_app_setting_data.use_inner_image_first);

	ini.WriteInt(L"config", L"volum_step", theApp.m_nc_setting_data.volum_step);
	ini.WriteInt(L"config", L"mouse_volum_step", theApp.m_nc_setting_data.mouse_volum_step);
	ini.WriteInt(L"config", L"cortana_back_color", theApp.m_lyric_setting_data.cortana_color);
	ini.WriteInt(L"config", L"volume_map", theApp.m_nc_setting_data.volume_map);
	ini.WriteBool(L"config", L"show_cover_tip", theApp.m_nc_setting_data.show_cover_tip);
	ini.WriteBool(L"config", L"always_on_top", theApp.m_nc_setting_data.always_on_top);

	ini.WriteBool(L"other", L"no_sf2_warning", theApp.m_nc_setting_data.no_sf2_warning);
	ini.WriteBool(L"other", L"show_hide_menu_bar_tip", theApp.m_nc_setting_data.show_hide_menu_bar_tip);
	ini.WriteBool(L"other", L"cortana_opaque", theApp.m_nc_setting_data.cortana_opaque);
	ini.WriteInt(L"other", L"cortana_transparent_color", theApp.m_nc_setting_data.cortana_transparent_color);

	ini.WriteBool(L"general", L"id3v2_first", theApp.m_general_setting_data.id3v2_first);
	ini.WriteBool(L"general", L"auto_download_lyric", theApp.m_general_setting_data.auto_download_lyric);
	ini.WriteBool(L"general", L"auto_download_album_cover", theApp.m_general_setting_data.auto_download_album_cover);
	ini.WriteBool(L"general", L"auto_download_only_tag_full", theApp.m_general_setting_data.auto_download_only_tag_full);
	ini.WriteString(L"general", L"sf2_path", theApp.m_general_setting_data.sf2_path);
	ini.WriteBool(L"general", L"midi_use_inner_lyric", theApp.m_general_setting_data.midi_use_inner_lyric);
	ini.WriteBool(L"general", L"minimize_to_notify_icon", theApp.m_general_setting_data.minimize_to_notify_icon);

	ini.WriteBool(L"config", L"stop_when_error", theApp.m_play_setting_data.stop_when_error);
	ini.WriteBool(L"config", L"auto_play_when_start", theApp.m_play_setting_data.auto_play_when_start);
	ini.WriteBool(L"config", L"show_taskbar_progress", theApp.m_play_setting_data.show_taskbar_progress);
	ini.WriteBool(L"config", L"show_playstate_icon", theApp.m_play_setting_data.show_playstate_icon);
	ini.WriteString(L"config", L"output_device", theApp.m_play_setting_data.output_device);

	int ui_selected;
	if (m_pUI == &m_ui)
		ui_selected = 0;
	else
		ui_selected = 1;
	ini.WriteInt(L"config", L"UI_selected", ui_selected);

	//�����ȼ�����
	ini.WriteBool(L"hot_key", L"hot_key_enable", theApp.m_hot_key_setting_data.hot_key_enable);
	theApp.m_hot_key.SaveToTni(ini);

	ini.Save();
}

void CMusicPlayerDlg::LoadConfig()
{
	CIniHelper ini(theApp.m_config_path);

	m_window_width = ini.GetInt(L"config", L"window_width", theApp.DPI(698));
	m_window_height = ini.GetInt(L"config", L"window_hight", theApp.DPI(565));
	theApp.m_app_setting_data.window_transparency = ini.GetInt(L"config", L"transparency", 100);
	theApp.m_ui_data.narrow_mode = ini.GetBool(L"config", L"narrow_mode", false);
	theApp.m_ui_data.show_translate = ini.GetBool(L"config", L"show_translate", true);
	theApp.m_ui_data.show_playlist = ini.GetBool(L"config", L"show_playlist", true);
	theApp.m_ui_data.show_menu_bar = ini.GetBool(L"config", L"show_menu_bar", true);
	theApp.m_ui_data.float_playlist = ini.GetBool(L"config", L"float_playlist", false);

	theApp.m_app_setting_data.theme_color.original_color = ini.GetInt(L"config", L"theme_color", 16760187);
	theApp.m_app_setting_data.theme_color_follow_system = ini.GetBool(L"config", L"theme_color_follow_system", true);
	theApp.m_ui_data.display_format = static_cast<DisplayFormat>(ini.GetInt(L"config", L"playlist_display_format", 2));
	theApp.m_lyric_setting_data.show_lyric_in_cortana = ini.GetBool(L"config", L"show_lyric_in_cortana", false);
	theApp.m_lyric_setting_data.save_lyric_in_offset = ini.GetBool(L"config", L"save_lyric_in_offset", false);
	theApp.m_app_setting_data.lyric_font.name = ini.GetString(L"config", L"font", CCommon::LoadText(IDS_DEFAULT_FONT));
	theApp.m_app_setting_data.lyric_font.size = ini.GetInt(L"config", L"font_size", 11);
	theApp.m_app_setting_data.lyric_font.style.FromInt(ini.GetInt(L"config", L"font_style", 0));
	theApp.m_app_setting_data.lyric_line_space = ini.GetInt(L"config", L"lyric_line_space", 4);
	theApp.m_app_setting_data.sprctrum_height = ini.GetInt(L"config", L"spectrum_height", 80);
	theApp.m_lyric_setting_data.cortana_lyric_double_line = ini.GetBool(L"config", L"cortana_lyric_double_line", true);
	theApp.m_app_setting_data.show_spectrum = ini.GetBool(L"config", L"show_spectrum", 1);
	theApp.m_app_setting_data.show_album_cover = ini.GetBool(L"config", L"show_album_cover", 1);
	theApp.m_app_setting_data.album_cover_fit = static_cast<CDrawCommon::StretchMode>(ini.GetInt(L"config", L"album_cover_fit", 1));
	theApp.m_app_setting_data.album_cover_as_background = ini.GetBool(L"config", L"album_cover_as_background", true);
	theApp.m_lyric_setting_data.cortana_show_album_cover = ini.GetBool(L"config", L"cortana_show_album_cover", 1);
	theApp.m_lyric_setting_data.cortana_icon_beat = ini.GetBool(L"config", L"cortana_icon_beat", 0);
	theApp.m_lyric_setting_data.cortana_lyric_compatible_mode = ini.GetBool(L"config", L"cortana_lyric_compatible_mode", false);
	theApp.m_lyric_setting_data.cortana_font.name = ini.GetString(L"config", L"cortana_font", CCommon::LoadText(IDS_DEFAULT_FONT));
	theApp.m_lyric_setting_data.cortana_font.size = ini.GetInt(L"config", L"cortana_font_size", 11);
	theApp.m_lyric_setting_data.cortana_font.style.FromInt(ini.GetInt(L"config", L"cortana_font_style", 0));
	theApp.m_lyric_setting_data.cortana_lyric_keep_display = ini.GetBool(L"config", L"cortana_lyric_keep_display", false);
	theApp.m_lyric_setting_data.cortana_show_spectrum = ini.GetBool(L"config", L"cortana_show_spectrum", false);

	theApp.m_app_setting_data.background_gauss_blur = ini.GetBool(L"config", L"background_gauss_blur", true);
	theApp.m_app_setting_data.gauss_blur_radius = ini.GetInt(L"config", L"gauss_blur_radius", 60);
	theApp.m_app_setting_data.lyric_background = ini.GetBool(L"config", L"lyric_background", false);
	theApp.m_app_setting_data.dark_mode = ini.GetBool(L"config", L"dark_mode", false);
	theApp.m_app_setting_data.background_transparency = ini.GetInt(L"config", L"background_transparency", 80);
	theApp.m_app_setting_data.use_out_image = ini.GetBool(L"config", L"use_out_image", 0);
	theApp.m_app_setting_data.use_inner_image_first = ini.GetBool(L"config", L"use_inner_image_first", true);

	theApp.m_nc_setting_data.volum_step = ini.GetInt(L"config", L"volum_step", 3);
	theApp.m_nc_setting_data.mouse_volum_step = ini.GetInt(L"config", L"mouse_volum_step", 2);
	theApp.m_lyric_setting_data.cortana_color = ini.GetInt(L"config", L"cortana_back_color", 0);
	theApp.m_nc_setting_data.volume_map = ini.GetInt(L"config", L"volume_map", 100);
	theApp.m_nc_setting_data.show_cover_tip = ini.GetBool(L"config", L"show_cover_tip", false);
	theApp.m_nc_setting_data.always_on_top = ini.GetBool(L"config", L"always_on_top", false);

	theApp.m_nc_setting_data.no_sf2_warning = ini.GetBool(L"other", L"no_sf2_warning", true);
	theApp.m_nc_setting_data.show_hide_menu_bar_tip = ini.GetBool(L"other", L"show_hide_menu_bar_tip", true);
	theApp.m_nc_setting_data.cortana_opaque = ini.GetBool(L"other", L"cortana_opaque", false);
	theApp.m_nc_setting_data.cortana_transparent_color = ini.GetInt(L"other", L"cortana_transparent_color", RGB(255, 0, 255));

	theApp.m_general_setting_data.id3v2_first = ini.GetBool(L"general", L"id3v2_first", 1);
	theApp.m_general_setting_data.auto_download_lyric = ini.GetBool(L"general", L"auto_download_lyric", 1);
	theApp.m_general_setting_data.auto_download_album_cover = ini.GetBool(L"general", L"auto_download_album_cover", 1);
	theApp.m_general_setting_data.auto_download_only_tag_full = ini.GetBool(L"general", L"auto_download_only_tag_full", 1);
	theApp.m_general_setting_data.sf2_path = ini.GetString(L"general", L"sf2_path", L"");
	theApp.m_general_setting_data.midi_use_inner_lyric = ini.GetBool(L"general", L"midi_use_inner_lyric", 0);
	theApp.m_general_setting_data.minimize_to_notify_icon = ini.GetBool(L"general", L"minimize_to_notify_icon", false);

	theApp.m_play_setting_data.stop_when_error = ini.GetBool(L"config", L"stop_when_error", true);
	theApp.m_play_setting_data.auto_play_when_start = ini.GetBool(L"config", L"auto_play_when_start", false);
	theApp.m_play_setting_data.show_taskbar_progress = ini.GetBool(L"config", L"show_taskbar_progress", true);
	theApp.m_play_setting_data.show_playstate_icon = ini.GetBool(L"config", L"show_playstate_icon", true);
	theApp.m_play_setting_data.output_device = ini.GetString(L"config", L"output_device", L"");

	int ui_selected = ini.GetInt(L"config", L"UI_selected", 1);
	if (ui_selected == 0)
		m_pUI = &m_ui;
	else
		m_pUI = &m_ui2;

	//�����ȼ�����
	theApp.m_hot_key_setting_data.hot_key_enable = ini.GetBool(L"hot_key", L"hot_key_enable", true);
	theApp.m_hot_key.LoadFromIni(ini);

}

void CMusicPlayerDlg::SetTransparency()
{
	CCommon::SetWindowOpacity(m_hWnd, theApp.m_app_setting_data.window_transparency);
}

void CMusicPlayerDlg::DrawInfo(bool reset)
{
	if (!IsIconic() && IsWindowVisible())		//������С��������ʱ�����ƣ��Խ���CPU������
		m_pUI->DrawInfo(reset);
}

void CMusicPlayerDlg::SetPlaylistSize(int cx, int cy)
{
	//���ò����б��С
	if (!theApp.m_ui_data.narrow_mode)
	{
		m_playlist_list.MoveWindow(cx / 2 + m_layout.margin, m_layout.search_edit_height + m_layout.path_edit_height + m_layout.margin,
			cx / 2 - 2 * m_layout.margin, cy - m_layout.search_edit_height - m_layout.path_edit_height - 2 * m_layout.margin);
	}
	else
	{
		m_playlist_list.MoveWindow(m_layout.margin, m_ui.DrawAreaHeight() + m_layout.search_edit_height + m_layout.path_edit_height,
			cx - 2 * m_layout.margin, cy - m_ui.DrawAreaHeight() - m_layout.search_edit_height - m_layout.path_edit_height - m_layout.margin);
	}
	m_playlist_list.AdjustColumnWidth();

	//���á���ǰ·����static�ؼ���С
	CRect rect_static;
	m_path_static.GetWindowRect(rect_static);
	rect_static.bottom = rect_static.top + m_layout.path_edit_height - 2 * m_layout.margin;
	if (!theApp.m_ui_data.narrow_mode)
		rect_static.MoveToXY(cx / 2 + m_layout.margin, m_layout.margin);
	else
		rect_static.MoveToXY(m_layout.margin, m_ui.DrawAreaHeight());
	m_path_static.MoveWindow(rect_static);

	//���á���ǰ·����edit�ؼ���С
	CRect rect_edit;
	m_path_edit.GetWindowRect(rect_edit);
	if (!theApp.m_ui_data.narrow_mode)
	{
		rect_edit.right = rect_edit.left + (cx / 2 - 3 * m_layout.margin - rect_static.Width() - m_layout.select_folder_width);
		rect_edit.MoveToXY(cx / 2 + m_layout.margin + rect_static.Width(), m_layout.margin);
	}
	else
	{
		rect_edit.right = rect_edit.left + (cx - 3 * m_layout.margin - rect_static.Width() - m_layout.select_folder_width);
		rect_edit.MoveToXY(m_layout.margin + rect_static.Width(), m_ui.DrawAreaHeight());
	}
	m_path_edit.MoveWindow(rect_edit);

	//���á�ѡ���ļ��С���ť�Ĵ�С��λ��
	CRect rect_select_folder{ rect_edit };
	rect_select_folder.left = rect_edit.right + m_layout.margin;
	rect_select_folder.right = cx - m_layout.margin;
	m_set_path_button.MoveWindow(rect_select_folder);

	//���ø���������Ĵ�С��λ��
	CRect rect_search;
	m_search_edit.GetWindowRect(rect_search);
	if (!theApp.m_ui_data.narrow_mode)
	{
		rect_search.right = rect_search.left + (cx / 2 - 3 * m_layout.margin - m_layout.margin - 2*rect_search.Height());
		rect_search.MoveToXY(cx / 2 + m_layout.margin, m_layout.path_edit_height + theApp.DPI(1));
	}
	else
	{
		rect_search.right = rect_search.left + (cx - 3 * m_layout.margin - m_layout.margin - 2*rect_search.Height());
		rect_search.MoveToXY(m_layout.margin, m_ui.DrawAreaHeight() + m_layout.path_edit_height - theApp.DPI(3));
	}
	
	m_search_edit.MoveWindow(rect_search);
	//�������������ť�Ĵ�С��λ��
	CRect prev_page{}, next_page{}, clear_rect{};
	prev_page.right = prev_page.bottom = rect_search.Height();
	//if (!theApp.m_ui_data.narrow_mode)
	prev_page.MoveToXY(rect_search.right + m_layout.margin, rect_search.top);
	m_search_prev_page_button.MoveWindow(prev_page);
	m_search_prev_page_button.Invalidate();

	next_page.right = next_page.bottom = rect_search.Height();
	next_page.MoveToXY(prev_page.right + m_layout.margin, prev_page.top);
	m_search_next_page_button.MoveWindow(next_page);
	m_search_next_page_button.Invalidate();

// 	clear_rect.right = clear_rect.bottom = rect_search.Height();
// 	clear_rect.MoveToXY(next_page.right + m_layout.margin, next_page.top);
// 	m_clear_search_button.MoveWindow(clear_rect);
// 	m_clear_search_button.Invalidate();
}

void CMusicPlayerDlg::SetAlwaysOnTop()
{
	if (theApp.m_nc_setting_data.always_on_top)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);			//�����ö�
	else
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);		//ȡ���ö�
}

void CMusicPlayerDlg::ShowPlayList()
{
	m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format);
	m_playlist_list.SetCurSel(-1);
	//���ò����б���ͻ����ʾ����Ŀ
	SetPlayListColor();
	//��ʾ��ǰ·��
	m_path_edit.SetWindowTextW(CPlayer::GetInstance().GetCurrentDir().c_str());

	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.ShowPlaylist();
	}

	if (theApp.m_ui_data.float_playlist && m_pFloatPlaylistDlg != nullptr)
	{
		m_pFloatPlaylistDlg->RefreshData();
	}
}

void CMusicPlayerDlg::SetPlayListColor()
{
	m_playlist_list.SetHightItem(CPlayer::GetInstance().GetIndex());
	//m_playlist_list.SetColor(theApp.m_app_setting_data.theme_color);
	m_playlist_list.Invalidate(FALSE);
	m_playlist_list.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);

	if (theApp.m_ui_data.float_playlist && m_pFloatPlaylistDlg != nullptr)
	{
		m_pFloatPlaylistDlg->RefreshState();
	}
}

void CMusicPlayerDlg::SwitchTrack()
{
	//���л����ڲ��ŵĸ���ʱ���ò����б���ͻ����ʾ����Ŀ
	SetPlayListColor();
	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.SetPlayListColor();
		//m_miniModeDlg.RePaint();
		m_miniModeDlg.Invalidate(FALSE);
	}
	//�л�����ʱ�����ǰ������ʱ��û����ʾ������ʾ����
	CString song_length_str;
	int index{ CPlayer::GetInstance().GetIndex() };
	song_length_str = m_playlist_list.GetItemText(index, 2);
	if (song_length_str == _T("-:--") && !CPlayer::GetInstance().GetAllSongLength(index).isZero())
	{
		m_playlist_list.SetItemText(index, 2, CPlayer::GetInstance().GetAllSongLength(index).time2str().c_str());
	}

	DrawInfo(true);

	m_ui.UpdateSongInfoToolTip();
	m_ui2.UpdateSongInfoToolTip();
}

void CMusicPlayerDlg::SetPlaylistVisible()
{
	int cmdShow = (theApp.m_ui_data.show_playlist ? SW_SHOW : SW_HIDE);
	m_playlist_list.ShowWindow(cmdShow);
	m_path_static.ShowWindow(cmdShow);
	m_path_edit.ShowWindow(cmdShow);
	m_search_edit.ShowWindow(cmdShow);
// 	m_clear_search_button.ShowWindow(cmdShow);
	m_set_path_button.ShowWindow(cmdShow);
}

void CMusicPlayerDlg::SetMenubarVisible()
{
	if (theApp.m_ui_data.show_menu_bar)
	{
		SetMenu(&m_main_menu);
	}
	else
	{
		SetMenu(nullptr);
	}
}

void CMusicPlayerDlg::UpdateTaskBarProgress()
{
#ifndef COMPILE_IN_WIN_XP
	if (IsTaskbarListEnable())
	{
		//���ݲ���״̬����������״̬�ͽ���
		if (theApp.m_play_setting_data.show_taskbar_progress)
		{
			int position, length;
			position = CPlayer::GetInstance().GetCurrentPosition();
			length = CPlayer::GetInstance().GetSongLength();
			if (position == 0) position = 1;
			if (length == 0) length = 1;
			if (CPlayer::GetInstance().IsError())
				m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_ERROR);
			else if (CPlayer::GetInstance().IsPlaying())
				m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_INDETERMINATE);
			else
				m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_PAUSED);
			m_pTaskbar->SetProgressValue(this->GetSafeHwnd(), position, length);
		}
		else
		{
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_NOPROGRESS);
		}
	}
#endif
}

void CMusicPlayerDlg::UpdatePlayPauseButton()
{
	if (CPlayer::GetInstance().IsPlaying() && !CPlayer::GetInstance().IsError())
	{
#ifndef COMPILE_IN_WIN_XP
		if (IsTaskbarListEnable())
		{
			//��������������ͼ�ϡ�����/��ͣ����ͼ��
			m_thumbButton[1].hIcon = theApp.m_icon_set.pause.GetIcon();
			wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PAUSE));
			//��������ť�ϵĲ���״̬ͼ��
			if (theApp.m_play_setting_data.show_playstate_icon)
				m_pTaskbar->SetOverlayIcon(m_hWnd, theApp.m_icon_set.play.GetIcon(), L"");
			else
				m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
		}
#endif
	}
	else
	{
#ifndef COMPILE_IN_WIN_XP
		if (IsTaskbarListEnable())
		{
			//��������������ͼ�ϡ�����/��ͣ����ͼ��
			m_thumbButton[1].hIcon = theApp.m_icon_set.play.GetIcon();
			wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
			//��������ť�ϵĲ���״̬ͼ��
			if (theApp.m_play_setting_data.show_playstate_icon && CPlayer::GetInstance().GetPlayingState2() == 1)
				m_pTaskbar->SetOverlayIcon(m_hWnd, theApp.m_icon_set.pause.GetIcon(), L"");
			else
				m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
		}
#endif
	}
#ifndef COMPILE_IN_WIN_XP
	if (IsTaskbarListEnable())
		m_pTaskbar->ThumbBarUpdateButtons(m_hWnd, 3, m_thumbButton);
#endif
	if (m_miniModeDlg.m_hWnd != NULL)
		m_miniModeDlg.UpdatePlayPauseButton();

	m_pUI->UpdatePlayPauseButtonTip();
}

void CMusicPlayerDlg::SetThumbnailClipArea()
{
#ifndef COMPILE_IN_WIN_XP
	if (IsTaskbarListEnable())
	{
		if (m_pTaskbar != nullptr && m_pUI != nullptr)
			m_pTaskbar->SetThumbnailClip(m_hWnd, m_pUI->GetThumbnailClipArea());
	}
#endif
}

void CMusicPlayerDlg::EnablePlaylist(bool enable)
{
	m_playlist_list.EnableWindow(enable);
	m_search_edit.EnableWindow(enable);
// 	m_clear_search_button.EnableWindow(enable);
}


void CMusicPlayerDlg::CreateDesktopShortcut()
{
	//���Ŀ¼��û��recent_path��song_data�ļ������ж�Ϊ�ǵ�һ�����г�����ʾ�û��Ƿ񴴽������ݷ�ʽ
	if (!CCommon::FileExist(theApp.m_song_data_path) && !CCommon::FileExist(theApp.m_recent_path_dat_path))
	{
		wstring shortcut_path;

		if (MessageBox(CCommon::LoadText(IDS_CREATE_SHORTCUT_INFO), NULL, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (CCommon::CreateFileShortcut(theApp.m_desktop_path.c_str(), NULL, _T("MusicPlayer2.lnk")))
			{
				CString info;
				info = CCommon::LoadTextFormat(IDS_SHORTCUT_CREATED, { theApp.m_desktop_path });
				MessageBox(info, NULL, MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(CCommon::LoadText(IDS_SHORTCUT_CREAT_FAILED), NULL, MB_ICONWARNING);
			}
		}
	}
}

void CMusicPlayerDlg::ApplySettings(const COptionsDlg& optionDlg)
{
	//��ȡѡ�����öԻ����е���������
	if (theApp.m_lyric_setting_data.show_lyric_in_cortana == true && optionDlg.m_tab1_dlg.m_data.show_lyric_in_cortana == false)	//�����ѡ���йر��ˡ���Cortana����������ʾ��ʡ���ѡ�������Cortana��������ı�
		m_cortana_lyric.ResetCortanaText();

	bool reload_sf2{ theApp.m_general_setting_data.sf2_path != optionDlg.m_tab3_dlg.m_data.sf2_path };
	bool gauss_blur_changed{ theApp.m_app_setting_data.background_gauss_blur != optionDlg.m_tab2_dlg.m_data.background_gauss_blur
		|| theApp.m_app_setting_data.gauss_blur_radius != optionDlg.m_tab2_dlg.m_data.gauss_blur_radius
		|| theApp.m_app_setting_data.album_cover_as_background != optionDlg.m_tab2_dlg.m_data.album_cover_as_background };
	bool output_device_changed{ theApp.m_play_setting_data.output_device != optionDlg.m_tab4_dlg.m_data.output_device };

	theApp.m_lyric_setting_data = optionDlg.m_tab1_dlg.m_data;
	theApp.m_app_setting_data = optionDlg.m_tab2_dlg.m_data;
	theApp.m_general_setting_data = optionDlg.m_tab3_dlg.m_data;
	theApp.m_play_setting_data = optionDlg.m_tab4_dlg.m_data;
	theApp.m_hot_key.FromHotkeyGroup(optionDlg.m_tab5_dlg.m_hotkey_group);
	theApp.m_hot_key_setting_data = optionDlg.m_tab5_dlg.m_data;

	if (reload_sf2 || output_device_changed)		//�����ѡ�������и�����MIDI��Ƶ���·���������¼���MIDI��Ƶ��
	{
		CPlayer::GetInstance().ReIniBASS(true);
		UpdatePlayPauseButton();
	}
	if (gauss_blur_changed)
		CPlayer::GetInstance().AlbumCoverGaussBlur();

	UpdatePlayPauseButton();

	ThemeColorChanged();
	ApplyThemeColor();

	if (optionDlg.m_tab2_dlg.FontChanged())
	{
		theApp.m_font_set.lyric.SetFont(theApp.m_app_setting_data.lyric_font);
		FontInfo translate_font = theApp.m_app_setting_data.lyric_font;
		translate_font.size--;
		theApp.m_font_set.lyric_translate.SetFont(translate_font);
	}
	if (optionDlg.m_tab1_dlg.FontChanged())
	{
		CCortanaLyric::InitFont();
	}

	SaveConfig();		//������д�뵽ini�ļ�
	theApp.SaveConfig();
	CPlayer::GetInstance().SaveConfig();
	DrawInfo(true);
}

void CMusicPlayerDlg::ApplyThemeColor()
{
	CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);
	SetPlayListColor();
	m_cortana_lyric.SetUIColors();
	DrawInfo();
	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.SetPlayListColor();
	}
}

void CMusicPlayerDlg::ThemeColorChanged()
{
	if (!theApp.m_app_setting_data.theme_color_follow_system)
		return;
	COLORREF color{};
	color = CCommon::GetWindowsThemeColor();
	CColorConvert::ReduceLuminance(color);				//���������ɫ����ͽ��併��һ������
	if (theApp.m_app_setting_data.theme_color.original_color != color && color != RGB(255, 255, 255))	//��ǰ����ɫ���˵�ʱ��������������ɫ������ȷ����ȡ������ɫ���Ǵ���ɫ
	{
		theApp.m_app_setting_data.theme_color.original_color = color;
		ApplyThemeColor();
	}
}

void CMusicPlayerDlg::SetMenuState(CMenu * pMenu)
{
	//����ѭ��ģʽ�˵��ĵ�ѡ���
	RepeatMode repeat_mode{ CPlayer::GetInstance().GetRepeatMode() };
	switch (repeat_mode)
	{
	case RM_PLAY_ORDER: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_ORDER, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_PLAY_SHUFFLE: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_SHUFFLE, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_PLAYLIST: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_PLAYLIST, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_TRACK: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	default: break;
	}

	//�����Ҽ��˵�ʱ�����û��ѡ�в����б��е���Ŀ��������Ҽ��˵��С����š��������б���ɾ�����������ԡ������Ӵ���ɾ������Ŀ��
	if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EXPLORE_ONLINE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_COPY_FILE_TO, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MOVE_FILE_TO, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EXPLORE_ONLINE, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_COPY_FILE_TO, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_MOVE_FILE_TO, MF_BYCOMMAND | MF_ENABLED);
	}

	//�򿪲˵�ʱ����������б���û�и�������������˵����Ҽ��˵��еġ����ļ�λ�á���Ŀ
	if (CPlayer::GetInstance().GetSongNum() == 0)
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_ENABLED);
	}

	//���á���ͼ���˵��µĸ�ѡ���
	pMenu->CheckMenuItem(ID_SHOW_PLAYLIST, MF_BYCOMMAND | (theApp.m_ui_data.show_playlist ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_SHOW_MENU_BAR, MF_BYCOMMAND | (theApp.m_ui_data.show_menu_bar ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_FULL_SCREEN, MF_BYCOMMAND | (theApp.m_ui_data.full_screen ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_DARK_MODE, MF_BYCOMMAND | (theApp.m_app_setting_data.dark_mode ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_ALWAYS_ON_TOP, MF_BYCOMMAND | (theApp.m_nc_setting_data.always_on_top ? MF_CHECKED : MF_UNCHECKED));

	pMenu->CheckMenuItem(ID_FLOAT_PLAYLIST, MF_BYCOMMAND | (theApp.m_ui_data.float_playlist ? MF_CHECKED : MF_UNCHECKED));

	//���ò����б�˵�������ʽ�ĵ�ѡ���
	switch (CPlayer::GetInstance().m_sort_mode)
	{
	case SM_FILE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_FILE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TITLE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ARTIST: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ALBUM: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ALBUM, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TRACK: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//���ò����б�˵��С������б���ʾ��ʽ���ĵ�ѡ���
	switch (theApp.m_ui_data.display_format)
	{
	case DF_FILE_NAME: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_FILE_NAME, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_ARTIST_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_ARTIST_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE_ARTIST: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//���ò����б��Ҽ��˵���Ĭ�ϲ˵���
	pMenu->SetDefaultItem(ID_PLAY_ITEM);

	//���ݸ���Ƿ�����������û���ò˵���
	bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
	bool lyric_disable{ midi_lyric || CPlayer::GetInstance().m_Lyrics.IsEmpty() };
	bool no_lyric{ CPlayer::GetInstance().m_Lyrics.IsEmpty() && CPlayer::GetInstance().MidiNoLyric() };
	//pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!CPlayer::GetInstance().m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_CURRENT_LYRIC, MF_BYCOMMAND | (!no_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_ALL_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	//pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!CPlayer::GetInstance().m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_FORWARD, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_DELAY, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_SAVE_MODIFIED_LYRIC, MF_BYCOMMAND | ((!lyric_disable && CPlayer::GetInstance().m_Lyrics.IsModified()) ? MF_ENABLED : MF_GRAYED));
	if (midi_lyric)
		pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | MF_GRAYED);
	else
		pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | (CCommon::FileExist(CPlayer::GetInstance().m_Lyrics.GetPathName()) ? MF_ENABLED : MF_GRAYED));		//������ļ�����ʱ���á�ɾ����ʡ��˵���
	pMenu->EnableMenuItem(ID_BROWSE_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));

	pMenu->EnableMenuItem(ID_ALBUM_COVER_SAVE_AS, MF_BYCOMMAND | (CPlayer::GetInstance().AlbumCoverExist() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DOWNLOAD_ALBUM_COVER, MF_BYCOMMAND | (!CPlayer::GetInstance().IsInnerCover() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DELETE_ALBUM_COVER, MF_BYCOMMAND | ((!CPlayer::GetInstance().IsInnerCover() && CPlayer::GetInstance().AlbumCoverExist()) ? MF_ENABLED : MF_GRAYED));

	//����ִ�и�ʽת��ʱ���á���ʽת�����˵���
	pMenu->EnableMenuItem(ID_FORMAT_CONVERT, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_FORMAT_CONVERT1, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));
}

void CMusicPlayerDlg::ShowFloatPlaylist()
{
	CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
	m_pFloatPlaylistDlg = new CFloatPlaylistDlg(m_item_selected, m_items_selected, m_list_popup_menu);
	m_pFloatPlaylistDlg->Create(IDD_MUSICPLAYER2_DIALOG, GetDesktopWindow());
	m_pFloatPlaylistDlg->ShowWindow(SW_SHOW);


	theApp.m_ui_data.show_playlist = false;
	SetPlaylistVisible();
	DrawInfo(true);
}

void CMusicPlayerDlg::HideFloatPlaylist()
{
	CCommon::DeleteModelessDialog(m_pFloatPlaylistDlg);
}

BOOL CMusicPlayerDlg::OnInitDialog()
{
	CMainDialogBase::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//����ͼ����Դ
	theApp.LoadIconResource();

	//��ʼ������
	theApp.m_font_set.Init();

	//��������
	LoadConfig();

	//ֻ��Windows Vista���ϵ�ϵͳ���ܸ���ϵͳ����ɫ
#ifdef COMPILE_IN_WIN_XP
	theApp.m_app_setting_data.theme_color_follow_system = false;
//#else
//	if (!CWinVersionHelper::IsWindows8OrLater())
//		theApp.m_app_setting_data.theme_color_follow_system = false;
#endif

	//ֻ��Win10���ϵ�ϵͳ������Cortana����������ʾ���
	if (!CWinVersionHelper::IsWindows10OrLater())
		theApp.m_lyric_setting_data.show_lyric_in_cortana = false;
	m_cortana_lyric.SetEnable(CWinVersionHelper::IsWindows10OrLater());

	//���ô��ڲ�͸����
	SetTransparency();

	//��ʼ�����ڴ�С
	//rect.right = m_window_width;
	//rect.bottom = m_window_height;
	if (m_window_height != -1 && m_window_width != -1)
	{
		//MoveWindow(rect);
		SetWindowPos(nullptr, 0, 0, m_window_width, m_window_height, SWP_NOZORDER | SWP_NOMOVE);
	}

	//��ʼ����ʾ��Ϣ
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	m_Mytip.SetMaxTipWidth(theApp.DPI(400));
	m_Mytip.AddTool(GetDlgItem(ID_SET_PATH), CCommon::LoadText(IDS_SELECT_RENENT_FOLDER));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_DOWN), _T("��С����"));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_UP), _T("��������"));
	//m_Mytip.AddTool(&m_time_static, CCommon::LoadText(IDS_PLAY_TIME));
// 	m_Mytip.AddTool(&m_clear_search_button, CCommon::LoadText(IDS_CLEAR_SEARCH_RESULT));
	m_Mytip.AddTool(&m_search_prev_page_button, _T("��һҳ"));
	m_Mytip.AddTool(&m_search_next_page_button, _T("��һҳ"));
	m_Mytip.AddTool(&m_search_edit, CCommon::LoadText(IDS_INPUT_KEY_WORD));
	m_ui.SetToolTip(&m_Mytip);
	m_ui2.SetToolTip(&m_Mytip);

	//Ϊ��ʾ����ʱ���static�ؼ�����SS_NOTIFY���ԣ������������ʾ
	//DWORD dwStyle = m_time_static.GetStyle();
	//::SetWindowLong(m_time_static.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	//��ʼ���˵�
	m_main_menu.LoadMenu(IDR_MENU1);
	m_list_popup_menu.LoadMenu(IDR_POPUP_MENU);		//װ�ز����б��Ҽ��˵�

	SetMenubarVisible();

	//�����˵���ӵ�ϵͳ�˵���
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		CCommon::AppendMenuOp(pSysMenu->GetSafeHmenu(), m_main_menu.GetSafeHmenu());		//�����˵���ӵ�ϵͳ�˵�

		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_MINIMODE, CCommon::LoadText(IDS_MINI_MODE2, _T("\tCtrl+M")));

		CString exitStr;
		m_main_menu.GetMenuString(ID_MENU_EXIT, exitStr, 0);
		pSysMenu->AppendMenu(MF_STRING, ID_MENU_EXIT, exitStr);
	}

	//��ʼ����Shift���������Ҽ��˵�
	m_main_popup_menu.CreatePopupMenu();
	CCommon::AppendMenuOp(m_main_popup_menu.GetSafeHmenu(), m_main_menu.GetSafeHmenu());
	m_main_popup_menu.AppendMenu(MF_SEPARATOR);
	CString exitStr;
	m_main_menu.GetMenuString(ID_MENU_EXIT, exitStr, 0);
	m_main_popup_menu.AppendMenu(MF_STRING, ID_MENU_EXIT, exitStr);

	m_search_edit.SetCueBanner(CCommon::LoadText(IDS_SEARCH_HERE), TRUE);

	//CoInitialize(0);	//��ʼ��COM���������֧����������ʾ���Ⱥ�����ͼ��ť
#ifndef COMPILE_IN_WIN_XP
	//if (CWinVersionHelper::IsWindows7OrLater())
	//	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pTaskbar));	//����ITaskbarList3��ʵ��

	//��ʼ������������ͼ�еİ�ť
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
	//��һ����ť
	m_thumbButton[0].dwMask = dwMask;
	m_thumbButton[0].iId = IDT_PREVIOUS;
	m_thumbButton[0].hIcon = theApp.m_icon_set.previous.GetIcon();
	wcscpy_s(m_thumbButton[0].szTip, CCommon::LoadText(IDS_PREVIOUS));
	m_thumbButton[0].dwFlags = THBF_ENABLED;
	//����/��ͣ��ť
	m_thumbButton[1].dwMask = dwMask;
	m_thumbButton[1].iId = IDT_PLAY_PAUSE;
	m_thumbButton[1].hIcon = theApp.m_icon_set.play.GetIcon();
	wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
	m_thumbButton[1].dwFlags = THBF_ENABLED;
	//��һ����ť
	m_thumbButton[2].dwMask = dwMask;
	m_thumbButton[2].iId = IDT_NEXT;
	m_thumbButton[2].hIcon = theApp.m_icon_set.next.GetIcon();
	wcscpy_s(m_thumbButton[2].szTip, CCommon::LoadText(IDS_NEXT));
	m_thumbButton[2].dwFlags = THBF_ENABLED;
#endif

	//ע��ȫ���ȼ�	
	if(theApp.m_hot_key_setting_data.hot_key_enable)
		theApp.m_hot_key.RegisterAllHotKey();

	//���ý������ɫ
	CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);

	//��ʼ�����ҶԻ����е�����
	m_findDlg.LoadConfig();

	//��ȡCortana���
	m_cortana_lyric.Init();

	//��ʼ����ͼ����
	m_pDC = GetDC();
	//m_draw.Create(m_pDC, this);
	m_ui.Init(m_pDC);
	m_ui2.Init(m_pDC);
	//m_pUI = &m_ui2;

	//��ʼ���������
	theApp.m_font_set.lyric.SetFont(theApp.m_app_setting_data.lyric_font);
	FontInfo translate_font = theApp.m_app_setting_data.lyric_font;
	translate_font.size--;
	theApp.m_font_set.lyric_translate.SetFont(translate_font);

	//����Ĭ�ϱ���ͼƬ������û��ר������ʱ��ʾ��
	theApp.m_ui_data.default_background.Load((theApp.m_local_dir + L"default_background.jpg").c_str());

	m_notify_icon.Init(m_hIcon);
	m_notify_icon.AddNotifyIcon();

	//���ö�ʱ��
	SetTimer(TIMER_ID, TIMER_ELAPSE, NULL);
	SetTimer(TIMER_1_SEC, 1000, NULL);

	// �������ع���������
	m_pDownloadMgrDlg = new CDownloadMgrDlg(this);
	m_pDownloadMgrDlg->Create(IDD_DOWN_MGR_DLG);
	m_pDownloadMgrDlg->ShowWindow(SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMusicPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	DWORD cmd = nID & 0xFFF0;
	/*if (cmd == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else */if (cmd == IDM_MINIMODE)
	{
		OnMiniMode();
	}
	else
	{
		CMainDialogBase::OnSysCommand(nID, lParam);
	}

	if (nID >= 0x8000)
	{
		SendMessage(WM_COMMAND, nID);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMusicPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CMainDialogBase::OnPaint();
	}
	//DrawInfo();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMusicPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CMainDialogBase::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED && m_pUI != nullptr)
	{
		if (m_pDC != NULL)
		{
			DrawInfo(true);
			if ((cx < m_ui.WidthThreshold()) != theApp.m_ui_data.narrow_mode)	//�����խ����ģʽ����ͨģʽ֮��������л������ػ�ͻ���
			{
				Invalidate(FALSE);
				//m_time_static.Invalidate(FALSE);
			}
			m_pUI->OnSizeRedraw(cx, cy);
		}
		theApp.m_ui_data.client_width = cx;
		theApp.m_ui_data.client_height = cy;
		if (m_ui.WidthThreshold() != 0)
		{
			theApp.m_ui_data.narrow_mode = (cx < m_ui.WidthThreshold());
			//if (!theApp.m_ui_data.show_playlist)
			//	theApp.m_ui_data.narrow_mode = false;
		}
		if (m_playlist_list.m_hWnd)
		{
			SetPlaylistSize(cx, cy);
		}

		if (nType != SIZE_MAXIMIZED && !theApp.m_ui_data.full_screen)
		{
			CRect rect;
			GetWindowRect(&rect);
			m_window_width = rect.Width();
			m_window_height = rect.Height();
		}

		//���ڴ�С�仯ʱ���½��������ʾ��λ��
		static UINT last_type;
		if (last_type!=nType)
		{
			if (m_pUI != nullptr)
				DrawInfo();
		}
		last_type = nType;
		m_pUI->UpdateToolTipPosition();
	}

	SetThumbnailClipArea();


	// TODO: �ڴ˴������Ϣ����������
}


void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��Ӧ����ʱ��
	if(nIDEvent==TIMER_ID)
	{
		if (m_first_start)
		{
			//��if���ֻ�ڶ�ʱ����1�δ���ʱ��ִ��
			m_first_start = false;
#ifndef COMPILE_IN_WIN_XP
			if (IsTaskbarListEnable())
			{
				//��������������ͼ���ڰ�ť
				m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
			}
#endif
			CRect rect;
			GetClientRect(rect);
			theApp.m_ui_data.client_width = rect.Width();
			theApp.m_ui_data.client_height = rect.Height();
			SetPlaylistSize(rect.Width(), rect.Height());		//���������б�Ĵ�С��λ��
			m_path_static.Invalidate();
			//SetPorgressBarSize(rect.Width(), rect.Height());		//�����������ڴ����еĴ�С��λ��
			SetPlaylistVisible();

			if (m_cmdLine.empty())		//û����ͨ�������д��ļ�
			{
				CPlayer::GetInstance().Create();
			}
			else if (m_cmdLine.find(L"RestartByRestartManager") != wstring::npos)		//��������в�������RestartByRestartManager������������в���
			{
				CPlayer::GetInstance().Create();
				////�������в���д����־�ļ�
				//wchar_t buff[256];
				//swprintf_s(buff, L"�����ѱ�Windows��RestartManager����������������%s", m_cmdLine.c_str());
				//theApp.WriteErrorLog(wstring{ buff });
			}
			else		//�������в�����ȡҪ�򿪵��ļ�
			{
				vector<wstring> files;
				wstring path = CCommon::DisposeCmdLineFiles(m_cmdLine, files);
				if (!path.empty())
					CPlayer::GetInstance().Create(path);
				else
					CPlayer::GetInstance().Create(files);
				//MessageBox(m_cmdLine.c_str(), NULL, MB_ICONINFORMATION);
			}
			DrawInfo();
			SetThumbnailClipArea();

			//ע����Ӧ��������򿪻򲥷Ÿ�����Ӧ���ڲ����б��ʼ�����ʱִ�С�
			//CPlayer::GetInstance().MusicControl(Command::OPEN);
			//CPlayer::GetInstance().MusicControl(Command::SEEK);
			//CPlayer::GetInstance().GetBASSError();
			//SetPorgressBarSize(rect.Width(), rect.Height());		//���µ����������ڴ����еĴ�С��λ�ã���Ҫ���ݸ�����ʱ��������ʾʱ��ؼ��Ŀ�ȣ�
			//ShowTime();
			//m_progress_bar.SetSongLength(CPlayer::GetInstance().GetSongLength());

			//if(!m_cmdLine.empty())
			//	CPlayer::GetInstance().MusicControl(Command::PLAY);	//����ļ���ͨ�������д򿪵ģ���򿪺�ֱ�Ӳ���

			UpdatePlayPauseButton();
			//SetForegroundWindow();
			//ShowPlayList();

			ThemeColorChanged();

			if (theApp.m_ui_data.float_playlist)
				ShowFloatPlaylist();

			//��ʾ�û��Ƿ񴴽������ݷ�ʽ
			CreateDesktopShortcut();

			SetAlwaysOnTop();
		}

		m_timer_count++;

		UpdateTaskBarProgress();
		//UpdateProgress();

		CPlayer::GetInstance().GetBASSError();
		if (m_miniModeDlg.m_hWnd == NULL && (CPlayer::GetInstance().IsPlaying() || GetActiveWindow() == this))		//��������ģʽʱ��ˢ�£����ڲ����Ҵ��ڴ��ں�̨ʱ��ˢ��
			DrawInfo();			//���ƽ����ϵ���Ϣ�������ʾ������ģʽ���򲻻��ƽ�����Ϣ��
		CPlayer::GetInstance().GetBASSSpectral();
		if (CPlayer::GetInstance().IsPlaying())
		{
			CPlayer::GetInstance().GetBASSCurrentPosition();
		}

		//��Cortana����������ʾ���
		if (theApp.m_lyric_setting_data.show_lyric_in_cortana)
		{
			if (theApp.m_lyric_setting_data.cortana_lyric_keep_display || CPlayer::GetInstance().IsPlaying())
				m_cortana_lyric.DrawInfo();
		}

		//if (CPlayer::GetInstance().SongIsOver() && (!theApp.m_lyric_setting_data.stop_when_error || !CPlayer::GetInstance().IsError()))	//��ǰ��Ŀ���������û�г��ִ���ʱ�Ų�����һ��
		if ((CPlayer::GetInstance().SongIsOver() || (!theApp.m_play_setting_data.stop_when_error && CPlayer::GetInstance().IsError())) && m_play_error_cnt <= CPlayer::GetInstance().GetSongNum())	//��ǰ��Ŀ���������û�г��ִ���ʱ�Ų�����һ��
		{
			if (CPlayer::GetInstance().IsError())
				m_play_error_cnt++;
			else
				m_play_error_cnt = 0;
			//��ǰ���ڱ༭��ʣ���˳�򲥷�ģʽ���б��еĸ����������ʱ��PlayTrack�����᷵��false���������굱ǰ������ֹͣ����
			if ((m_pLyricEdit != nullptr && m_pLyricEdit->m_dlg_exist) || !CPlayer::GetInstance().PlayTrack(NEXT))
			{
				CPlayer::GetInstance().MusicControl(Command::STOP);		//ֹͣ����
				//ShowTime();
				if (theApp.m_lyric_setting_data.show_lyric_in_cortana)
					m_cortana_lyric.ResetCortanaText();
			}
			SwitchTrack();
			UpdatePlayPauseButton();
		}
		if (CPlayer::GetInstance().IsPlaying() && (theApp.m_play_setting_data.stop_when_error && CPlayer::GetInstance().IsError()))
		{
			CPlayer::GetInstance().MusicControl(Command::PAUSE);
			UpdatePlayPauseButton();
		}

		//if (m_timer_count % 10 == 0)
		//	m_cortana_lyric.CheckDarkMode();

		if (CWinVersionHelper::IsWindowsVista())
		{
			if (m_timer_count % 15 == 14)
				ThemeColorChanged();
		}

		if (m_timer_count % 600 == 599)
		{
			//CPlayer::GetInstance().EmplaceCurrentPathToRecent();
			//CPlayer::GetInstance().SaveRecentPath();
			if(theApp.IsSongDataModified())				//�ڸ�����Ϣ���޸Ĺ�������£�ÿ��һ����ʱ�䱣��һ��
				theApp.SaveSongData();
		}
	}

	//��Ӧ1�붨ʱ��
	else if (nIDEvent == TIMER_1_SEC)
	{
		if (CPlayer::GetInstance().IsPlaying())
		{
			CPlayer::GetInstance().AddListenTime(1);
		}

	}

	CMainDialogBase::OnTimer(nIDEvent);
}


void CMusicPlayerDlg::OnPlayPause()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().MusicControl(Command::PLAY_PAUSE);
	if (!CPlayer::GetInstance().IsPlaying())
		DrawInfo();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnStop()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().MusicControl(Command::STOP);
	UpdatePlayPauseButton();
	//ShowTime();
}


void CMusicPlayerDlg::OnPrevious()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().PlayTrack(PREVIOUS);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnNext()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().PlayTrack(NEXT);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnRew()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().MusicControl(Command::REW);
	UpdateTaskBarProgress();
	//ShowTime();
}


void CMusicPlayerDlg::OnFF()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().MusicControl(Command::FF);
	UpdateTaskBarProgress();
	//ShowTime();
}


void CMusicPlayerDlg::OnSetPath()
{
	// TODO: �ڴ���������������
	CCommon::DeleteModelessDialog(m_pSetPathDlg);
	m_pSetPathDlg = new CSetPathDlg(CPlayer::GetInstance().GetRecentPath(), CPlayer::GetInstance().GetCurrentDir());
	m_pSetPathDlg->Create(IDD_SET_PATH_DIALOG);
	m_pSetPathDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnDownMgr()
{
	m_pDownloadMgrDlg->ShowWindow(SW_SHOW);
}

afx_msg LRESULT CMusicPlayerDlg::OnPathSelected(WPARAM wParam, LPARAM lParam)
{
	if (m_pSetPathDlg != nullptr)
	{
		CPlayer::GetInstance().SetPath(m_pSetPathDlg->GetSelPath(), m_pSetPathDlg->GetTrack(), m_pSetPathDlg->GetPosition(), m_pSetPathDlg->GetSortMode());
		UpdatePlayPauseButton();
		//SetPorgressBarSize();
		//ShowTime();
		DrawInfo(true);
		m_findDlg.ClearFindResult();		//����·����������ҽ��
		CPlayer::GetInstance().SaveRecentPath();
		m_play_error_cnt = 0;
	}
	return 0;
}


void CMusicPlayerDlg::OnFind()
{
	// TODO: �ڴ���������������
	if (m_findDlg.DoModal() == IDOK)
	{
		if (m_findDlg.GetFindCurrentPlaylist())
		{
			int selected_track{ m_findDlg.GetSelectedTrack() };
			if (selected_track >= 0)
			{
				CPlayer::GetInstance().PlayTrack(m_findDlg.GetSelectedTrack());
				SwitchTrack();
				UpdatePlayPauseButton();
			}
		}
		else
		{
			wstring selected_song_path{ m_findDlg.GetSelectedSongPath() };
			if (!CCommon::FileExist(selected_song_path))
			{
				//����ļ������ڣ��򵯳�������Ϣ
				CString info;
				info = CCommon::LoadTextFormat(IDS_CONNOT_FIND_FILE, { selected_song_path });
				MessageBox(info, NULL, MB_ICONWARNING);
				return;
			}
			if (!selected_song_path.empty())
			{
				CPlayer::GetInstance().OpenAFile(selected_song_path);
				SwitchTrack();
				UpdatePlayPauseButton();
			}
		}
	}
}


void CMusicPlayerDlg::OnExplorePath()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().ExplorePath();
}


BOOL CMusicPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���


	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{

		if (GetFocus() == GetDlgItem(IDC_SEARCH_EDIT))
		{
			m_current_page_idx = 1;
			UpdateSearchList();
// 			m_playlist_list.QuickSearch(wstring(key_word));
			m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
		}
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd != m_search_edit.GetSafeHwnd())
	{
		//����Ctrl��ʱ
		if (GetKeyState(VK_CONTROL) & 0x80)
		{
			if (pMsg->wParam == 'O')		//���ð�Ctr+O���ļ�
			{
				OnFileOpen();
				return TRUE;
			}
			if (pMsg->wParam == 'D')		//���ð�Ctr+D���ļ���
			{
				OnFileOpenFolder();
				return TRUE;
			}
			if (pMsg->wParam == 'B')		//���ð�Ctr+B����ļ�
			{
				OnExplorePath();
				return TRUE;
			}
			if (pMsg->wParam == 'N')		//���ð�Ctr+N����Ŀ��Ϣ
			{
				OnSongInfo();
				return TRUE;
			}
			if (pMsg->wParam == 'F')		//��Ctr+F�������ļ�
			{
				OnFind();
				return TRUE;
			}
			if (pMsg->wParam == 'T')		//��Ctr+T������·��
			{
				OnSetPath();
				return TRUE;
			}
			if (pMsg->wParam == 'I')		//���ð�Ctr+I������
			{
				OnOptionSettings();
				return TRUE;
			}
			//if (pMsg->wParam == 'X')		//���ð�Ctr+X�˳�
			//{
			//	OnMenuExit();
			//	return TRUE;
			//}
			if (pMsg->wParam == 'R')		//���ð�Ctr+R��¼����
			{
				OnRecorder();
				return TRUE;
			}
			if (pMsg->wParam == 'M')		//���ð�Ctr+M��������ģʽ
			{
				OnMiniMode();
				return TRUE;
			}
			if (pMsg->wParam == VK_PRIOR)		//���ð�Ctr+PgUp�����ǰ0.5��
			{
				OnLyricForward();
				return TRUE;
			}
			if (pMsg->wParam == VK_NEXT)		//���ð�Ctr+PgDn����Ӻ�0.5��
			{
				OnLyricDelay();
				return TRUE;
			}
			if (pMsg->wParam == 'S')		//���ð�Ctr+S�����ʸ���
			{
				if (!CPlayer::GetInstance().m_Lyrics.IsEmpty() && CPlayer::GetInstance().m_Lyrics.IsModified())
					OnSaveModifiedLyric();
				return TRUE;
			}
			if (pMsg->wParam == 'E')		//���ð�Ctr+S�򿪾�����
			{
				OnEqualizer();
				return TRUE;
			}
			if (pMsg->wParam == 'L')		//���ð�Ctr+L��ʾ/���ز����б�
			{
				OnShowPlaylist();
				return TRUE;
			}
			if (pMsg->wParam == 'K')		//���ð�Ctr+K��ʾ���������б�
			{
				OnFloatPlaylist();
				return TRUE;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
//				//����Ctrl + Shift��ʱ
//#ifdef _DEBUG
//				if (pMsg->wParam == 'Z')
//				{
//					CTest::Test();
//					return TRUE;
//				}
//#endif
			}
		}
		else
		{
			if (pMsg->wParam == VK_SPACE || pMsg->wParam == 'P'/* || pMsg->wParam == VK_MEDIA_PLAY_PAUSE*/)		//���ո��/P������/��ͣ
			{
				OnPlayPause();
				return TRUE;
			}
			if (pMsg->wParam == 'S')	//��S��ֹͣ
			{
				OnStop();
				return TRUE;
			}
			if (pMsg->wParam == 'V')	//��V����һ��
			{
				OnPrevious();
				return TRUE;
			}
			if (pMsg->wParam == 'N')	//��N����һ��
			{
				OnNext();
				return TRUE;
			}
			if (pMsg->wParam == VK_LEFT)	//��������¿���5��
			{
				OnRew();
				return TRUE;
			}
			if (pMsg->wParam == VK_RIGHT)	//���ҷ�����¿��5��
			{
				OnFF();
				return TRUE;
			}
			if (pMsg->wParam == VK_UP)	//���Ϸ������������
			{
				OnVolumeUp();
				return TRUE;
			}
			if (pMsg->wParam == VK_DOWN)	//���·����������
			{
				OnVolumeDown();
				return TRUE;
			}
			if (pMsg->wParam == 'M')	//��M������ѭ��ģʽ
			{
				CPlayer::GetInstance().SetRepeatMode();
				m_ui.UpdateRepeatModeToolTip();
				m_ui2.UpdateRepeatModeToolTip();
				return TRUE;
			}
			if (pMsg->wParam == 'F')	//��F�����ٲ���
			{
				m_search_edit.SetFocus();
				return TRUE;
			}
			if (pMsg->wParam == VK_F11)	//��F11��������˳�ȫ��ģʽ
			{
				OnFullScreen();
				return TRUE;
			}
			if (pMsg->wParam == VK_ESCAPE)	//��ESC���˳�ȫ��ģʽ
			{
				if (theApp.m_ui_data.full_screen)
				{
					OnFullScreen();
					return TRUE;
				}
			}

			if (pMsg->wParam == VK_APPS)		//���˵����������˵�
			{
				SendMessage(WM_MAIN_MENU_POPEDUP, (WPARAM)&CPoint(0, 0));
				return TRUE;
			}
		}
	}

	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))		//���ΰ��س�����ESC���˳�
	{
		return TRUE;
	}


	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);


	return CMainDialogBase::PreTranslateMessage(pMsg);
}


void CMusicPlayerDlg::OnDestroy()
{
	CMainDialogBase::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	//�˳�ʱ��������
	CPlayer::GetInstance().OnExit();
	SaveConfig();
	m_findDlg.SaveConfig();
	theApp.SaveConfig();
	//���ȫ���ȼ�
	theApp.m_hot_key.UnRegisterAllHotKey();

	//�˳�ʱ�ָ�Cortana��Ĭ���ı�
	m_cortana_lyric.ResetCortanaText();

	////�˳�ʱɾ��ר��������ʱ�ļ�
	//DeleteFile(CPlayer::GetInstance().GetAlbumCoverPath().c_str());

	m_notify_icon.DeleteNotifyIcon();
}


void CMusicPlayerDlg::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMusicPlayerDlg::OnFileOpen()
{
	// TODO: �ڴ���������������
	vector<wstring> files;	//����򿪵Ķ���ļ�·��
	//���ù�����
	wstring filter(CCommon::LoadText(IDS_ALL_SUPPORTED_FORMAT, _T("|")));
	for (const auto& ext : CAudioCommon::m_all_surpported_extensions)
	{
		filter += L"*.";
		filter += ext;
		filter.push_back(L';');
	}
	filter.pop_back();
	filter.push_back(L'|');
	for (const auto& format : CAudioCommon::m_surpported_format)
	{
		filter += format.description;
		filter.push_back(L'|');
		filter += format.extensions_list;
		filter.push_back(L'|');
	}
	filter += CCommon::LoadText(IDS_ALL_FILES, _T("|*.*||"));
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, filter.c_str(), this);
	//���ñ����ļ������ַ�����Ĵ�СΪ128kB�������ƽ��һ���ļ�������Ϊ32�ֽڼ��㣬�����Դ򿪴�Լ4096���ļ���
	fileDlg.m_ofn.nMaxFile = 128 * 1024;
	LPTSTR ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
	fileDlg.m_ofn.lpstrFile = ch;
	//���ڴ������
	ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
	//��ʾ���ļ��Ի���
	if (IDOK == fileDlg.DoModal())
	{
		POSITION posFile = fileDlg.GetStartPosition();
		while (posFile != NULL)
		{
			files.push_back(fileDlg.GetNextPathName(posFile).GetString());
		}
		CPlayer::GetInstance().OpenFiles(files);
		//ShowPlayList();
		UpdatePlayPauseButton();
		//SetPorgressBarSize();
		DrawInfo(true);
		m_play_error_cnt = 0;
	}
	delete[] ch;
}


void CMusicPlayerDlg::OnFileOpenFolder()
{
	// TODO: �ڴ���������������

#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(CCommon::LoadText(IDS_OPEN_FOLDER_INFO));
#else
	CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		CPlayer::GetInstance().OpenFolder(wstring(folderPickerDlg.GetPathName()));
		//ShowPlayList();
		UpdatePlayPauseButton();
		//SetPorgressBarSize();
		DrawInfo(true);
		m_play_error_cnt = 0;
	}
}


void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	vector<wstring> files;	//�����Ϸŵ����ڵĶ���ļ�·��
	TCHAR file_path[MAX_PATH];
	int drop_count = DragQueryFile(hDropInfo, -1, NULL, 0);		//ȡ�ñ��϶��ļ�����Ŀ
	//��ȡ��1���ļ�
	DragQueryFile(hDropInfo, 0, file_path, MAX_PATH);
	wstring file_path_wcs{ file_path };
	//if (file_path_wcs.size() > 4 && file_path_wcs[file_path_wcs.size() - 4] != L'.' && file_path_wcs[file_path_wcs.size() - 5] != L'.')
	if(CCommon::IsFolder(file_path_wcs))
	{
		//file_path_wcs.push_back(L'\\');
		CPlayer::GetInstance().OpenFolder(file_path_wcs);
	}
	else
	{
		for (int i{}; i < drop_count; i++)
		{
			DragQueryFile(hDropInfo, i, file_path, MAX_PATH);	//��ȡ��i���ļ�
			if (CAudioCommon::FileIsAudio(wstring(file_path)))
				files.push_back(file_path);
		}
		CPlayer::GetInstance().OpenFiles(files, false);
	}
	//ShowPlayList();
	UpdatePlayPauseButton();
	//SetPorgressBarSize();
	DrawInfo(true);

	CMainDialogBase::OnDropFiles(hDropInfo);
}


void CMusicPlayerDlg::OnInitMenu(CMenu* pMenu)
{
	CMainDialogBase::OnInitMenu(pMenu);

	// TODO: �ڴ˴������Ϣ����������
	SetMenuState(pMenu);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	SetMenuState(pSysMenu);
}


void CMusicPlayerDlg::OnPlayOrder()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().SetRepeatMode(RM_PLAY_ORDER);		//����˳�򲥷�
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnPlayShuffle()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().SetRepeatMode(RM_PLAY_SHUFFLE);		//�����������
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopPlaylist()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().SetRepeatMode(RM_LOOP_PLAYLIST);		//�����б�ѭ��
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopTrack()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().SetRepeatMode(RM_LOOP_TRACK);		//���õ���ѭ��
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


BOOL CMusicPlayerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		OnVolumeUp();
	}
	if (zDelta < 0)
	{
		OnVolumeDown();
	}

	return CMainDialogBase::OnMouseWheel(nFlags, zDelta, pt);
}


void CMusicPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ƴ�����С��С
	lpMMI->ptMinTrackSize.x = theApp.DPI(340);		//������С���
	lpMMI->ptMinTrackSize.y = theApp.DPI(360);		//������С�߶�

	CMainDialogBase::OnGetMinMaxInfo(lpMMI);
}


void CMusicPlayerDlg::OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_searched)	//��������б�������״̬����ǰѡ������кž�����Ŀ������
	{
		if (pNMItemActivate->iItem < 0)
			return;
		CPlayer::GetInstance().PlayTrack(pNMItemActivate->iItem);
	}
	else		//��������б���ѡ��״̬������Ŀ��������ѡ���е�һ�е�����-1
	{
		int song_index;
		CString str;
		str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
		song_index = _ttoi(str) - 1;
		if (song_index < 0)
			return;
		CPlayer::GetInstance().PlayTrack(song_index);
	}
	SwitchTrack();
	UpdatePlayPauseButton();

	*pResult = 0;
}


//void CMusicPlayerDlg::OnRefreshPlaylist()
//{
//	// TODO: �ڴ���������������
//	ShowPlayList();
//}


void CMusicPlayerDlg::OnOptionSettings()
{
	// TODO: �ڴ���������������

	theApp.m_hot_key.UnRegisterAllHotKey();

	COptionsDlg optionDlg;
	//��ʼ���Ի����б�����ֵ
	optionDlg.m_tab_selected = m_tab_selected;
	optionDlg.m_tab1_dlg.m_data = theApp.m_lyric_setting_data;
	if (m_miniModeDlg.m_hWnd == NULL)
		optionDlg.m_tab2_dlg.m_hMainWnd = m_hWnd;
	else
		optionDlg.m_tab2_dlg.m_hMainWnd = m_miniModeDlg.m_hWnd;
	optionDlg.m_tab2_dlg.m_data = theApp.m_app_setting_data;
	optionDlg.m_tab3_dlg.m_data = theApp.m_general_setting_data;
	optionDlg.m_tab4_dlg.m_data = theApp.m_play_setting_data;
	optionDlg.m_tab5_dlg.m_hotkey_group = theApp.m_hot_key.GetHotKeyGroup();
	optionDlg.m_tab5_dlg.m_data = theApp.m_hot_key_setting_data;

	int sprctrum_height = theApp.m_app_setting_data.sprctrum_height;		//����theApp.m_app_setting_data.sprctrum_height��ֵ������û������ѡ��Ի����ȡ��������Ҫ�ѻָ�Ϊԭ����
	int background_transparency = theApp.m_app_setting_data.background_transparency;		//ͬ��

	if (optionDlg.DoModal() == IDOK)
	{
		ApplySettings(optionDlg);
	}
	else
	{
		SetTransparency();		//��������ȡ��������Ҫ�������ô���͸����
		if (m_miniModeDlg.m_hWnd != NULL)
			m_miniModeDlg.SetTransparency();

		theApp.m_app_setting_data.sprctrum_height = sprctrum_height;
		theApp.m_app_setting_data.background_transparency = background_transparency;
	}

	m_tab_selected = optionDlg.m_tab_selected;
	
	if(theApp.m_hot_key_setting_data.hot_key_enable)
		theApp.m_hot_key.RegisterAllHotKey();

}


void CMusicPlayerDlg::OnReloadPlaylist()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().ReloadPlaylist();
	//ShowPlayList();
	//UpdatePlayPauseButton();
	//ShowTime();
}


void CMusicPlayerDlg::OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_searched)
	{
		m_item_selected = pNMItemActivate->iItem;	//��ȡ���ѡ�е���Ŀ
		m_playlist_list.GetItemSelected(m_items_selected);		//��ȡ���ѡ�е���Ŀ
	}
	else
	{
		CString str;
		str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
		m_item_selected = _ttoi(str) - 1;
		m_playlist_list.GetItemSelectedSearched(m_items_selected);
	}

	CMenu* pContextMenu = m_list_popup_menu.GetSubMenu(0); //��ȡ��һ�������˵�
	CPoint point;			//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	if (m_item_selected >= 0 && m_item_selected < CPlayer::GetInstance().GetSongNum())
	{
		CRect item_rect;
		m_playlist_list.GetItemRect(pNMItemActivate->iItem, item_rect, LVIR_BOUNDS);		//��ȡѡ����Ŀ�ľ��������Բ����б�ؼ����Ͻ�Ϊԭ�㣩
		CRect playlist_rect;
		m_playlist_list.GetWindowRect(playlist_rect);		//��ȡ�����б�ؼ��ľ�����������Ļ���Ͻ�Ϊԭ�㣩
		point.y = playlist_rect.top + item_rect.bottom;	//�������Ҫ������y����Ϊѡ����Ŀ���±߿�λ�ã���ֹ�Ҽ��˵���סѡ�е���Ŀ
	}

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //��ָ��λ����ʾ�����˵�

	*pResult = 0;
}


void CMusicPlayerDlg::OnPlayItem()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().PlayTrack(m_item_selected);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnDownloadItem()
{
	m_pDownloadMgrDlg->GetDownListCtrl().Download(m_item_selected);
}

void CMusicPlayerDlg::OnItemProperty()
{
	// TODO: �ڴ���������������
	CPropertyDlg propertyDlg(CPlayer::GetInstance().GetPlayList());
	propertyDlg.m_path = CPlayer::GetInstance().GetCurrentDir();
	propertyDlg.m_index = m_item_selected;
	propertyDlg.m_song_num = CPlayer::GetInstance().GetSongNum();
	//propertyDlg.m_playing_index = CPlayer::GetInstance().GetIndex();
	//propertyDlg.m_lyric_name = CPlayer::GetInstance().GetLyricName();
	propertyDlg.DoModal();
	if (propertyDlg.GetListRefresh())
		ShowPlayList();
}


//void CMusicPlayerDlg::OnRemoveFromPlaylist()
//{
//	// TODO: �ڴ���������������
//	CPlayer::GetInstance().RemoveSong(m_item_selected);
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


//void CMusicPlayerDlg::OnClearPlaylist()
//{
//	// TODO: �ڴ���������������
//	CPlayer::GetInstance().ClearPlaylist();
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


void CMusicPlayerDlg::OnExploreTrack()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().ExplorePath(m_item_selected);
}


BOOL CMusicPlayerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	//��Ӧ����������ͼ��ť
	WORD command = LOWORD(wParam);
	switch(command)
	{
	case IDT_PLAY_PAUSE: OnPlayPause(); break;
	case IDT_PREVIOUS: OnPrevious(); break;
	case IDT_NEXT: OnNext(); break;
	}

	return CMainDialogBase::OnCommand(wParam, lParam);
}


void CMusicPlayerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��Ӧȫ�ֿ�ݼ�
	switch (nHotKeyId)
	{
	case HK_PLAY_PAUSE: OnPlayPause(); break;
	case HK_PREVIOUS: OnPrevious(); break;
	case HK_NEXT: OnNext(); break;
	case HK_VOLUME_UP:
		OnVolumeUp();
		break;
	case HK_VOLUME_DOWN:
		OnVolumeDown();
		break;
	default: break;
	case HK_STOP:
		OnStop();
		break;
	case HK_FF:
		OnFF();
		break;
	case HK_REW:
		OnRew();
		break;
	case HK_EXIT:
		OnMenuExit();
		break;
	}

	CMainDialogBase::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMusicPlayerDlg::OnReIniBass()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().ReIniBASS();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnSortByFile()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().m_sort_mode = SM_FILE;
	CPlayer::GetInstance().SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTitle()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().m_sort_mode = SM_TITLE;
	CPlayer::GetInstance().SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByArtist()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().m_sort_mode = SM_ARTIST;
	CPlayer::GetInstance().SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByAlbum()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().m_sort_mode = SM_ALBUM;
	CPlayer::GetInstance().SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTrack()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().m_sort_mode = SM_TRACK;
	CPlayer::GetInstance().SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnDeleteFromDisk()
{
	// TODO: �ڴ���������������
	if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
		return;
	int rtn;
	wstring delected_file;
	vector<wstring> delected_files;
	if (m_items_selected.size() > 1)
	{
		CString info;
		info = CCommon::LoadTextFormat(IDS_DELETE_FILE_INQUARY, { m_items_selected.size() });
		if (MessageBox(info, NULL, MB_ICONWARNING | MB_OKCANCEL) != IDOK)
			return;
		if(CCommon::IsItemInVector(m_items_selected, CPlayer::GetInstance().GetIndex()))	//���ѡ�е��ļ��������ڲ��ŵ��ļ�����ɾ��ǰ�����ȹر��ļ�
			CPlayer::GetInstance().MusicControl(Command::CLOSE);
		for (const auto& index : m_items_selected)
			delected_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
		rtn = CCommon::DeleteFiles(m_hWnd, delected_files);
	}
	else
	{
		if (m_item_selected == CPlayer::GetInstance().GetIndex())	//���ɾ�����ļ������ڲ��ŵ��ļ�����ɾ��ǰ�����ȹر��ļ�
			CPlayer::GetInstance().MusicControl(Command::CLOSE);
		delected_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
		rtn = CCommon::DeleteAFile(m_hWnd, delected_file);
	}
	if (rtn == 0)
	{
		//����ļ�ɾ���ɹ���ͬʱ�Ӳ����б����Ƴ�
		if (m_items_selected.size() > 1)
			CPlayer::GetInstance().RemoveSongs(m_items_selected);
		else
			CPlayer::GetInstance().RemoveSong(m_item_selected);
		ShowPlayList();
		UpdatePlayPauseButton();
		DrawInfo(true);
		//�ļ�ɾ����ͬʱɾ�����ļ�ͬ����ͼƬ�ļ��͸���ļ�
		if (m_items_selected.size() > 1)
		{
			for (auto& file : delected_files)
			{
				CFilePathHelper file_path(file);
				file = file_path.ReplaceFileExtension(L"jpg").c_str();
			}
			CCommon::DeleteFiles(m_hWnd, delected_files);
			for (auto& file : delected_files)
			{
				CFilePathHelper file_path(file);
				file = file_path.ReplaceFileExtension(L"lrc").c_str();
			}
			CCommon::DeleteFiles(m_hWnd, delected_files);
		}
		else
		{
			CFilePathHelper file_path(delected_file);
			CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"jpg").c_str());
			CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"lrc").c_str());
		}
	}
	else if (rtn == 1223)	//����ڵ����ĶԻ����е����ȡ�����򷵻�ֵΪ1223
	{
		if (m_item_selected == CPlayer::GetInstance().GetIndex())		//���ɾ�����ļ������ڲ��ŵ��ļ����ֵ���ˡ�ȡ�����������´򿪵�ǰ�ļ�
		{
			CPlayer::GetInstance().MusicControl(Command::OPEN);
			CPlayer::GetInstance().MusicControl(Command::SEEK);
			//CPlayer::GetInstance().Refresh();
			UpdatePlayPauseButton();
			DrawInfo(true);
		}
	}
	else
	{
		MessageBox(CCommon::LoadText(IDS_CONNOT_DELETE_FILE), NULL, MB_ICONWARNING);
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnTaskbarcreated(WPARAM wParam, LPARAM lParam)
{
#ifndef COMPILE_IN_WIN_XP
	if (IsTaskbarListEnable())
	{
		//����Դ�����������������������������ͼ���ڰ�ť
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
		SetThumbnailClipArea();
		//���¸����������ϵĲ���״̬�Ǳ�
		UpdatePlayPauseButton();
	}
#endif
	//��Դ������������Cortana�ľ���ᷢ���ı䣬��ʱҪ���»�ȡCortana�ľ��
	m_cortana_lyric.Init();

	m_notify_icon.AddNotifyIcon();		//�������֪ͨ��ͼ��

	return 0;
}


void CMusicPlayerDlg::OnDispFileName()
{
	// TODO: �ڴ���������������
	theApp.m_ui_data.display_format = DF_FILE_NAME;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitle()
{
	// TODO: �ڴ���������������
	theApp.m_ui_data.display_format = DF_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispArtistTitle()
{
	// TODO: �ڴ���������������
	theApp.m_ui_data.display_format = DF_ARTIST_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitleArtist()
{
	// TODO: �ڴ���������������
	theApp.m_ui_data.display_format = DF_TITLE_ARTIST;
	ShowPlayList();
}


void CMusicPlayerDlg::OnMiniMode()
{
	// TODO: �ڴ���������������
	if (m_miniModeDlg.m_hWnd != NULL)
		return;

	//m_miniModeDlg.SetDefaultBackGround(&theApp.m_ui_data.default_background);
	//m_miniModeDlg.SetDisplayFormat(&theApp.m_ui_data.display_format);
	ShowWindow(SW_HIDE);
	if (m_miniModeDlg.DoModal() == IDCANCEL)
	{
		//SendMessage(WM_COMMAND, ID_APP_EXIT);
		OnMenuExit();
	}
	else
	{
		ShowWindow(SW_SHOW);
#ifndef COMPILE_IN_WIN_XP
		if (IsTaskbarListEnable())
		{
			m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//�����������������ͼ��ť
			SetThumbnailClipArea();		//������������������ͼ
		}
#endif
		SetForegroundWindow();
		SwitchTrack();
		UpdatePlayPauseButton();
		SetTransparency();
	}
}


void CMusicPlayerDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	OnStop();
}


void CMusicPlayerDlg::OnBnClickedPrevious()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	OnPrevious();
}


void CMusicPlayerDlg::OnBnClickedPlayPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	OnPlayPause();
}


void CMusicPlayerDlg::OnBnClickedNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	OnNext();
}


//void CMusicPlayerDlg::OnMove(int x, int y)
//{
//	CMainDialogBase::OnMove(x, y);
//
//	// TODO: �ڴ˴������Ϣ����������
//	SetMaskWindowPos();
//}


void CMusicPlayerDlg::OnReloadLyric()
{
	// TODO: �ڴ���������������
	CWaitCursor wait_cursor;
	CPlayer::GetInstance().SearchLyrics();
	CPlayer::GetInstance().IniLyrics();
}


void CMusicPlayerDlg::OnSongInfo()
{
	// TODO: �ڴ���������������
	CPropertyDlg propertyDlg(CPlayer::GetInstance().GetPlayList());
	propertyDlg.m_path = CPlayer::GetInstance().GetCurrentDir();
	propertyDlg.m_index = CPlayer::GetInstance().GetIndex();
	propertyDlg.m_song_num = CPlayer::GetInstance().GetSongNum();
	propertyDlg.DoModal();
	if (propertyDlg.GetListRefresh())
		ShowPlayList();
}


void CMusicPlayerDlg::OnCopyCurrentLyric()
{
	// TODO: �ڴ���������������
	bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
	wstring lyric_str;
	if (midi_lyric)
	{
		lyric_str = CPlayer::GetInstance().GetMidiLyric();
	}
	else
	{
		CLyrics::Lyric lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(Time(CPlayer::GetInstance().GetCurrentPosition()), 0) };
		lyric_str = lyric.text;
		if (theApp.m_ui_data.show_translate && !lyric.translate.empty())
		{
			lyric_str += L"\r\n";
			lyric_str += lyric.translate;
		}
	}
	if (!CCommon::CopyStringToClipboard(lyric_str))
	//	MessageBox(_T("��ǰ����ѳɹ����Ƶ������塣"), NULL, MB_ICONINFORMATION);
	//else
		MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnCopyAllLyric()
{
	// TODO: �ڴ���������������
	if (CCommon::CopyStringToClipboard(CPlayer::GetInstance().m_Lyrics.GetAllLyricText(theApp.m_ui_data.show_translate)))
		MessageBox(CCommon::LoadText(IDS_ALL_LRYIC_COPIED), NULL, MB_ICONINFORMATION);
	else
		MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	////˫����������������ģʽ
	//if (!theApp.m_ui_data.repetemode_rect.PtInRect(point) && !theApp.m_ui_data.volume_btn.rect.PtInRect(point)
	//	&& !theApp.m_ui_data.volume_up_rect.PtInRect(point) && !theApp.m_ui_data.volume_down_rect.PtInRect(point)
	//	&& !theApp.m_ui_data.translate_btn.rect.PtInRect(point))
	//	OnMiniMode();
	CMainDialogBase::OnLButtonDblClk(nFlags, point);
}


void CMusicPlayerDlg::OnLyricForward()
{
	// TODO: �ڴ���������������
	//�����ǰ0.5��
	CPlayer::GetInstance().m_Lyrics.AdjustLyric(-500);

}


void CMusicPlayerDlg::OnLyricDelay()
{
	// TODO: �ڴ���������������
	//����Ӻ�0.5��
	CPlayer::GetInstance().m_Lyrics.AdjustLyric(500);
}


void CMusicPlayerDlg::OnSaveModifiedLyric()
{
	// TODO: �ڴ���������������
	if (theApp.m_lyric_setting_data.save_lyric_in_offset && !CPlayer::GetInstance().m_Lyrics.IsChineseConverted())		//���ִ�������ķ���ת�����򱣴�ʱ����ѡ��������ζ�����SaveLyric2()
		CPlayer::GetInstance().m_Lyrics.SaveLyric();
	else
		CPlayer::GetInstance().m_Lyrics.SaveLyric2();
}


void CMusicPlayerDlg::OnEditLyric()
{
	// TODO: �ڴ���������������
	//ShellExecute(NULL, _T("open"), CPlayer::GetInstance().m_Lyrics.GetPathName().c_str(), NULL, NULL, SW_SHOWNORMAL);
	CCommon::DeleteModelessDialog(m_pLyricEdit);
	m_pLyricEdit = new CLyricEditDlg;
	m_pLyricEdit->Create(IDD_LYRIC_EDIT_DIALOG);
	m_pLyricEdit->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnDownloadLyric()
{
	// TODO: �ڴ���������������
	CLyricDownloadDlg aDlg;
	aDlg.DoModal();
}


void CMusicPlayerDlg::OnLyricBatchDownload()
{
	// TODO: �ڴ���������������
	//CLyricBatchDownloadDlg aDlg;
	//aDlg.DoModal();
	CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
	m_pLyricBatchDownDlg = new CLyricBatchDownloadDlg;
	m_pLyricBatchDownDlg->Create(IDD_LYRIC_BATCH_DOWN_DIALOG);
	m_pLyricBatchDownDlg->ShowWindow(SW_SHOW);

}


void CMusicPlayerDlg::OnDeleteLyric()
{
	// TODO: �ڴ���������������
	if (CCommon::FileExist(CPlayer::GetInstance().m_Lyrics.GetPathName()))
	{
		int rtn = CCommon::DeleteAFile(m_hWnd, CPlayer::GetInstance().m_Lyrics.GetPathName());		//ɾ������ļ�
		CPlayer::GetInstance().ClearLyric();		//�����ʹ���
	}
}


void CMusicPlayerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nFlags == MK_SHIFT)		//��סShift���������Ҽ�ʱ������ϵͳ�˵�
	{
		CPoint point1;
		GetCursorPos(&point1);
		m_main_popup_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	}
	else
	{
		m_pUI->RButtonUp(point);
	}

	CMainDialogBase::OnRButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pUI->MouseMove(point);

	CMainDialogBase::OnMouseMove(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pUI->LButtonUp(point);

	CMainDialogBase::OnLButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pUI->LButtonDown(point);

	CMainDialogBase::OnLButtonDown(nFlags, point);
}


HBRUSH CMusicPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CMainDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (pWnd == this /*|| pWnd == &m_path_static*/)
	{
		HBRUSH hBackBrush = CreateSolidBrush(CONSTVAL::BACKGROUND_COLOR);
		return hBackBrush;
	}

	return hbr;
}


afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam)
{
	theApp.DoWaitCursor(0);
	ShowPlayList();
	//ShowTime();
	DrawInfo(true);
	//SetPorgressBarSize();
	UpdatePlayPauseButton();
	//ShowTime();

	m_ui.UpdateSongInfoToolTip();
	m_ui2.UpdateSongInfoToolTip();

	EnablePlaylist(true);
	theApp.DoWaitCursor(-1);

	return 0;
}


afx_msg LRESULT CMusicPlayerDlg::OnSetTitle(WPARAM wParam, LPARAM lParam)
{
	CString title;
	title = CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str();

	CString title_suffix;
	if (!title.IsEmpty())
		title_suffix += _T(" - ");
	#ifdef _DEBUG
		title_suffix += CCommon::LoadText(_T("MusicPlayer+ "), IDS_DEBUG_MODE);
	#else
		title_suffix += _T("MusicPlayer2");
	#endif

	SetWindowText(title + title_suffix);		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���

	int title_length = 128 - title_suffix.GetLength() - 1;
	if(title.GetLength() > title_length)
		title = title.Left(title_length);
	m_notify_icon.SetIconToolTip(title + title_suffix);

	return 0;
}


void CMusicPlayerDlg::OnEqualizer()
{
	// TODO: �ڴ���������������
	CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
	m_pSoundEffecDlg = new CSoundEffectDlg;
	m_pSoundEffecDlg->Create(IDD_SOUND_EFFECT_DIALOG);
	m_pSoundEffecDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnExploreOnline()
{
	// TODO: �ڴ���������������
	m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)m_item_selected);
}

UINT CMusicPlayerDlg::ViewOnlineThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	//������������ת��������Ӧ�����������ֵ�����ҳ��
	int item_selected = (int)lpParam;
	if (item_selected >= 0 && item_selected < CPlayer::GetInstance().GetSongNum())
	{
		//���Ҹ�������ȡ���ƥ����ĸ���ID
		const SongInfo& song{ CPlayer::GetInstance().GetPlayList()[item_selected] };
		if (song.song_id.empty())		//���û�л�ȡ��ID�����ȡһ��ID
		{
			wstring song_id;
			song_id = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name).id;
			CPlayer::GetInstance().SetRelatedSongID(item_selected, song_id);
		}

		if (song.song_id.empty())
			return 0;
		//��ȡ�����������иø��������߽�����ַ
		wstring song_url{ L"http://music.163.com/#/song?id=" + song.song_id };

		//�򿪳�����
		ShellExecute(NULL, _T("open"), song_url.c_str(), NULL, NULL, SW_SHOW);
	}
	return 0;
}

UINT CMusicPlayerDlg::DownloadLyricAndCoverThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	//CMusicPlayerDlg* pThis = (CMusicPlayerDlg*)lpParam;
	const SongInfo& song{ CPlayer::GetInstance().GetCurrentSongInfo() };
	if (theApp.m_general_setting_data.auto_download_only_tag_full)		//�����ˡ�����������Ϣ���������Զ����ء�ʱ����������������������һ��Ϊ�գ����Զ�����
	{
		if ((song.title.empty() || song.title == CCommon::LoadText(IDS_DEFAULT_TITLE).GetString()) || (song.artist.empty() || song.artist == CCommon::LoadText(IDS_DEFAULT_ARTIST).GetString()))
			return 0;
	}

	bool download_cover{ theApp.m_general_setting_data.auto_download_album_cover && !CPlayer::GetInstance().AlbumCoverExist() && !CPlayer::GetInstance().GetCurrentSongInfo().is_cue };
	bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric };
	bool download_lyric{ theApp.m_general_setting_data.auto_download_lyric && CPlayer::GetInstance().m_Lyrics.IsEmpty() && !midi_lyric };
	CInternetCommon::ItemInfo match_item;
	if (download_cover || download_lyric)
	{
		if (song.song_id.empty())		//���û�л�ȡ��ID�����ȡһ��ID
		{
			//������������ȡ���ƥ�����Ŀ
			match_item = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name, false);
			CPlayer::GetInstance().SetRelatedSongID(match_item.id);
		}
		if (song.song_id.empty())
			return 0;
	}
	//�Զ�����ר������
	if (download_cover)
	{
// 		wstring cover_url = CCoverDownloadCommon::GetAlbumCoverURL(song.song_id);
		wstring cover_url = song.album_url;
		if (cover_url.empty())
		{
			return 0;
		}
	
		//��ȡҪ�����ר��������ļ�·��
		std::wstring album_file_path = CPlayer::GetInstance().GetAlbumPath();
		CFilePathHelper album_url(song.album_url);
		album_file_path = album_file_path + song.title + L"." + album_url.GetFileExtension();

		//����ר������
		URLDownloadToFile(0, song.album_url.c_str(), album_file_path.c_str(), 0, NULL);

		//�����ص�ר�������Ϊ��������
// 		SetFileAttributes(album_file_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

		//���´ӱ��ػ�ȡר������
// 		CPlayer::GetInstance().SearchOutAlbumCover();
		CPlayer::GetInstance().GetAlbumCover().Load(album_file_path.c_str());
		
		::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_ALBUM_COVER_DOWNLOAD_COMPLETE, 0, 0);
	}
	//�Զ����ظ��
	if (download_lyric)
	{
		//���ظ��
		wstring lyric_str;
		if (!CLyricDownloadCommon::DownloadKgLyric(song, lyric_str))
			return 0;
		if (!CLyricDownloadCommon::DisposeLryic(lyric_str))
			return 0;
		CLyricDownloadCommon::AddLyricTag(lyric_str, match_item.id, match_item.title, match_item.artist, match_item.album);
		//������
// 		CFilePathHelper lyric_path;
		wstring file_name;
		if (!song.is_cue)
			file_name = song.file_name;
		else
			file_name = song.artist + L" - " + song.title;
		//if (CCommon::FolderExist(theApp.m_lyric_setting_data.lyric_path))
		//{
		//	lyric_path.SetFilePath(theApp.m_lyric_setting_data.lyric_path + CPlayer::GetInstance().GetCurrentSongInfo().file_name);
		//}
		//else
		//{
// 		lyric_path.SetFilePath(CPlayer::GetInstance().GetCurrentDir() + file_name);
		//}
// 		lyric_path.ReplaceFileExtension(L"lrc");

		std::wstring lyric_path = CPlayer::GetInstance().GetLyricPath() + song.title + L".lrc";

		string _lyric_str = CCommon::UnicodeToStr(lyric_str, CodeType::UTF8);
		ofstream out_put{ lyric_path, std::ios::binary };
		out_put << _lyric_str;
		out_put.close();
		//�����ʷ���
		CLyrics lyrics{ lyric_path };		//�򿪱�����ĸ��
		lyrics.DeleteRedundantLyric();		//ɾ������ĸ��
		lyrics.CombineSameTimeLyric();		//����ʷ����ԭʼ��ʺϲ���һ��
		lyrics.SaveLyric2();

		CPlayer::GetInstance().IniLyrics(lyric_path);
	}
	return 0;
}



afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniStart(WPARAM wParam, LPARAM lParam)
{
	EnablePlaylist(false);
	theApp.DoWaitCursor(1);
	return 0;
}


void CMusicPlayerDlg::OnBrowseLyric()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().ExploreLyric();
}


void CMusicPlayerDlg::OnTranslateToSimplifiedChinese()
{
	// TODO: �ڴ���������������
	CWaitCursor wait_cursor;
	CPlayer::GetInstance().m_Lyrics.ChineseConvertion(true);
}


void CMusicPlayerDlg::OnTranslateToTranditionalChinese()
{
	// TODO: �ڴ���������������
	CWaitCursor wait_cursor;
	CPlayer::GetInstance().m_Lyrics.ChineseConvertion(false);
}


void CMusicPlayerDlg::OnAlbumCoverSaveAs()
{
	// TODO: �ڴ���������������
	//���ù�����
	CString szFilter = CCommon::LoadText(IDS_ALL_FILES, _T("(*.*)|*.*||"));
	//�������Ϊʱ��Ĭ���ļ���
	CString file_name;
	CString extension;
	if (CPlayer::GetInstance().IsInnerCover())
	{
		switch (CPlayer::GetInstance().GetAlbumCoverType())
		{
		case 0:
			extension = _T("jpg");
			break;
		case 1:
			extension = _T("png");
			break;
		case 2:
			extension = _T("gif");
			break;
		default: return;
		}
	}
	else
	{
		CFilePathHelper cover_path(CPlayer::GetInstance().GetAlbumCoverPath());
		extension = cover_path.GetFileExtension().c_str();
	}
	file_name.Format(_T("AlbumCover - %s - %s.%s"), CPlayer::GetInstance().GetCurrentSongInfo().artist.c_str(), CPlayer::GetInstance().GetCurrentSongInfo().album.c_str(), extension);
	wstring file_name_wcs{ file_name };
	CCommon::FileNameNormalize(file_name_wcs);		//�滻���ļ����е���Ч�ַ�
	//���챣���ļ��Ի���
	CFileDialog fileDlg(FALSE, _T("txt"), file_name_wcs.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	//��ʾ�����ļ��Ի���
	if (IDOK == fileDlg.DoModal())
	{
		CString dest_file = fileDlg.GetPathName();
		::CopyFile(CPlayer::GetInstance().GetAlbumCoverPath().c_str(), dest_file, FALSE);
		SetFileAttributes(dest_file, FILE_ATTRIBUTE_NORMAL);		//ȡ���ļ�����������
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnConnotPlayWarning(WPARAM wParam, LPARAM lParam)
{
	if (theApp.m_nc_setting_data.no_sf2_warning)
	{
		if (MessageBox(CCommon::LoadText(IDS_NO_MIDI_SF2_WARNING), NULL, MB_ICONWARNING | MB_OKCANCEL) == IDCANCEL)
			theApp.m_nc_setting_data.no_sf2_warning = false;
	}
	return 0;
}

#include <json/json.h>
#include <fmt/format.h>
#include "strcpcvt.h"
#include "restclient-cpp/restclient.h"
#include "downloader/HttpClient.h"
vector<SongInfo> g_song_info;
vector<KgSongInfo> g_kgsong_info;

std::string KG_SEARCH_API = "http://mobilecdn.kugou.com/api/v3/search/song?format=json&keyword={0}&page={1}&pagesize=20&showtype=1";
std::string KG_DOWN_API = "http://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash={0}";



void CMusicPlayerDlg::UpdateSearchList()
{
	vector<SongInfo>& songInfos = CPlayer::GetInstance().GetPlayList();

	CString key_word;
	m_search_edit.GetWindowText(key_word);

	songInfos.clear();
	std::string search_api, down_api;

	search_api = fmt::format(KG_SEARCH_API, cvt::ws2s(key_word.GetBuffer(0)), m_current_page_idx);

	RestClient::Response resp = RestClient::get(cvt::s2utf8(search_api));
	Json::Reader reader;
	Json::Value root, info, downinfo;
	std::string ansi_ret = cvt::utf8ts(resp.body);
	reader.parse(ansi_ret, root);

	Json::Value& data = root["data"];

	if (!data.empty()) {
		Json::Value& info = data["info"];
		if (!info.empty()) {
			for (unsigned int i = 0; i < info.size(); i++) {
				Json::Value song = info[i];
				SongInfo songInfo;
				songInfo.title = cvt::s2ws(song["filename"].asString());

				std::string sqhash;
				sqhash = song["sqhash"].asString();
				if (!sqhash.empty()) {
					down_api = fmt::format(KG_DOWN_API, sqhash);
					songInfo.kg_hash = cvt::s2ws(sqhash);
				}
				else {
					down_api = fmt::format(KG_DOWN_API, song["hash"].asString());
					songInfo.kg_hash = cvt::s2ws(downinfo["hash"].asString());
				}
				
				RestClient::Response resp2 = RestClient::get(down_api);
				std::string ansi_ret2 = cvt::utf8ts(resp2.body);
				reader.parse(ansi_ret2, downinfo);

				std::string album_url = downinfo["album_img"].asString();
				int bit_rate = downinfo["bitRate"].asInt();
				cvt::eraseSubStr(album_url, "{size}/");
				songInfo.album_url = cvt::s2ws(album_url);
				songInfo.play_url = cvt::s2ws(downinfo["url"].asString());
				songInfo.artist = cvt::s2ws(downinfo["singerName"].asString());
				songInfo.duration = downinfo["timeLength"].asInt() * 1000;
				songInfo.filesize = downinfo["fileSize"].asInt();
				songInfo.bitrate = downinfo["fileSize"].asInt();
				songInfos.push_back(songInfo);

			}
		}
	}
}

void CMusicPlayerDlg::OnEnChangeSearchEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CMainDialogBase::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 	CString key_word;
// 	m_search_edit.GetWindowText(key_word);
// 	m_searched = (key_word.GetLength() != 0);
// 
// 	m_playlist_list.QuickSearch(wstring(key_word));
// 	m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
}


void CMusicPlayerDlg::OnBnClickedClearSearchButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_searched)
	{
		//����������
		m_searched = false;
		m_search_edit.SetWindowText(_T(""));
		m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
		m_playlist_list.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);		//������������ȷ�����ڲ�����Ŀ�ɼ�
	}
}


void CMusicPlayerDlg::OnBnClickedSearchPrevPageButton()
{
	if (CPlayer::GetInstance().GetCurrentPageIndex() > 1)
		CPlayer::GetInstance().GetCurrentPageIndex()--;
	UpdateSearchList();
	m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
}


void CMusicPlayerDlg::OnBnClickedSearchNextPageButton()
{
	CPlayer::GetInstance().GetCurrentPageIndex()++;
	UpdateSearchList();
	m_playlist_list.ShowPlaylist(theApp.m_ui_data.display_format, m_searched);
}

void CMusicPlayerDlg::OnDownloadAlbumCover()
{
	// TODO: �ڴ���������������
	//_OnDownloadAlbumCover(true);
	CCoverDownloadDlg dlg;
	dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnMusicStreamOpened(WPARAM wParam, LPARAM lParam)
{
	//ר�������˹ģ�������ŵ�������Ϊ�˱���˺����ڹ����߳��б����ã��ڹ����߳��У�����ͼƬ�Ĵ���CDrawCommon::BitmapStretch��һ���ĸ��ʳ���ԭ��δ֪��
	CPlayer::GetInstance().AlbumCoverGaussBlur();
	//�Զ�����ר������
	m_pDownloadThread = AfxBeginThread(DownloadLyricAndCoverThreadFunc, this);
	return 0;
}


LRESULT CMusicPlayerDlg::OnPlaySelectedItem(WPARAM wParam, LPARAM lParam)
{
	int itemID = (int)(wParam);
	CPlayer::GetInstance().PlayTrack(itemID);
	SwitchTrack();
	UpdatePlayPauseButton();

	return 0;
}

void CMusicPlayerDlg::OnCurrentExploreOnline()
{
	// TODO: �ڴ���������������
	m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)CPlayer::GetInstance().GetIndex());
}


void CMusicPlayerDlg::OnDeleteAlbumCover()
{
	// TODO: �ڴ���������������
	CPlayer::GetInstance().DeleteAlbumCover();
}


void CMusicPlayerDlg::OnCopyFileTo()
{
	// TODO: �ڴ���������������
	LPCTSTR title{ CCommon::LoadText(IDS_SELECT_COPY_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(title);
#else
	CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
	folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
			return;
		if (m_items_selected.size() > 1)
		{
			vector<wstring> source_files;
			for (const auto& index : m_items_selected)
				source_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
			CCommon::CopyFiles(this->GetSafeHwnd(), source_files, wstring(folderPickerDlg.GetPathName()));
		}
		else
		{
			wstring source_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
			CCommon::CopyAFile(this->GetSafeHwnd(), CPlayer::GetInstance().GetCurrentFilePath(), wstring(folderPickerDlg.GetPathName()));
		}
	}
}


void CMusicPlayerDlg::OnMoveFileTo()
{
	// TODO: �ڴ���������������
	LPCTSTR title{ CCommon::LoadText(IDS_SELECT_MOVE_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(title);
#else
	CFilePathHelper current_path(CPlayer::GetInstance().GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
	folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
			return;
		wstring source_file;
		vector<wstring> source_files;
		int rtn;
		if (m_items_selected.size() > 1)
		{
			if (CCommon::IsItemInVector(m_items_selected, CPlayer::GetInstance().GetIndex()))	//���ѡ�е��ļ��������ڲ��ŵ��ļ������ƶ�ǰ�����ȹر��ļ�
				CPlayer::GetInstance().MusicControl(Command::CLOSE);
			for (const auto& index : m_items_selected)
				source_files.push_back(CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[index].file_name);
			rtn = CCommon::MoveFiles(m_hWnd, source_files, wstring(folderPickerDlg.GetPathName()));
		}
		else
		{
			if (m_item_selected == CPlayer::GetInstance().GetIndex())	//����ƶ����ļ������ڲ��ŵ��ļ������ƶ�ǰ�����ȹر��ļ�
				CPlayer::GetInstance().MusicControl(Command::CLOSE);
			source_file = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetPlayList()[m_item_selected].file_name;
			rtn = CCommon::MoveAFile(m_hWnd, source_file, wstring(folderPickerDlg.GetPathName()));
		}
		if (rtn == 0)
		{
			//����ļ��ƶ��ɹ���ͬʱ�Ӳ����б����Ƴ�
			if (m_items_selected.size() > 1)
				CPlayer::GetInstance().RemoveSongs(m_items_selected);
			else
				CPlayer::GetInstance().RemoveSong(m_item_selected);
			ShowPlayList();
			UpdatePlayPauseButton();
			DrawInfo(true);
		}
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnOpenFileCommandLine(WPARAM wParam, LPARAM lParam)
{
	wstring cmd_line = CCommon::GetStringFromClipboard();
	if (cmd_line.empty())
		return 0;
	vector<wstring> files;
	CCommon::DisposeCmdLineFiles(wstring(cmd_line), files);
	CPlayer::GetInstance().OpenFiles(files);
	return 0;
}


void CMusicPlayerDlg::OnFormatConvert()
{
	// TODO: �ڴ���������������
	if (!theApp.m_format_convert_dialog_exit)
		return;
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
	m_pFormatConvertDlg = new CFormatConvertDlg(m_items_selected);
	m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
	m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnFormatConvert1()
{
	// TODO: �ڴ���������������
	if (!theApp.m_format_convert_dialog_exit)
		return;
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);

	vector<int> items_selected;
	items_selected.push_back(CPlayer::GetInstance().GetIndex());
	m_pFormatConvertDlg = new CFormatConvertDlg(items_selected);
	m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
	m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


afx_msg LRESULT CMusicPlayerDlg::OnSettingsApplied(WPARAM wParam, LPARAM lParam)
{
	COptionsDlg* pOptionsDlg = (COptionsDlg*)wParam;
	if (pOptionsDlg == nullptr)
		return 0;
	ApplySettings(*pOptionsDlg);
	return 0;
}


void CMusicPlayerDlg::OnRecorder()
{
	// TODO: �ڴ���������������
	CRecorderDlg dlg;
	dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnAlbumCoverDownloadComplete(WPARAM wParam, LPARAM lParam)
{
	//���ڴ˺����ŵ��߳��д���ʱ������ͼƬ�Ĵ���CDrawCommon::BitmapStretch��һ���ĸ��ʳ���ԭ��δ֪
	//����ר�����汳���Ǻ�ɫ�ģ����ͨ��������Ϣ�ŵ����߳��д���
	CPlayer::GetInstance().AlbumCoverGaussBlur();
	
	if(theApp.m_nc_setting_data.show_cover_tip)
	{
		m_ui.UpdateSongInfoToolTip();
		m_ui2.UpdateSongInfoToolTip();
	}

	return 0;
}


void CMusicPlayerDlg::OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��Ӧ������ɫ�ı���Ϣ
	ThemeColorChanged();

	CMainDialogBase::OnColorizationColorChanged(dwColorizationColor, bOpacity);
}


void CMusicPlayerDlg::OnSupportedFormat()
{
	// TODO: �ڴ���������������
	CSupportedFormatDlg dlg;
	dlg.DoModal();
}


void CMusicPlayerDlg::OnSwitchUi()
{
	// TODO: �ڴ���������������

	if (m_pUI == &m_ui)
	{
		m_pUI = &m_ui2;
		m_ui.ClearBtnRect();
		m_ui.UpdateToolTipPosition();
	}
	else
	{
		m_pUI = &m_ui;
		m_ui2.ClearBtnRect();
		m_ui2.UpdateToolTipPosition();
	}

	DrawInfo(true);
	SetThumbnailClipArea();
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
	m_pUI->UpdateToolTipPosition();
}

void CMusicPlayerDlg::OnVolumeUp()
{
	if (m_miniModeDlg.m_hWnd == NULL)
		CPlayer::GetInstance().MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	else
		m_miniModeDlg.SetVolume(true);
}

void CMusicPlayerDlg::OnVolumeDown()
{
	if (m_miniModeDlg.m_hWnd == NULL)
		CPlayer::GetInstance().MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	else
		m_miniModeDlg.SetVolume(false);
}


afx_msg LRESULT CMusicPlayerDlg::OnNotifyicon(WPARAM wParam, LPARAM lParam)
{
	m_notify_icon.OnNotifyIcon(lParam, m_miniModeDlg.m_hWnd);

	if (lParam == WM_LBUTTONUP && m_miniModeDlg.m_hWnd == NULL)
	{
#ifndef COMPILE_IN_WIN_XP
		if (IsTaskbarListEnable())
		{
			m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//�����������������ͼ��ť
			SetThumbnailClipArea();		//������������������ͼ
		}
#endif
		UpdatePlayPauseButton();
	}

	return 0;
}


void CMusicPlayerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CMainDialogBase::OnClose();
}


void CMusicPlayerDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(theApp.m_general_setting_data.minimize_to_notify_icon)
		this->ShowWindow(HIDE_WINDOW);
	else
		CMainDialogBase::OnCancel();
}


void CMusicPlayerDlg::OnMenuExit()
{
	// TODO: �ڴ���������������
	if (m_miniModeDlg.m_hWnd == NULL)
	{
		CMainDialogBase::OnCancel();
	}
	else
	{
		::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, ID_MINI_MODE_EXIT, NULL);
	}
}


void CMusicPlayerDlg::OnMinimodeRestore()
{
	// TODO: �ڴ���������������
	if (m_miniModeDlg.m_hWnd == NULL)
	{
		OnMiniMode();
	}
	else
	{
		::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, IDOK, NULL);
	}
}


void CMusicPlayerDlg::OnAppCommand(CWnd* pWnd, UINT nCmd, UINT nDevice, UINT nKey)
{
	// �ù���Ҫ��ʹ�� Windows 2000 ����߰汾��
	// ���� _WIN32_WINNT �� WINVER ���� >= 0x0500��

	//��Ӧ��ý���
	if (!theApp.IsGlobalMultimediaKeyEnabled())	//���û��������Ӧȫ�ֵĶ�ý�尴����Ϣ�����ڵ�ǰ��������Ӧ��ý�尴����Ϣ
	{
		switch (nCmd)
		{
		case APPCOMMAND_MEDIA_PLAY_PAUSE:
			OnPlayPause();
			break;
		case APPCOMMAND_MEDIA_PREVIOUSTRACK:
			OnPrevious();
			break;
		case APPCOMMAND_MEDIA_NEXTTRACK:
			OnNext();
			break;
		case APPCOMMAND_MEDIA_STOP:
			OnStop();
			break;
		default:
			break;
		}
	}

	CMainDialogBase::OnAppCommand(pWnd, nCmd, nDevice, nKey);
}



void CMusicPlayerDlg::OnShowPlaylist()
{
	// TODO: �ڴ���������������
		m_pUI->ClearInfo();
		theApp.m_ui_data.show_playlist = !theApp.m_ui_data.show_playlist;

		OnSize(SIZE_RESTORED, theApp.m_ui_data.client_width, theApp.m_ui_data.client_height);
		SetPlaylistVisible();

		DrawInfo(true);
}


BOOL CMusicPlayerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pUI->SetCursor())
		return TRUE;
	else
	return CMainDialogBase::OnSetCursor(pWnd, nHitTest, message);
}


void CMusicPlayerDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pUI->MouseLeave();

	CMainDialogBase::OnMouseLeave();
}


void CMusicPlayerDlg::OnShowMenuBar()
{
	// TODO: �ڴ���������������
	theApp.m_ui_data.show_menu_bar = !theApp.m_ui_data.show_menu_bar;
	SetMenubarVisible();
	SetThumbnailClipArea();

	//���ز˵����󵯳���ʾ�������û�����ٴ���ʾ�˵���
	if (!theApp.m_ui_data.show_menu_bar)
	{
		if (theApp.m_nc_setting_data.show_hide_menu_bar_tip)
		{
			if (MessageBox(CCommon::LoadText(IDS_HIDE_MENU_BAR_TIP), NULL, MB_ICONINFORMATION | MB_OKCANCEL) == IDCANCEL)
				theApp.m_nc_setting_data.show_hide_menu_bar_tip = false;
		}
	}
}


void CMusicPlayerDlg::OnFullScreen()
{
	// TODO: �ڴ���������������

	if (m_miniModeDlg.m_hWnd != NULL)	//����ģʽ�²�������Ӧȫ����ʾ
		return;

	theApp.m_ui_data.full_screen = !theApp.m_ui_data.full_screen;
	SetFullScreen(theApp.m_ui_data.full_screen);
	DrawInfo(true);
	m_pUI->UpdateToolTipPosition();
	m_pUI->UpdateFullScreenTip();
	SetThumbnailClipArea();
}


void CMusicPlayerDlg::OnCreatePlayShortcut()
{
	// TODO: �ڴ���������������

	if (MessageBox(CCommon::LoadText(IDS_CREATE_PLAY_SHORTCUT_INFO), NULL, MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		//��������/��ͣ��ݷ�ʽ
		wstring play_pause = CCommon::LoadText(IDS_PLAY_PAUSE, L".lnk");
		CCommon::FileNameNormalize(play_pause);

		bool success = true;
		success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, play_pause.c_str(), NULL, 0, 0, 1, L"-play_pause", 2);

		//������һ����ݷ�ʽ
		success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_PREVIOUS, L".lnk"), NULL, 0, 0, 1, L"-previous", 1);

		//������һ����ݷ�ʽ
		success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_NEXT, L".lnk"), NULL, 0, 0, 1, L"-next", 3);

		//����ֹͣ��ݷ�ʽ
		success &= CCommon::CreateFileShortcut(theApp.m_module_dir.c_str(), NULL, CCommon::LoadText(IDS_STOP, L".lnk"), NULL, 0, 0, 1, L"-stop", 6);

		if (success)
			MessageBox(CCommon::LoadTextFormat(IDS_PLAY_SHORTCUT_CREATED, { theApp.m_module_dir }), NULL, MB_ICONINFORMATION);
		else
			MessageBox(CCommon::LoadText(IDS_SHORTCUT_CREAT_FAILED), NULL, MB_ICONWARNING);
	}
}


void CMusicPlayerDlg::OnListenStatistics()
{
	// TODO: �ڴ���������������
	CListenTimeStatisticsDlg dlg;
	dlg.DoModal();
}


void CMusicPlayerDlg::OnDarkMode()
{
	// TODO: �ڴ���������������
	theApp.m_app_setting_data.dark_mode = !theApp.m_app_setting_data.dark_mode;
	if (theApp.m_app_setting_data.dark_mode)
		theApp.m_app_setting_data.background_transparency = 50;
	else
		theApp.m_app_setting_data.background_transparency = 80;
	SaveConfig();
}


afx_msg LRESULT CMusicPlayerDlg::OnMainMenuPopup(WPARAM wParam, LPARAM lParam)
{
	CPoint point = *((CPoint*)wParam);
	ClientToScreen(&point);
	m_main_popup_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	return 0;
}



void CMusicPlayerDlg::OnAlwaysOnTop()
{
	// TODO: �ڴ���������������
	theApp.m_nc_setting_data.always_on_top = !theApp.m_nc_setting_data.always_on_top;
	SetAlwaysOnTop();
}


void CMusicPlayerDlg::OnFloatPlaylist()
{
	// TODO: �ڴ���������������

	theApp.m_ui_data.float_playlist = !theApp.m_ui_data.float_playlist;
	if(theApp.m_ui_data.float_playlist)
	{
		ShowFloatPlaylist();
	}
	else
	{
		HideFloatPlaylist();
	}
}
