
// MusicPlayer2.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "MessageDlg.h"
#include "SimpleXML.h"
#include "crashtool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicPlayerApp

BEGIN_MESSAGE_MAP(CMusicPlayerApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_HELP, &CMusicPlayerApp::OnHelp)
END_MESSAGE_MAP()


// CMusicPlayerApp ����

CMusicPlayerApp::CMusicPlayerApp()
{
	// ֧����������������
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;		//����Ҫ֧����������������

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	CRASHREPORT::StartCrashReport();

	//��ʼ��·��
	m_module_dir = CCommon::GetExePath();
#ifdef _DEBUG
	m_local_dir = L".\\";
#else
	m_local_dir = m_module_dir;
#endif // _DEBUG

	m_config_path = m_module_dir + L"config.ini";
	m_song_data_path = m_module_dir + L"song_data.dat";
	m_recent_path_dat_path = m_module_dir + L"recent_path.dat";
	m_desktop_path = CCommon::GetDesktopPath();
	//m_temp_path = CCommon::GetTemplatePath() + L"MusicPlayer2\\";


	//��ȡ��ǰDPI
	HDC hDC = ::GetDC(NULL);
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
	::ReleaseDC(NULL, hDC);
	if (m_dpi == 0)
	{
		WriteErrorLog(L"Get system DPI failed!");
		m_dpi = 96;
	}
}


// Ψһ��һ�� CMusicPlayerApp ����

CMusicPlayerApp theApp;


// CMusicPlayerApp ��ʼ��

BOOL CMusicPlayerApp::InitInstance()
{
	//�滻���Ի�������Ĭ������
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);	//MFCĬ�ϵ����жԻ��������Ϊ#32770
	wc.lpszClassName = _T("MusicPlayer_l3gwYT");	//���Ի���������޸�Ϊ������
	AfxRegisterClass(&wc);

	wstring cmd_line{ m_lpCmdLine };
	//������Windows��������ʱ��ֱ���˳�����
	if (cmd_line.find(L"RestartByRestartManager") != wstring::npos)
	{
		//�������в���д����־�ļ�
		CString info = CCommon::LoadTextFormat(IDS_RESTART_EXIT, { cmd_line });
		//swprintf_s(buff, CCommon::LoadText(IDS_RESTART_EXIT), cmd_line.c_str());
		WriteErrorLog(wstring{ info.GetString() });
		return FALSE;
	}

	ControlCmd cmd;
	bool cmd_control = CCommon::GetCmdLineCommand(cmd_line, cmd);		//�����в����Ƿ������������
	if (cmd_control)		//����������в���������������������в������
		cmd_line.clear();

	//����Ƿ�����ʵ���������У�Debugʱ����飩
#ifndef _DEBUG
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("bXS1E7joK0Kh"));		//ʹ��һ��������ַ�������һ��������
	if (hMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)		//����������ʧ�ܣ�˵���Ѿ���һ�������ʵ����������
		{
			//AfxMessageBox(_T("�Ѿ���һ�������������С�"));
			HWND handle = FindWindow(_T("MusicPlayer_l3gwYT"), NULL);		//������������������ʵ�����ڵľ��
			if (handle != NULL)
			{
				HWND minidlg_handle = FindWindow(_T("MiniDlg_ByH87M"), NULL);
				if (!cmd_control)
				{
					if (minidlg_handle == NULL)			//û���ҵ�������ģʽ�����ڣ��򼤻�������
					{
						ShowWindow(handle, SW_SHOWNORMAL);		//�����ʾ����
						SetForegroundWindow(handle);		//����������Ϊ����
					}
					else				//�ҵ��ˡ�����ģʽ�����ڣ��򼤻����ģʽ������
					{
						ShowWindow(minidlg_handle, SW_SHOWNORMAL);
						SetForegroundWindow(minidlg_handle);
					}
				}

				if (cmd_control)
				{
					switch (cmd)
					{
					case ControlCmd::PLAY_PAUSE:
						::SendMessage(handle, WM_COMMAND, ID_PLAY_PAUSE, 0);
						break;
					case ControlCmd::_PREVIOUS:
						::SendMessage(handle, WM_COMMAND, ID_PREVIOUS, 0);
						break;
					case ControlCmd::_NEXT:
						::SendMessage(handle, WM_COMMAND, ID_NEXT, 0);
						break;
					case ControlCmd::STOP:
						::SendMessage(handle, WM_COMMAND, ID_STOP, 0);
						break;
					case ControlCmd::FF:
						::SendMessage(handle, WM_COMMAND, ID_FF, 0);
						break;
					case ControlCmd::REW:
						::SendMessage(handle, WM_COMMAND, ID_REW, 0);
						break;
					case ControlCmd::VOLUME_UP:
						::SendMessage(handle, WM_COMMAND, ID_VOLUME_UP, 0);
						break;
					case ControlCmd::VOLUME_DOWM:
						::SendMessage(handle, WM_COMMAND, ID_VOLUME_DOWN, 0);
						break;

					default:
						break;
					}
				}

				if (!cmd_line.empty())		//���ͨ�������д����˴򿪵��ļ�����������һ�����������У��򽫴��ļ�������������в������ݸ��ý���
				{
					CCommon::CopyStringToClipboard(cmd_line);		//�������в������Ƶ������壬ͨ���������ڲ�ͬ���̼��ַ���
					::SendMessage(handle, WM_OPEN_FILE_COMMAND_LINE, 0, 0);
				}
				return FALSE;		//�˳���ǰ����
			}
		}
	}
