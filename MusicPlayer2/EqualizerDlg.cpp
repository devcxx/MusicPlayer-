// EqualizerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "EqualizerDlg.h"
#include "afxdialogex.h"


// CEqualizerDlg �Ի���

IMPLEMENT_DYNAMIC(CEqualizerDlg, CDialogEx)

CEqualizerDlg::CEqualizerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EQUALIZER_DIALOG, pParent)
{

}

CEqualizerDlg::~CEqualizerDlg()
{
}

void CEqualizerDlg::EnableControls(bool enable)
{
	m_equ_style_list.EnableWindow(enable);
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		m_sliders[i].EnableWindow(enable);
	}
}

void CEqualizerDlg::SaveConfig() const
{
	CIniHelper ini(theApp.m_config_path);

	ini.WriteInt(L"equalizer", L"equalizer_style", m_equ_style_selected);
	//�����Զ����ÿ��������ͨ��������
	wchar_t buff[16];
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		swprintf_s(buff, L"channel%d", i + 1);
		ini.WriteInt(L"equalizer", buff, m_user_defined_gain[i]);
	}
	ini.Save();
}

void CEqualizerDlg::LoadConfig()
{
	CIniHelper ini(theApp.m_config_path);

	m_equ_style_selected = ini.GetInt(L"equalizer", L"equalizer_style", 0);
	//��ȡ�Զ����ÿ��������ͨ��������
	wchar_t buff[16];
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		swprintf_s(buff, L"channel%d", i + 1);
		m_user_defined_gain[i] = ini.GetInt(L"equalizer", buff, 0);
	}
}

void CEqualizerDlg::UpdateChannelTip(int channel, int gain)
{
	if (channel < 0 || channel >= EQU_CH_NUM) return;
	wchar_t buff[8];
	swprintf_s(buff, L"%ddB", gain);
	m_Mytip.UpdateTipText(buff, &m_sliders[channel]);		//���������ʾ
}

void CEqualizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliders[0]);
	DDX_Control(pDX, IDC_SLIDER2, m_sliders[1]);
	DDX_Control(pDX, IDC_SLIDER3, m_sliders[2]);
	DDX_Control(pDX, IDC_SLIDER4, m_sliders[3]);
	DDX_Control(pDX, IDC_SLIDER5, m_sliders[4]);
	DDX_Control(pDX, IDC_SLIDER6, m_sliders[5]);
	DDX_Control(pDX, IDC_SLIDER7, m_sliders[6]);
	DDX_Control(pDX, IDC_SLIDER8, m_sliders[7]);
	DDX_Control(pDX, IDC_SLIDER9, m_sliders[8]);
	DDX_Control(pDX, IDC_SLIDER10, m_sliders[9]);
	DDX_Control(pDX, IDC_EQU_STYLES_LIST, m_equ_style_list);
	DDX_Control(pDX, IDC_ENABLE_EQU_CHECK, m_enable_equ_check);
}


BEGIN_MESSAGE_MAP(CEqualizerDlg, CDialogEx)
//	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_ENABLE_EQU_CHECK, &CEqualizerDlg::OnBnClickedEnableEquCheck)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_LISTBOX_SEL_CHANGED, &CEqualizerDlg::OnListboxSelChanged)
END_MESSAGE_MAP()


// CEqualizerDlg ��Ϣ�������


BOOL CEqualizerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LoadConfig();
	//��ʼ����������λ��
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		m_sliders[i].SetRange(-15, 15, TRUE);
		m_sliders[i].SetPos(-CPlayer::GetInstance().GeEqualizer(i));
	}

	//��ʼ�����������ظ�ѡ��
	m_enable_equ_check.SetCheck(CPlayer::GetInstance().GetEqualizerEnable());

	//��ʼ����������Ԥ�衱�б�
	m_equ_style_list.AddString(CCommon::LoadText(IDS_NONE));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_CLASSICAL));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_POP));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_JAZZ));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_ROCK));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_SOFT));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_BASS));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_ELIMINATE_BASS));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_WEAKENING_TREBLE));
	m_equ_style_list.AddString(CCommon::LoadText(IDS_USER_DEFINE));
	m_equ_style_list.SetCurSel(m_equ_style_selected);

	//��ʼ���ؼ������ý���״̬
	EnableControls(CPlayer::GetInstance().GetEqualizerEnable());
	
	//��ʼ����ʾ��Ϣ
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	wchar_t buff[8];
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		swprintf_s(buff, L"%ddB", -m_sliders[i].GetPos());
		m_Mytip.AddTool(&m_sliders[i], buff);
	}

	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CEqualizerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		if (pScrollBar->GetSafeHwnd() == m_sliders[i].GetSafeHwnd())
		{
			int gain{ -m_sliders[i].GetPos() };		//���ڻ�������ֵԽ����ԽС����������ȡ����
			CPlayer::GetInstance().SetEqualizer(i, gain);		//����ͨ��i������
			UpdateChannelTip(i, gain);		//���������ʾ

			if (m_equ_style_list.GetCurSel() == 9)		//�����������Ԥ�衱��ѡ�е����Զ���
			{
				m_user_defined_gain[i] = gain;		//������ֵ���浽�û��Զ���������
			}
			else
			{
				//���򣬽���ǰÿ���������ϵ��������ñ��浽m_user_defined_gain��
				for (int j{}; j < EQU_CH_NUM; j++)
				{
					m_user_defined_gain[j] = -m_sliders[j].GetPos();
				}
				m_equ_style_list.SetCurSel(9);		//�����˾��������趨����������Ԥ�衱���Զ�ѡ�С��Զ��塱
				m_equ_style_selected = 9;
			}
			break;
		}
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CEqualizerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEqualizerDlg::OnBnClickedEnableEquCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool enable = (m_enable_equ_check.GetCheck() != 0);
	CPlayer::GetInstance().EnableEqualizer(enable);
	EnableControls(enable);
}


afx_msg LRESULT CEqualizerDlg::OnListboxSelChanged(WPARAM wParam, LPARAM lParam)
{
	if ((CWnd*)wParam == &m_equ_style_list)
	{
		m_equ_style_selected = (int)lParam;
		if (m_equ_style_selected >= 0 && m_equ_style_selected < 9)
		{
			//����ѡ�еľ������������ÿ��ͨ��������
			for (int i{}; i < EQU_CH_NUM; i++)
			{
				int gain = EQU_STYLE_TABLE[m_equ_style_selected][i];
				CPlayer::GetInstance().SetEqualizer(i, gain);
				m_sliders[i].SetPos(-gain);
				UpdateChannelTip(i, gain);		//���������ʾ
			}
		}
		else if (m_equ_style_selected == 9)		//���ѡ���ˡ��Զ��塱
		{
			for (int i{}; i < EQU_CH_NUM; i++)
			{
				int gain = m_user_defined_gain[i];
				CPlayer::GetInstance().SetEqualizer(i, gain);
				m_sliders[i].SetPos(-gain);
				UpdateChannelTip(i, gain);		//���������ʾ
			}
		}

	}
	return 0;
}


void CEqualizerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	SaveConfig();
}


HBRUSH CEqualizerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//���þ�����ͨ���������ı���ɫΪ��ɫ
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		if (pWnd == &m_sliders[i])
		{
			return (HBRUSH)::GetStockObject(WHITE_BRUSH);
		}
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
