// SoundEffectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "SoundEffectDlg.h"
#include "afxdialogex.h"


// CSoundEffectDlg �Ի���

IMPLEMENT_DYNAMIC(CSoundEffectDlg, CDialog)

CSoundEffectDlg::CSoundEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SOUND_EFFECT_DIALOG, pParent)
{

}

CSoundEffectDlg::~CSoundEffectDlg()
{
}

void CSoundEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CSoundEffectDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSoundEffectDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CSoundEffectDlg ��Ϣ�������


BOOL CSoundEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CenterWindow();
	//�����ǩ
	m_tab.InsertItem(0, CCommon::LoadText(IDS_EQUALIZER));
	m_tab.InsertItem(1, CCommon::LoadText(IDS_REVERB));
	//�����ӶԻ���
	m_equ_dlg.Create(IDD_EQUALIZER_DIALOG, &m_tab);
	m_reverb_dlg.Create(IDD_REVERB_DIALOG, &m_tab);
	//�����ӶԻ���Ĵ�С��λ��
	CRect rect;
	m_tab.GetClientRect(rect);
	CRect rcTabItem;
	m_tab.GetItemRect(0, rcTabItem);
	rect.top += rcTabItem.Height() + 4;
	rect.left += 4;
	rect.bottom -= 4;
	rect.right -= 4;
	m_equ_dlg.MoveWindow(&rect);
	m_reverb_dlg.MoveWindow(&rect);
	//Ĭ��ѡ�е�һ����ǩ
	m_equ_dlg.ShowWindow(SW_SHOW);
	m_tab.SetCurFocus(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CSoundEffectDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tab_selected = m_tab.GetCurSel();
	switch (m_tab_selected)
	{
	case 0:
		m_equ_dlg.ShowWindow(SW_SHOW);
		m_reverb_dlg.ShowWindow(SW_HIDE);
		m_equ_dlg.SetFocus();
		break;
	case 1:
		m_reverb_dlg.ShowWindow(SW_SHOW);
		m_equ_dlg.ShowWindow(SW_HIDE);
		m_reverb_dlg.SetFocus();
		break;
	}
	*pResult = 0;
}


void CSoundEffectDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();

	//CDialog::OnCancel();
}
