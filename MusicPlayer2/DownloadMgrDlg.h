#pragma once

#include "DownloadListCtrl.h"
// CDownloadMgrDlg �Ի���

class CDownloadMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadMgrDlg)

public:
	CDownloadMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadMgrDlg();

	CDownloadListCtrl& GetDownList() { return m_down_list; }

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWN_MGR_DLG };
#endif

public:


protected:
	CDownloadListCtrl m_down_list;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
