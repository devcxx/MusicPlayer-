// MiniModeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MiniModeDlg.h"
#include "afxdialogex.h"


// CMiniModeDlg �Ի���

IMPLEMENT_DYNAMIC(CMiniModeDlg, CDialogEx)

CMiniModeDlg::CMiniModeDlg(int& item_selected, vector<int>& items_selected, CMenu& popup_menu, CMenu& main_popup_menu, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MINI_DIALOG, pParent), m_item_selected{ item_selected }, m_items_selected{ items_selected },
	m_popup_menu{ popup_menu }, m_main_popup_menu{ main_popup_menu }
{
	m_screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
	m_screen_height = GetSystemMetrics(SM_CYFULLSCREEN) + 23;

	//���Ĵ��ڵ�����
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("MiniDlg_ByH87M");
	AfxRegisterClass(&wc);
}

CMiniModeDlg::~CMiniModeDlg()
{
}

void CMiniModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_playlist_ctrl);
}

void CMiniModeDlg::SaveConfig() const
{
	CIniHelper ini(theApp.m_config_path);
	ini.WriteInt(L"mini_mode", L"position_x", m_position_x);
	ini.WriteInt(L"mini_mode", L"position_y", m_position_y);
	ini.Save();
}

void CMiniModeDlg::LoadConfig()
{
	CIniHelper ini(theApp.m_config_path);
	m_position_x = ini.GetInt(L"mini_mode", L"position_x", -1);
	m_position_y = ini.GetInt(_T("mini_mode"), _T("position_y"), -1);
}

void CMiniModeDlg::CheckWindowPos()
{
	CRect rect;
	GetWindowRect(rect);
	if (rect.left < 0)
	{
		rect.MoveToX(0);
		MoveWindow(rect);
	}
	if (rect.top < 0)
	{
		rect.MoveToY(0);
		MoveWindow(rect);
	}
	if (rect.right > m_screen_width)
	{
		rect.MoveToX(m_screen_width - rect.Width());
		MoveWindow(rect);
	}
	if (rect.bottom > m_screen_height)
	{
		rect.MoveToY(m_screen_height - rect.Height());
		MoveWindow(rect);
	}
}

void CMiniModeDlg::UpdateSongTipInfo()
{
	CString song_tip_info;
	song_tip_info += CCommon::LoadText(IDS_NOW_PLAYING, _T(": "));
	song_tip_info += CPlayer::GetInstance().GetFileName().c_str();
	song_tip_info += _T("\r\n");
	song_tip_info += CCommon::LoadText(IDS_TITLE, _T(": "));
	song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].title.c_str();
	song_tip_info += _T("\r\n");
	song_tip_info += CCommon::LoadText(IDS_ARTIST, _T(": "));
	song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].artist.c_str();
	song_tip_info += _T("\r\n");
	song_tip_info += CCommon::LoadText(IDS_ALBUM, _T(": "));
	song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].album.c_str();
	m_ui.UpdateSongInfoTip(song_tip_info);
}

void CMiniModeDlg::SetTitle()
{
	CString title;
	title = CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str();
	if (!title.IsEmpty())
		title += _T(" - ");
	title += _T("MusicPlayer2");
	SetWindowText(title);
}


BEGIN_MESSAGE_MAP(CMiniModeDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MINI_MODE_EXIT, &CMiniModeDlg::OnMiniModeExit)
	ON_WM_INITMENU()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMiniModeDlg::OnNMDblclkList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMiniModeDlg::OnNMRClickList2)
	ON_WM_PAINT()
	//ON_STN_CLICKED(IDC_MINI_PROGRESS_STATIC, &CMiniModeDlg::OnStnClickedMiniProgressStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_SHOW_PLAY_LIST, &CMiniModeDlg::OnShowPlayList)
	ON_WM_MOUSELEAVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_MINI_MIDE_MINIMIZE, &CMiniModeDlg::OnMiniMideMinimize)
END_MESSAGE_MAP()


// CMiniModeDlg ��Ϣ�������


void CMiniModeDlg::UpdatePlayPauseButton()
{
	m_ui.UpdatePlayPauseButtonTip();
}

void CMiniModeDlg::ShowPlaylist()
{
	m_playlist_ctrl.ShowPlaylist(theApp.m_ui_data.display_format);
	SetPlayListColor();
}

void CMiniModeDlg::SetPlayListColor()
{
	m_playlist_ctrl.SetHightItem(CPlayer::GetInstance().GetIndex());
	//m_playlist_ctrl.SetColor(theApp.m_app_setting_data.theme_color);
	m_playlist_ctrl.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);
}
//
//void CMiniModeDlg::SetDefaultBackGround(CImage * pImage)
//{
//	m_ui_data.pDefaultBackground = pImage;
//}

