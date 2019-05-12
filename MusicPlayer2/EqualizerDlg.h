#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CListBoxEnhanced.h"


// CEqualizerDlg �Ի���

class CEqualizerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEqualizerDlg)

public:
	CEqualizerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEqualizerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQUALIZER_DIALOG };
#endif

protected:
	int m_user_defined_gain[EQU_CH_NUM]{};		//�û��Զ���ľ���������

	CSliderCtrl m_sliders[EQU_CH_NUM];
	CListBoxEnhanced m_equ_style_list;
	CButton m_enable_equ_check;
	CToolTipCtrl m_Mytip;		//�����ʾ
	int m_equ_style_selected{};		//ѡ�еľ�����Ԥ��

	void EnableControls(bool enable);		//���û���ÿؼ�
	void SaveConfig() const;
	void LoadConfig();
	void UpdateChannelTip(int channel, int gain);		//����ͨ���������������ʾ

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedEnableEquCheck();
	afx_msg LRESULT OnListboxSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
