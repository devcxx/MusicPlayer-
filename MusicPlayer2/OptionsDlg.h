#pragma once
#include "afxcmn.h"
#include "LyricSettingsDlg.h"
#include "AppearanceSettingDlg.h"
#include "DataSettingsDlg.h"
#include "PlaySettingsDlg.h"
#include "CTabCtrlEx.h"
#include "CHotKeySettingDlg.h"

// COptionsDlg �Ի���
#define TIMER_ID2 1122
#define WM_SETTINGS_APPLIED (WM_USER+113)

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionsDlg();

	CLyricSettingsDlg m_tab1_dlg{ this };		//��������á��ӶԻ���
	CAppearanceSettingDlg m_tab2_dlg{ this };	//��������á��ӶԻ���
	CDataSettingsDlg m_tab3_dlg{ this };		//���������á��ӶԻ���
	CPlaySettingsDlg m_tab4_dlg{ this };		//������ѡ��ӶԻ���
	CHotKeySettingDlg m_tab5_dlg{ this };		//��ȫ�ֿ�ݼ����ӶԻ���

	int m_tab_selected{};

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG };
#endif

protected:

	CTabCtrlEx m_tab;		//ѡ��ؼ�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedApplyButton();
	afx_msg void OnDestroy();
};