//void CMiniModeDlg::SetDisplayFormat(DisplayFormat * pDisplayFormat)
//{
//	m_ui_data.pDisplayFormat = pDisplayFormat;
//}
//
BOOL CMiniModeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_playlist_ctrl.SetFont(theApp.m_pMainWnd->GetFont());

	LoadConfig();

	m_pDC = GetDC();
	m_ui.Init(m_pDC);

	//��ʼ�������ʾ
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	m_Mytip.SetMaxTipWidth(800);

	m_ui.SetToolTip(&m_Mytip);
	//UpdateSongTipInfo();

	//��ʼ������λ��
	if (m_position_x != -1 && m_position_y != -1)
		SetWindowPos(nullptr , m_position_x, m_position_y, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOZORDER);
	else
		SetWindowPos(nullptr , 0, 0, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOMOVE | SWP_NOZORDER);
	CheckWindowPos();

	//��ʼ�������б�ؼ���λ��
	CRect playlist_rect;
	int margin = 0;
	playlist_rect.left = margin;
	playlist_rect.right = m_ui_data.widnow_width - margin;
	playlist_rect.top = m_ui_data.window_height + margin;
	playlist_rect.bottom = m_ui_data.window_height2 - margin;
	m_playlist_ctrl.MoveWindow(playlist_rect);
	m_playlist_ctrl.AdjustColumnWidth();

	//װ���Ҽ��˵�
	m_menu.LoadMenu(IDR_MINI_MODE_MENU);

	//���ö�ʱ��
	SetTimer(TIMER_ID_MINI, TIMER_ELAPSE_MINI, NULL);	//��������ʱ��
	SetTimer(TIMER_ID_MINI2, TIMER_ELAPSE, NULL);		//�������ڽ���ˢ�µĶ�ʱ��

	//��ʾ�����б�
	ShowPlaylist();

	//���ô��ڲ�͸����
	SetTransparency();

	m_show_playlist = false;
	m_ui_data.m_show_volume = false;
	m_first_start = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMiniModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_ui.PointInControlArea(point))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	m_ui.LButtonDown(point);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMiniModeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_ID_MINI)
	{
		//���������ʾ
		static int index{};
		static wstring song_name{};
		//�����ǰ���ŵĸ��������仯���͸��������ʾ��Ϣ
		if (index != CPlayer::GetInstance().GetIndex() || song_name != CPlayer::GetInstance().GetFileName())
		{
			UpdateSongTipInfo();
			SetTitle();
			//m_Mytip.UpdateTipText(m_song_tip_info, this);
			index = CPlayer::GetInstance().GetIndex();
			song_name = CPlayer::GetInstance().GetFileName();
			m_ui.DrawInfo(true);
		}
		
	}
	if (nIDEvent == TIMER_ID_MINI2)
	{
		if (!IsIconic() && IsWindowVisible())		//������С��������ʱ�����ƣ��Խ���CPU������
			m_ui.DrawInfo(false);

		if (m_first_start)
		{
			UpdateSongTipInfo();
			SetTitle();
		}
		m_first_start = false;
	}
	if (nIDEvent == 11)
	{
		m_ui_data.m_show_volume = false;
		KillTimer(11);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMiniModeDlg::SetVolume(bool up)
{
	if (up)
		CPlayer::GetInstance().MusicControl(Command::VOLUME_UP);
	else
		CPlayer::GetInstance().MusicControl(Command::VOLUME_DOWN);
	//ShowVolume(CPlayer::GetInstance().GetVolume());
	KillTimer(11);
	SetTimer(11, 1500, NULL);		//��ʾ����������һ��1500����Ķ�ʱ����������ʾ����1.5�룩
	m_ui_data.m_show_volume = true;
}

void CMiniModeDlg::SetTransparency()
{
	CCommon::SetWindowOpacity(m_hWnd, theApp.m_app_setting_data.window_transparency);
}

BOOL CMiniModeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnOK();		//��ESC������������
			return TRUE;
		}
		if (pMsg->wParam == VK_RETURN) return TRUE;

		//�����·������������
		if (pMsg->wParam == VK_UP)
		{
			SetVolume(true);
			return TRUE;
		}
		if (pMsg->wParam == VK_DOWN)
		{
			SetVolume(false);
			return TRUE;
		}

		//��Ctrl+X�˳�
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'X')
		{
			OnCancel();
			return TRUE;
		}
		//��Ctrl+M�ص�������
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'M')
		{
			OnOK();
			return TRUE;
		}

		if (pMsg->wParam == VK_APPS)		//���˵����������˵�
		{
			CRect rect;
			GetWindowRect(rect);
			m_main_popup_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect.left, rect.bottom, this);
			return TRUE;
		}

	}

	//���˴��ڵ�����������Ϣת����������
	if (pMsg->message == WM_KEYDOWN)
	{
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (pMsg->message == WM_MOUSEMOVE)
	{
		//POINT point = pMsg->pt;
		//ScreenToClient(&point);

		//if (!m_ui.PointInControlArea(point))
			m_Mytip.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMiniModeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetWindowRect(rect);
	m_position_x = rect.left;
	m_position_y = rect.top;
	SaveConfig();
	KillTimer(TIMER_ID_MINI);
	m_menu.DestroyMenu();
	ReleaseDC(m_pDC);
}


void CMiniModeDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: �ڴ˴������Ϣ����������
	CheckWindowPos();
}


void CMiniModeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���õ������Ҽ������˵�
	CPoint point1;	//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
	if (nFlags == MK_SHIFT)		//��סShift���������Ҽ�ʱ������ϵͳ�˵�
	{
		m_main_popup_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	}
	else
	{
		CMenu* pContextMenu = m_menu.GetSubMenu(0);
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //��ָ��λ����ʾ�����˵�
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CMiniModeDlg::OnMiniModeExit()
{
	// TODO: �ڴ���������������
	//ִ���˳�����ʱ�öԻ��򷵻�IDCANCEL�������ڼ�⵽�������ֵʱ���˳�����
	OnCancel();
}


void CMiniModeDlg::OnInitMenu(CMenu* pMenu)
{
	CDialogEx::OnInitMenu(pMenu);

	// TODO: �ڴ˴������Ϣ����������

	//��������ģʽ�����Ҽ��˵�
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

	//���ø�ѡ�˵���Ĺ�ѡ
	pMenu->CheckMenuItem(ID_SHOW_PLAY_LIST, MF_BYCOMMAND | (m_show_playlist ? MF_CHECKED : MF_UNCHECKED));

	//���ò����б��Ҽ��˵�
	//���������б��Ҽ��˵�ʱ�����û��ѡ�в����б��е���Ŀ��������Ҽ��˵��С����š��������б���ɾ�����������ԡ������Ӵ���ɾ������Ŀ��
	if (m_item_selected < 0 || m_item_selected >= CPlayer::GetInstance().GetSongNum())
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_ENABLED);
	}

	pMenu->EnableMenuItem(ID_DOWNLOAD_ALBUM_COVER, MF_BYCOMMAND | (!CPlayer::GetInstance().IsInnerCover() ? MF_ENABLED : MF_GRAYED));
	bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
	pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));

	//���ò����б��Ҽ��˵���Ĭ�ϲ˵���
	pMenu->SetDefaultItem(ID_PLAY_ITEM);

}


BOOL CMiniModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		SetVolume(true);
	}
	if (zDelta < 0)
	{
		SetVolume(false);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CMiniModeDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if(!m_ui.PointInControlArea(point))
	//	OnOK();

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CMiniModeDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = pNMItemActivate->iItem;
	CPlayer::GetInstance().PlayTrack(row);
	//SwitchTrack();
	SetPlayListColor();
	//RePaint();
	*pResult = 0;
}


void CMiniModeDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_item_selected = pNMItemActivate->iItem;	//��ȡ���ѡ�е���Ŀ
	m_playlist_ctrl.GetItemSelected(m_items_selected);

	CMenu* pContextMenu = m_popup_menu.GetSubMenu(0); //��ȡ��һ�������˵�
	CPoint point;			//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //��ָ��λ����ʾ�����˵�
	*pResult = 0;
}


void CMiniModeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	m_ui.DrawInfo();
}


//void CMiniModeDlg::OnStnClickedMiniProgressStatic()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//int pos = m_progress_bar.GetPos();
//	//int song_pos = static_cast<__int64>(pos) * CPlayer::GetInstance().GetSongLength() / 1000;
//	//CPlayer::GetInstance().SeekTo(song_pos);
//}


void CMiniModeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_ui.MouseMove(point);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMiniModeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_ui.LButtonUp(point);

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMiniModeDlg::OnShowPlayList()
{
	// TODO: �ڴ���������������
	if (m_show_playlist)
	{
		SetWindowPos(nullptr, 0, 0, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOMOVE | SWP_NOZORDER);
		CheckWindowPos();
		m_show_playlist = false;
	}
	else
	{
		SetWindowPos(nullptr, 0, 0, m_ui_data.widnow_width, m_ui_data.window_height2, SWP_NOMOVE | SWP_NOZORDER);
		CheckWindowPos();
		m_show_playlist = true;
	}
}


void CMiniModeDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_ui.MouseLeave();

	CDialogEx::OnMouseLeave();
}


BOOL CMiniModeDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_ui.SetCursor())
		return TRUE;
	else	
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CMiniModeDlg::OnMiniMideMinimize()
{
	// TODO: �ڴ���������������
	ShowWindow(HIDE_WINDOW);
}
