// PlayTrackDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlayTrackDlg.h"
#include "afxdialogex.h"


// CPlayTrackDlg �Ի���

IMPLEMENT_DYNAMIC(CPlayTrackDlg, CDialogEx)

CPlayTrackDlg::CPlayTrackDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PLAY_TRACK_DIALOG, pParent)
{

}

CPlayTrackDlg::~CPlayTrackDlg()
{
}

void CPlayTrackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TRACK, m_track_edit);
}


BEGIN_MESSAGE_MAP(CPlayTrackDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TRACK, &CPlayTrackDlg::OnEnChangeEditTrack)
END_MESSAGE_MAP()


// CPlayTrackDlg ��Ϣ�������


int CPlayTrackDlg::GetTrack() const
{
	return m_track;
}

void CPlayTrackDlg::OnEnChangeEditTrack()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_track_edit.GetWindowText(str);
	m_track = _wtoi(str);
}


BOOL CPlayTrackDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_track_edit.SetLimitText(6);
	m_track_edit.SetFocus();		//��ʼʱ���������õ��༭�ؼ�

	return FALSE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
