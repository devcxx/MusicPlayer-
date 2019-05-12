// ReverbDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "ReverbDlg.h"
#include "afxdialogex.h"


// CReverbDlg �Ի���

IMPLEMENT_DYNAMIC(CReverbDlg, CDialogEx)

CReverbDlg::CReverbDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REVERB_DIALOG, pParent)
{

}

CReverbDlg::~CReverbDlg()
{
}

void CReverbDlg::EnableControls(bool enable)
{
	m_reverb_mix_slider.EnableWindow(enable);
	m_reverb_time_slider.EnableWindow(enable);
}

void CReverbDlg::UpdateStaticText(int mix, int time)
{
	wchar_t buff[16];
	swprintf_s(buff, L"%d%%", mix);
	SetDlgItemText(IDC_REVERB_MIX_STATIC, buff);
	swprintf_s(buff, L"%.2fs", static_cast<float>(time) / 100);
	SetDlgItemText(IDC_REVERB_TIME_STATIC, buff);
}

void CReverbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REVERB_MIX_SLIDER, m_reverb_mix_slider);
	DDX_Control(pDX, IDC_REVERB_TIME_SLIDER, m_reverb_time_slider);
	DDX_Control(pDX, IDC_ENABLE_REVERB_CHECK, m_enable_reverb_chk);
}


BEGIN_MESSAGE_MAP(CReverbDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ENABLE_REVERB_CHECK, &CReverbDlg::OnBnClickedEnableReverbCheck)
END_MESSAGE_MAP()


// CReverbDlg ��Ϣ�������


BOOL CReverbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ñ���ɫΪ��ɫ
	SetBackgroundColor(RGB(255, 255, 255));

	//��ʼ��������
	m_reverb_mix_slider.SetRange(0, 100);
	m_reverb_mix_slider.SetPos(CPlayer::GetInstance().GetReverbMix());
	m_reverb_time_slider.SetRange(1, 300);
	m_reverb_time_slider.SetPos(CPlayer::GetInstance().GetReverbTime());

	//��ʼ����̬�ؼ����ı�
	UpdateStaticText(m_reverb_mix_slider.GetPos(), m_reverb_time_slider.GetPos());

	//��ʼ�����쿪�ظ�ѡ��
	m_enable_reverb_chk.SetCheck(CPlayer::GetInstance().GetReverbEnable());

	//��ʼ���ؼ�������״̬
	EnableControls(CPlayer::GetInstance().GetReverbEnable());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CReverbDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pScrollBar->GetDlgCtrlID() == IDC_REVERB_MIX_SLIDER || pScrollBar->GetDlgCtrlID() == IDC_REVERB_TIME_SLIDER)
	{
		int mix{ m_reverb_mix_slider.GetPos() };
		int time{ m_reverb_time_slider.GetPos() };
		CPlayer::GetInstance().SetReverb(mix, time);
		UpdateStaticText(mix, time);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


HBRUSH CReverbDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == &m_reverb_mix_slider || pWnd == &m_reverb_time_slider)		//���û������ؼ��ı���ɫΪ��ɫ
	{
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CReverbDlg::OnBnClickedEnableReverbCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool enable = (m_enable_reverb_chk.GetCheck() != 0);
	CPlayer::GetInstance().EnableReverb(enable);
	EnableControls(enable);
}
