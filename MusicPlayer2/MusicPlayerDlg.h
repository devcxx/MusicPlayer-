
// MusicPlayerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include"Player.h"
#include"PlayListCtrl.h"
#include"FindDlg.h"
#include"OptionsDlg.h"
#include"PropertyDlg.h"
#include"ProgressStatic.h"
#include"ColorConvert.h"
#include"MiniModeDlg.h"
#include"LyricEditDlg.h"
#include "LyricDownloadDlg.h"
#include "LyricBatchDownloadDlg.h"
//#include "EqualizerDlg.h"
#include "SoundEffectDlg.h"
#include "CortanaLyric.h"
#include "FilePathHelper.h"
#include "CoverDownloadCommon.h"
#include "CoverDownloadDlg.h"
#include "FormatConvertDlg.h"
#include "RecorderDlg.h"
#include "CPlayerUI.h"
#include "CPlayerUI2.h"
#include "CNotifyIcon.h"
#include "StaticEx.h"
#include "CMainDialogBase.h"
#include "CFloatPlaylistDlg.h"
#include "DownloadMgrDlg.h"
#define WM_ALBUM_COVER_DOWNLOAD_COMPLETE (WM_USER+114)		//自动下载专辑封面和歌词完成时发出的消息

// CMusicPlayerDlg 对话框
class CMusicPlayerDlg : public CMainDialogBase
{

// 构造
public:
	CMusicPlayerDlg(wstring cmdLine = wstring() ,CWnd* pParent = NULL);	// 标准构造函数
	~CMusicPlayerDlg();

	bool IsTaskbarListEnable() const;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICPLAYER2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CToolTipCtrl m_Mytip;
	//控件变量
	CPlayListCtrl m_playlist_list{ CPlayer::GetInstance().GetPlayList() };		//播放列表控件(初始化时通过构造函数传递歌曲信息的引用，
																	//用于支持鼠标指向列表中的项目时显示歌曲信息)
	CStaticEx m_path_static;
	CEdit m_path_edit;
	CMenu m_main_menu;				//菜单栏上的菜单
	CMenu m_list_popup_menu;		//播放列表右键菜单
	CMenu m_main_popup_menu;		//按住Shift键时弹出的右键菜单
	CButton m_set_path_button;
	CEdit m_search_edit;
	CButton m_search_prev_page_button;
	CButton m_search_next_page_button;
	CButton m_clear_search_button;
	int m_current_page_idx{1};

#ifndef COMPILE_IN_WIN_XP
	ITaskbarList3* m_pTaskbar{ theApp.GetITaskbarList3() };          //用于支持任务栏显示播放进度
	THUMBBUTTON m_thumbButton[3]{};
#endif

	CFindDlg m_findDlg{ CPlayer::GetInstance().GetPlayList() };		//查找对话框

	wstring m_cmdLine;	//命令行参数

	CDC* m_pDC;				//当前窗口的DC
	CPlayerUI m_ui{ theApp.m_ui_data, this };
	CPlayerUI2 m_ui2{ theApp.m_ui_data, this };
	IPlayerUI* m_pUI = nullptr;

	bool m_first_start{ true };		//初始时为true，在定时器第一次启动后置为flase

	int m_window_width;		//窗口的宽度
	int m_window_height;	//窗口的高度

	SLayoutData m_layout;		//窗口布局的固定数据



	bool m_searched;		//播放列表是否处于搜索状态

	unsigned int m_timer_count{};


	int m_item_selected{};		//播放列表中鼠标选中的项目
	vector<int> m_items_selected;
	int m_tab_selected{};		//选项设置中选择的标签

	CMiniModeDlg m_miniModeDlg{ m_item_selected, m_items_selected, m_list_popup_menu, m_main_popup_menu };		//迷你模式对话框

	CCortanaLyric m_cortana_lyric;		//用于显示Cortana歌词

	CLyricEditDlg* m_pLyricEdit;		//歌词编辑对话框（非模态对话框）
	CLyricBatchDownloadDlg* m_pLyricBatchDownDlg;	//歌词批量下载对话框（非模态对话框）
	CSetPathDlg* m_pSetPathDlg;		//选择路径对话框（非模态对话框）
	CSoundEffectDlg* m_pSoundEffecDlg;		//音效设定对话框（非模态对话框）
	CFormatConvertDlg* m_pFormatConvertDlg;		//格式转换对话框（非模态对话框）
	CFloatPlaylistDlg* m_pFloatPlaylistDlg;
	CDownloadMgrDlg* m_pDownloadMgrDlg;

	CWinThread* m_pThread;		//执行在线查看的线程
	static UINT ViewOnlineThreadFunc(LPVOID lpParam);	//执行在线查看的线程函数

	CWinThread* m_pDownloadThread;		//执行自动下载歌词和专辑封面的线程
	static UINT DownloadLyricAndCoverThreadFunc(LPVOID lpParam);	//执行自动下载歌词和专辑封面的线程函数

	int m_play_error_cnt{};		//统计播放出错的次数

	CNotifyIcon m_notify_icon;

private:
	void SaveConfig();		//保存设置到ini文件
	void LoadConfig();		//从ini文件读取设置
	void SetTransparency();			//根据m_transparency的值设置窗口透明度
	void DrawInfo(bool reset = false);		//绘制信息
	void SetPlaylistSize(int cx, int cy);		//设置播放列表的大小
	void SetAlwaysOnTop();

public:
	void ShowPlayList();