#endif

	//CString str = CCommon::LoadTextFormat(IDS_TEST_STR, { 3, L"asdfghhh", 1.2 });

	LoadSongData();
	LoadConfig();

	//��ʼ����������
	CCommon::SetThreadLanguage(m_general_setting_data.language);

	//���bass.dll�İ汾�Ƿ��API�İ汾ƥ��
	WORD dll_version{ HIWORD(BASS_GetVersion()) };
	//WORD dll_version{ 0x203 };
	if (dll_version != BASSVERSION)
	{
		CString info = CCommon::LoadTextFormat(IDS_BASS_VERSION_WARNING, { HIBYTE(dll_version), LOBYTE(dll_version), HIBYTE(BASSVERSION), LOBYTE(BASSVERSION) });
		if (AfxMessageBox(info, MB_ICONWARNING | MB_OKCANCEL) == IDCANCEL)
			return FALSE;
	}

	//����ʱ������
#ifndef _DEBUG		//DEBUG�²�������ʱ������
	if (m_general_setting_data.check_update_when_start)
	{
		AfxBeginThread(CheckUpdateThreadFunc, NULL);
	}
#endif // !_DEBUG


	CColorConvert::Initialize();

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//����һ��ȫ�ֹ����Խػ��ý�尴����Ϣ
	if(m_hot_key_setting_data.global_multimedia_key_enable)
		m_multimedia_key_hook = SetWindowsHookEx(WH_KEYBOARD_LL, CMusicPlayerApp::MultiMediaKeyHookProc, m_hInstance, 0);

	CMusicPlayerDlg dlg(cmd_line);
	//CMusicPlayerDlg dlg(L"D:\\����2\\Test\\1.wma \"D:\\����2\\Test\\Sweety - ӣ����.mp3\"");
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	SaveSongData();

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	if (m_multimedia_key_hook != NULL)
	{
		UnhookWindowsHookEx(m_multimedia_key_hook);
		m_multimedia_key_hook = NULL;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



void CMusicPlayerApp::OnHelp()
{
	// TODO: �ڴ���������������
	static bool dialog_exist{ false };

	if(!dialog_exist)		//ȷ���Ի����Ѿ�����ʱ���ٵ���
	{
		dialog_exist = true;
		CMessageDlg helpDlg;
		helpDlg.SetWindowTitle(CCommon::LoadText(IDS_HELP));
		helpDlg.SetInfoText(CCommon::LoadText(IDS_WELCOM_TO_USE));

		CString info{ GetHelpString() };
		info += _T("\r\n\r\n");
		info += GetSystemInfoString();

		helpDlg.SetMessageText(info);
		helpDlg.DoModal();

		dialog_exist = false;
	}
}

void CMusicPlayerApp::SaveSongData()
{
	// �򿪻����½��ļ�
	CFile file;
	BOOL bRet = file.Open(m_song_data_path.c_str(),
		CFile::modeCreate | CFile::modeWrite);
	if (!bRet)		//���ļ�ʧ��
	{
		return;
	}
	// ����CArchive����
	CArchive ar(&file, CArchive::store);
	// д����
	ar << CString(VERSION);			//д��汾��
	ar << m_song_data.size();		//д��ӳ�������Ĵ�С
	for (auto& song_data : m_song_data)
	{
		ar << CString(song_data.first.c_str())		//����ӳ�������ļ����������ľ���·��
			//<< CString(song_data.second.lyric_file.c_str())
			<< song_data.second.lengh.time2int()
			<< song_data.second.bitrate
			<< CString(song_data.second.title.c_str())
			<< CString(song_data.second.artist.c_str())
			<< CString(song_data.second.album.c_str())
			<< CString(song_data.second.year.c_str())
			<< CString(song_data.second.comment.c_str())
			<< CString(song_data.second.genre.c_str())
			<< song_data.second.genre_idx
			<< song_data.second.track
			<< song_data.second.tag_type
			<< CString(song_data.second.song_id.c_str())
			<< song_data.second.listen_time
			<< song_data.second.info_acquired
			;
	}
	// �ر�CArchive����
	ar.Close();
	// �ر��ļ�
	file.Close();

	m_song_data_modified = false;
}

void CMusicPlayerApp::CheckUpdate(bool message)
{
	CWaitCursor wait_cursor;
	wstring version_info;
	if (!CInternetCommon::GetURL(L"https://raw.githubusercontent.com/zhongyang219/MusicPlayer2/master/version.info", version_info))		//��ȡ�汾��Ϣ
	{
		if (message)
			AfxMessageBox(CCommon::LoadText(IDS_CHECK_UPDATA_FAILED), MB_OK | MB_ICONWARNING);
		return;
	}

	wstring version;		//����汾
	wstring link;			//��������
	CString contents_zh_cn;	//�������ݣ��������ģ�
	CString contents_en;	//�������ݣ�English��
	CSimpleXML version_xml;
	version_xml.LoadXMLContentDirect(version_info);

	version = version_xml.GetNode(L"version");
#ifdef _M_X64
	link = version_xml.GetNode(L"link_x64");
#else
	link = version_xml.GetNode(L"link");
#endif
	contents_zh_cn = version_xml.GetNode(L"contents_zh_cn", L"update_contents").c_str();
	contents_en = version_xml.GetNode(L"contents_en", L"update_contents").c_str();
	contents_zh_cn.Replace(L"\\n", L"\r\n");
	contents_en.Replace(L"\\n", L"\r\n");

	if (version.empty() || link.empty())
	{
		if (message)
			theApp.m_pMainWnd->MessageBox(CCommon::LoadText(IDS_CHECK_UPDATA_ERROR), NULL, MB_OK | MB_ICONWARNING);
		return;
	}
	if (version > VERSION)		//����������ϵİ汾���ڱ��ذ汾
	{
		CString info;
		//������������ѡ���Ӧ���԰汾�ĸ�������
		int language_code = _ttoi(CCommon::LoadText(IDS_LANGUAGE_CODE));
		CString contents_lan;
		switch (language_code)
		{
		case 2: contents_lan = contents_zh_cn; break;
		default: contents_lan = contents_en; break;
		}

		if (contents_lan.IsEmpty())
			info = CCommon::LoadTextFormat(IDS_UPDATE_AVLIABLE, { version });
		else
			info = CCommon::LoadTextFormat(IDS_UPDATE_AVLIABLE2, { version, contents_lan });

		if (theApp.m_pMainWnd->MessageBox(info, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			ShellExecute(NULL, _T("open"), link.c_str(), NULL, NULL, SW_SHOW);		//ת����������
		}
	}
	else
	{
		if (message)
			theApp.m_pMainWnd->MessageBox(CCommon::LoadText(IDS_ALREADY_UPDATED), NULL, MB_OK | MB_ICONINFORMATION);
	}
}

UINT CMusicPlayerApp::CheckUpdateThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	CheckUpdate(false);		//������
	return 0;
}

void CMusicPlayerApp::SaveConfig()
{
	CIniHelper ini(m_config_path);
	ini.WriteBool(L"general", L"check_update_when_start", m_general_setting_data.check_update_when_start);
	ini.WriteInt(_T("general"), _T("language"), static_cast<int>(m_general_setting_data.language));
	ini.WriteBool(L"hot_key", L"global_multimedia_key_enable", m_hot_key_setting_data.global_multimedia_key_enable);
	ini.Save();
}

void CMusicPlayerApp::LoadConfig()
{
	CIniHelper ini(m_config_path);
	m_general_setting_data.check_update_when_start = ini.GetBool(L"general", L"check_update_when_start", true);
	m_general_setting_data.language = static_cast<Language>(ini.GetInt(L"general", L"language", 0));
	m_hot_key_setting_data.global_multimedia_key_enable = ini.GetBool(L"hot_key", L"global_multimedia_key_enable", false);
}

void CMusicPlayerApp::LoadIconResource()
{
	m_icon_set.default_cover.Load(IDI_DEFAULT_COVER, NULL, 512);
	m_icon_set.default_cover_small.Load(IDI_DEFAULT_COVER, NULL, DPI(32));
	m_icon_set.skin.Load(IDI_SKIN, IDI_SKIN_D, DPI(16));
	m_icon_set.eq.Load(IDI_EQ, IDI_EQ_D, DPI(16));
	m_icon_set.setting.Load(IDI_SETTING, IDI_SETTING_D, DPI(16));
	m_icon_set.mini.Load(IDI_MINI, IDI_MINI_D, DPI(16));
	m_icon_set.play_oder.Load(IDI_PLAY_ORDER, IDI_PLAY_ORDER_D, DPI(16));
	m_icon_set.play_shuffle.Load(IDI_PLAY_SHUFFLE, IDI_PLAY_SHUFFLE_D, DPI(16));
	m_icon_set.loop_playlist.Load(IDI_LOOP_PLAYLIST, IDI_LOOP_PLAYLIST_D, DPI(16));
	m_icon_set.loop_track.Load(IDI_LOOP_TRACK, IDI_LOOP_TRACK_D, DPI(16));
	m_icon_set.info.Load(IDI_SONG_INFO, IDI_SONG_INFO_D, DPI(16));
	m_icon_set.select_folder.Load(IDI_SELECT_FOLDER, IDI_SELECT_FOLDER_D, DPI(16));
	m_icon_set.show_playlist.Load(IDI_PLAYLIST, IDI_PLAYLIST_D, DPI(16));
	m_icon_set.find_songs.Load(IDI_FIND_SONGS, IDI_FIND_SONGS_D, DPI(16));
	m_icon_set.full_screen.Load(IDI_FULL_SCREEN, IDI_FULL_SCREEN_D, DPI(16));
	m_icon_set.full_screen1.Load(IDI_FULL_SCREEN1, IDI_FULL_SCREEN1_D, DPI(16));
	m_icon_set.menu.Load(IDI_MENU, IDI_MENU_D, DPI(16));

	m_icon_set.previous.Load(IDI_PREVIOUS, NULL, DPI(16));
	m_icon_set.play.Load(IDI_PLAY, NULL, DPI(16));
	m_icon_set.pause.Load(IDI_PAUSE, NULL, DPI(16));
	m_icon_set.next.Load(IDI_NEXT1, NULL, DPI(16));

	m_icon_set.stop_l.Load(IDI_STOP, NULL, DPI(20));
	m_icon_set.previous_l.Load(IDI_PREVIOUS, NULL, DPI(20));
	m_icon_set.play_l.Load(IDI_PLAY, NULL, DPI(20));
	m_icon_set.pause_l.Load(IDI_PAUSE, NULL, DPI(20));
	m_icon_set.next_l.Load(IDI_NEXT1, NULL, DPI(20));

}

int CMusicPlayerApp::DPI(int pixel)
{
	return (m_dpi*(pixel) / 96);
}

int CMusicPlayerApp::DPI(double pixel)
{
	return static_cast<int>(m_dpi*(pixel) / 96);
}

int CMusicPlayerApp::DPIRound(double pixel, double round)
{
	double rtn;
	rtn = static_cast<double>(m_dpi)*pixel / 96;
	rtn += round;
	return static_cast<int>(rtn);
}

void CMusicPlayerApp::GetDPIFromWindow(CWnd * pWnd)
{
	CWindowDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
}

WORD CMusicPlayerApp::GetCurrentLanguage() const
{
	switch (m_general_setting_data.language)
	{
	case Language::SIMPLIFIED_CHINESE:
		return MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	default:
		return MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	}
}

bool CMusicPlayerApp::IsGlobalMultimediaKeyEnabled() const
{
	return m_multimedia_key_hook != NULL;
}

CString CMusicPlayerApp::GetHelpString()
{
	CString help_info;
	HRSRC hRes;
	if (m_general_setting_data.language == Language::FOLLOWING_SYSTEM)
		hRes = FindResource(NULL, MAKEINTRESOURCE(IDR_TEXT1), _T("TEXT"));
	else
		hRes = FindResourceEx(NULL, _T("TEXT"), MAKEINTRESOURCE(IDR_TEXT1), GetCurrentLanguage());
	if (hRes != NULL)
	{
		HGLOBAL hglobal = LoadResource(NULL, hRes);
		if (hglobal != NULL)
			help_info.Format(_T("%s"), (LPVOID)hglobal);
	}

	return help_info;
}

CString CMusicPlayerApp::GetSystemInfoString()
{
	CString info;
	info += _T("System Info:\r\n");

	CString strTmp;
	strTmp.Format(_T("Windows Version: %d.%d build %d\r\n"), CWinVersionHelper::GetMajorVersion(),
		CWinVersionHelper::GetMinorVersion(), CWinVersionHelper::GetBuildNumber());
	info += strTmp;

	strTmp.Format(_T("DPI: %d"), GetDPI());
	info += strTmp;
	info += _T("\r\n");

	return info;
}

void CMusicPlayerApp::SetSongDataModified()
{
	m_song_data_modified = true;
}

bool CMusicPlayerApp::IsSongDataModified() const
{
	return m_song_data_modified;
}

void CMusicPlayerApp::WriteErrorLog(const wstring & log_str)
{
	CCommon::WriteLog((m_module_dir + L"error.log").c_str(), log_str);
}

void CMusicPlayerApp::LoadSongData()
{
	// ���ļ�
	CFile file;
	BOOL bRet = file.Open(m_song_data_path.c_str(), CFile::modeRead);
	if (!bRet) return;
	// ����CArchive����
	CArchive ar(&file, CArchive::load);
	// ������
	size_t size{};
	SongInfo song_info;
	CString song_path;
	CString temp;
	int song_length;
	try
	{
		//��ȡ�汾
		CString version_str;
		ar >> version_str;
		double version = _ttof(version_str);
		ar >> size;		//��ȡӳ�������ĳ���
		for (size_t i{}; i < size; i++)
		{
			ar >> song_path;
			//ar >> temp;
			//song_info.lyric_file = temp;
			ar >> song_length;
			song_info.lengh.int2time(song_length);
			ar >> song_info.bitrate;
			ar >> temp;
			song_info.title = temp;
			ar >> temp;
			song_info.artist = temp;
			ar >> temp;
			song_info.album = temp;
			ar >> temp;
			song_info.year = temp;
			ar >> temp;
			song_info.comment = temp;
			ar >> temp;
			song_info.genre = temp;
			ar >> song_info.genre_idx;
			ar >> song_info.track;
			ar >> song_info.tag_type;
			ar >> temp;
			song_info.song_id = temp;

			if(version >= 2.63999)		//�汾�Ŵ��ڵ���2.64
			{
				ar >> song_info.listen_time;
				ar >> song_info.info_acquired;
			}
			m_song_data[wstring{ song_path }] = song_info;		//����ȡ����һ�׸�����Ϣ��ӵ�ӳ��������
		}
	}
	catch(CArchiveException* exception)
	{
		CString info;
		info = CCommon::LoadTextFormat(IDS_SERIALIZE_ERROR, { exception->m_cause });
		WriteErrorLog(wstring{ info });
	}
	// �رն���
	ar.Close();
	// �ر��ļ�
	file.Close();
}

LRESULT CMusicPlayerApp::MultiMediaKeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
 //	if (wParam == HSHELL_APPCOMMAND)
	//{
	//	int a = 0;
	//}

	//�ػ�ȫ�ֵĶ�ý�尴����Ϣ
	if (wParam == WM_KEYUP)
	{
		KBDLLHOOKSTRUCT* pKBHook = (KBDLLHOOKSTRUCT*)lParam;
		switch (pKBHook->vkCode)
		{
		case VK_MEDIA_PLAY_PAUSE:
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, ID_PLAY_PAUSE, 0);
			return TRUE;
		case VK_MEDIA_PREV_TRACK:
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, ID_PREVIOUS, 0);
			return TRUE;
		case VK_MEDIA_NEXT_TRACK:
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, ID_NEXT, 0);
			return TRUE;
		case VK_MEDIA_STOP:
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_COMMAND, ID_STOP, 0);
			return TRUE;
		default:
			break;
		}
	}

	CallNextHookEx(theApp.m_multimedia_key_hook, nCode, wParam, lParam);

	return LRESULT();
}

