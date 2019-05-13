// DownloadMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "DownloadMgrDlg.h"
#include "afxdialogex.h"
#include "Common.h"
#include "PropertyDlg.h"

// CDownloadMgrDlg �Ի���

IMPLEMENT_DYNAMIC(CDownloadMgrDlg, CDialogEx)

CDownloadMgrDlg::CDownloadMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOWN_MGR_DLG, pParent)
{
	if (pParent)
		m_hParent = pParent->GetSafeHwnd();
}

CDownloadMgrDlg::~CDownloadMgrDlg()
{
}

void CDownloadMgrDlg::OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_item_selected = pNMItemActivate->iItem;	//��ȡ���ѡ�е���Ŀ

	CMenu* pContextMenu = m_list_popup_menu.GetSubMenu(0); //��ȡ��һ�������˵�
	CPoint point;			//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //��ָ��λ����ʾ�����˵�

	*pResult = 0;
}

void CDownloadMgrDlg::OnFormatConvert()
{
	if (!theApp.m_format_convert_dialog_exit)
		return;
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
	vector<int> items_selected;
	int item_id = GetDownListCtrl().DownItems()[m_item_selected]->item_id;
	items_selected.push_back(item_id);
	m_pFormatConvertDlg = new CFormatConvertDlg(items_selected);
	m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
	m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}

void CDownloadMgrDlg::OnItemExploreTrack()
{
	// TODO: �ڴ���������������
	int item_id = GetDownListCtrl().DownItems()[m_item_selected]->item_id;
	CPlayer::GetInstance().GetPlayList();
	std::wstring path = CPlayer::GetInstance().GetPlayList()[item_id].file_name;
	CPlayer::GetInstance().ExplorePath(path);
}

void CDownloadMgrDlg::OnItemProperty()
{
	// TODO: �ڴ���������������
	CPropertyDlg propertyDlg(CPlayer::GetInstance().GetPlayList());
	propertyDlg.m_path = CPlayer::GetInstance().GetSongPath();
	propertyDlg.m_index = GetDownListCtrl().DownItems()[m_item_selected]->item_id;
	propertyDlg.m_song_num = CPlayer::GetInstance().GetSongNum();
	//propertyDlg.m_playing_index = CPlayer::GetInstance().GetIndex();
	//propertyDlg.m_lyric_name = CPlayer::GetInstance().GetLyricName();
	propertyDlg.DoModal();
// 	if (propertyDlg.GetListRefresh())
// 		ShowPlayList();
}

void CDownloadMgrDlg::OnPlayItem()
{
	int item_id = GetDownListCtrl().DownItems()[m_item_selected]->item_id;
	::SendMessage(m_hParent, WM_PLAY_SELECTED_ITEM, item_id, NULL);

// 	CPlayer::GetInstance().PlayTrack(m_item_selected);
// 	SwitchTrack();
// 	UpdatePlayPauseButton();
}

BOOL CDownloadMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list_popup_menu.LoadMenu(IDR_DOWNMGR_POPUP_MENU);

	return TRUE;
}

void CDownloadMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWN_LIST, m_down_list);

}


BEGIN_MESSAGE_MAP(CDownloadMgrDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_DOWN_LIST, &CDownloadMgrDlg::OnNMRClickPlaylistList)
	ON_COMMAND(ID_DOWNMGR_FORMAT_CONVERT, &CDownloadMgrDlg::OnFormatConvert)
	ON_COMMAND(ID_DOWNMGR_EXPLORE_TRACK, &CDownloadMgrDlg::OnItemExploreTrack)
	ON_COMMAND(ID_DOWNMGR_ITEM_PROPERTY, &CDownloadMgrDlg::OnItemProperty)
	ON_COMMAND(ID_DOWNMGR_PLAY_ITEM, &CDownloadMgrDlg::OnPlayItem)
END_MESSAGE_MAP()


// CDownloadMgrDlg ��Ϣ�������
