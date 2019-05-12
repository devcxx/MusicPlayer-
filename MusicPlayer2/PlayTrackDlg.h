#pragma once
#include "afxwin.h"


// CPlayTrackDlg �Ի���

class CPlayTrackDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayTrackDlg)

public:
	CPlayTrackDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayTrackDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAY_TRACK_DIALOG };
#endif

protected:
	int m_track;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_track_edit;

	int GetTrack() const;
	afx_msg void OnEnChangeEditTrack();
	virtual BOOL OnInitDialog();
};
