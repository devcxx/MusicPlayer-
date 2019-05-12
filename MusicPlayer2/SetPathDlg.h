#pragma once
#include "afxwin.h"
#include "Common.h"
#include "AudioCommon.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"

// CSetPathDlg �Ի���
#define WM_PATH_SELECTED (WM_USER+107)

class CSetPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPathDlg)

public:
	CSetPathDlg(deque<PathInfo>& recent_path, wstring current_path, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetPathDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_PATH_DIALOG };
#endif

public:
	void QuickSearch(const wstring& key_words);		//���ݹؼ���ִ�п��ٲ���m_search_result��

protected:
	deque<PathInfo>& m_recent_path;		//����򿪹���·��
	int m_path_selected{};		//ѡ���·��
	wstring m_current_path;

	CEdit m_path_name;
	CListCtrlEx m_path_list;
	CMenu m_menu;
	CEdit m_search_edit;
	CToolTipCtrl m_Mytip;

	CSize m_min_size;		//���ڵ���С��С

	vector<int> m_search_result;			//���������������ĸ������
	bool m_searched{ false };				//�Ƿ�������״̬

protected:
	void ShowPathList();
	void SetButtonsEnable(bool enable);
	void CalculateColumeWidth(vector<int>& width);
	void SetListRowData(int index, const PathInfo& path_info);
	bool IsSelectedPlayEnable() const;			//�ж�ѡ��·���Ƿ���Բ���

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	wstring GetSelPath() const;		//��ȡѡ���·��
	int GetTrack() const;
	int GetPosition() const;
	SortMode GetSortMode() const;
	bool SelectValid() const;		//�ж�ѡ���Ƿ���Ч

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedDeletePathButton();
	//virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedOpenFolder();
	afx_msg void OnPlayPath();
	afx_msg void OnDeletePath();
	afx_msg void OnBrowsePath();
	afx_msg void OnClearInvalidPath();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnEnChangeSearchEdit();
	afx_msg void OnBnClickedClearButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