	void UpdateSearchList();

protected:
	void SetPlayListColor();
	void SwitchTrack();		//当切换正在播放的歌曲时的处理
	void SetPlaylistVisible();
	void SetMenubarVisible();

	void UpdateTaskBarProgress();	//更新任务栏按钮上的进度
	void UpdatePlayPauseButton();		//根据当前播放状态更新“播放/暂停”按钮上的文字和图标
	void SetThumbnailClipArea();		//设置任务栏缩略图的区域
	void EnablePlaylist(bool enable);		//设置启用或禁用播放列表控件

	void CreateDesktopShortcut();		//用于在提示用户创建桌面快捷方式

	void ApplySettings(const COptionsDlg& optionDlg);		//从选项设置对话框获取设置
	void ApplyThemeColor();			//应用主题颜色设置

	void ThemeColorChanged();

	void SetMenuState(CMenu* pMenu);

	void ShowFloatPlaylist();
	void HideFloatPlaylist();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPlayPause();
	afx_msg void OnStop();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnRew();
	afx_msg void OnFF();
	afx_msg void OnSetPath();
	afx_msg void OnDownMgr();
	afx_msg void OnFind();
	afx_msg void OnExplorePath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenFolder();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnPlayOrder();
	afx_msg void OnPlayShuffle();
	afx_msg void OnLoopPlaylist();
	afx_msg void OnLoopTrack();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnBnClickedVolumeUp();
	//afx_msg void OnBnClickedVolumeDown();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnRefreshPlaylist();
	afx_msg void OnOptionSettings();
	afx_msg void OnReloadPlaylist();
	afx_msg void OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPlayItem();
	afx_msg void OnDownloadItem();
	afx_msg void OnItemProperty();
	//afx_msg void OnRemoveFromPlaylist();
//	afx_msg void OnClearPlaylist();
	afx_msg void OnExploreTrack();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	//afx_msg void OnStnClickedProgressStatic();
	afx_msg void OnReIniBass();
	afx_msg void OnSortByFile();
	afx_msg void OnSortByTitle();
	afx_msg void OnSortByArtist();
	afx_msg void OnSortByAlbum();
	afx_msg void OnSortByTrack();
public:
	afx_msg void OnDeleteFromDisk();
protected:
	afx_msg LRESULT OnTaskbarcreated(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDispFileName();
	afx_msg void OnDispTitle();
	afx_msg void OnDispArtistTitle();
	afx_msg void OnDispTitleArtist();
	afx_msg void OnMiniMode();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedPlayPause();
	afx_msg void OnBnClickedNext();
//	afx_msg void OnMove(int x, int y);
	afx_msg void OnReloadLyric();
	afx_msg void OnSongInfo();
	afx_msg void OnCopyCurrentLyric();
	afx_msg void OnCopyAllLyric();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLyricForward();
	afx_msg void OnLyricDelay();
	afx_msg void OnSaveModifiedLyric();
	afx_msg void OnEditLyric();
	afx_msg void OnDownloadLyric();
	afx_msg void OnLyricBatchDownload();
	afx_msg void OnDeleteLyric();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	afx_msg LRESULT OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTitle(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEqualizer();
	afx_msg void OnExploreOnline();
protected:
	afx_msg LRESULT OnPlaylistIniStart(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBrowseLyric();
	afx_msg void OnTranslateToSimplifiedChinese();
	afx_msg void OnTranslateToTranditionalChinese();
	afx_msg void OnAlbumCoverSaveAs();
protected:
	afx_msg LRESULT OnPathSelected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConnotPlayWarning(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChangeSearchEdit();
	afx_msg void OnBnClickedClearSearchButton();
	afx_msg void OnBnClickedSearchPrevPageButton();
	afx_msg void OnBnClickedSearchNextPageButton();
	afx_msg void OnDownloadAlbumCover();
protected:
	afx_msg LRESULT OnMusicStreamOpened(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlaySelectedItem(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnCurrentExploreOnline();
	afx_msg void OnDeleteAlbumCover();
	afx_msg void OnCopyFileTo();
	afx_msg void OnMoveFileTo();
protected:
	afx_msg LRESULT OnOpenFileCommandLine(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnFormatConvert();
	afx_msg void OnFormatConvert1();
protected:
	afx_msg LRESULT OnSettingsApplied(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRecorder();
protected:
	afx_msg LRESULT OnAlbumCoverDownloadComplete(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity);
	afx_msg void OnSupportedFormat();
	afx_msg void OnSwitchUi();
	afx_msg void OnVolumeUp();
	afx_msg void OnVolumeDown();
protected:
	afx_msg LRESULT OnNotifyicon(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnMenuExit();
	afx_msg void OnMinimodeRestore();
	afx_msg void OnAppCommand(CWnd* pWnd, UINT nCmd, UINT nDevice, UINT nKey);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowPlaylist();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseLeave();
	afx_msg void OnShowMenuBar();
	afx_msg void OnFullScreen();
	afx_msg void OnCreatePlayShortcut();
	afx_msg void OnListenStatistics();
	afx_msg void OnDarkMode();
protected:
	afx_msg LRESULT OnMainMenuPopup(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnAlwaysOnTop();
	afx_msg void OnFloatPlaylist();
};
