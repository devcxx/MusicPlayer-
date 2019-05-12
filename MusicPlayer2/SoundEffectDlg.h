#pragma once
#include "EqualizerDlg.h"
#include "ReverbDlg.h"
#include "afxcmn.h"

// CSoundEffectDlg �Ի���

class CSoundEffectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoundEffectDlg)

public:
	CSoundEffectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoundEffectDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOUND_EFFECT_DIALOG };
#endif

private:
	CEqualizerDlg m_equ_dlg;		//�������Ի���
	CReverbDlg m_reverb_dlg;		//����Ի���

	int m_tab_selected{};
	CTabCtrl m_tab;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
};
