#pragma once

#include "DownloadListCtrl.h"
#include "FormatConvertDlg.h"

// CDownloadMgrDlg �Ի���

class CDownloadMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadMgrDlg)

public:
	CDownloadMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadMgrDlg();

	CDownloadListCtrl& GetDownListCtrl() { return m_down_list; }


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWN_MGR_DLG };
#endif

public:
	afx_msg void OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnFormatConvert();
	afx_msg void OnItemExploreTrack();
	afx_msg void OnItemProperty();
	afx_msg void OnPlayItem();

	virtual BOOL OnInitDialog();

	CMenu m_list_popup_menu;		//�����б��Ҽ��˵�
	CFormatConvertDlg* m_pFormatConvertDlg{NULL};		//��ʽת���Ի��򣨷�ģ̬�Ի���
protected:
	CDownloadListCtrl m_down_list;
	int m_item_selected{};		//�����б������ѡ�е���Ŀ
	HWND m_hParent{};
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
