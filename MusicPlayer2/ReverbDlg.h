#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReverbDlg �Ի���

class CReverbDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReverbDlg)

public:
	CReverbDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReverbDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REVERB_DIALOG };
#endif

protected:
	CSliderCtrl m_reverb_mix_slider;
	CSliderCtrl m_reverb_time_slider;

	void EnableControls(bool enable);		//���û���ÿؼ�
	void UpdateStaticText(int mix, int time);		//���¾�̬�ؼ��ϵ��ı�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_enable_reverb_chk;
	afx_msg void OnBnClickedEnableReverbCheck();
};
