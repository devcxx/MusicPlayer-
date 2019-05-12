#pragma once
#include"Common.h"
#include "TabDlg.h"

// CDataSettingsDlg �Ի���

class CDataSettingsDlg : public CTabDlg
{
	DECLARE_DYNAMIC(CDataSettingsDlg)

public:
	CDataSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataSettingsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATA_SETTINGS_DIALOG };
#endif

	GeneralSettingData m_data;

private:
	size_t m_data_size;		//�����ļ��Ĵ�С
	CToolTipCtrl m_toolTip;
	CEdit m_sf2_path_edit;
	CComboBox m_language_combo;

	void ShowDataSizeInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCleanDataFileButton();
	afx_msg void OnBnClickedId3v2FirstCheck();
	afx_msg void OnBnClickedCoverAutoDownloadCheck();
	afx_msg void OnBnClickedLyricAutoDownloadCheck();
	afx_msg void OnBnClickedCheckUpdateCheck();
	afx_msg void OnBnClickedBrowseButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedMidiUseInnerLyricCheck();
	afx_msg void OnBnClickedDownloadWhenTagFullCheck();
	afx_msg void OnEnChangeSf2PathEdit();
	virtual void OnOK();
};
