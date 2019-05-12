#pragma once
#include "LyricDownloadCommon.h"
#include "afxcmn.h"
#include "Lyric.h"
#include "afxwin.h"
#include "ListCtrlEx.h"


// CLyricDownloadDlg �Ի���

class CLyricDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLyricDownloadDlg)

public:
	CLyricDownloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLyricDownloadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRIC_DOWNLOAD_DIALOG };
#endif

#define WM_SEARCH_COMPLATE (WM_USER+101)		//�������������Ϣ
#define WM_DOWNLOAD_COMPLATE (WM_USER+102)		//������������Ϣ

	//�������������̴߳������ݵĽṹ��
	struct SearchThreadInfo
	{
		wstring url;
		wstring result;
		int rtn;
		HWND hwnd;
		//bool exit;
	};
	//��������̺߳���
	static UINT LyricSearchThreadFunc(LPVOID lpParam);

	//�������������̴߳������ݵĽṹ��
	struct DownloadThreadInfo
	{
		wstring song_id;
		wstring result;
		bool success;
		bool download_translate;
		bool save_as;		//������ɺ��ǵ��������Ϊ���Ի�����ֱ�ӱ���
		HWND hwnd;
		//bool exit;
	};
	//��������̺߳���
	static UINT LyricDownloadThreadFunc(LPVOID lpParam);

	SearchThreadInfo m_search_thread_info;
	DownloadThreadInfo m_download_thread_info;

protected:
	wstring m_title;		//Ҫ���Ҹ�ʵĸ����ı���
	wstring m_artist;		//Ҫ���Ҹ�ʵĸ�����������
	wstring m_album;		//Ҫ���Ҹ�ʵĸ����ĳ�Ƭ��
	wstring m_file_name;	//Ҫ���Ҹ�ʵĸ������ļ���
	wstring m_file_path;			//��ǰҪ����ĸ���ļ�������·��
	wstring m_search_result;	//���ҽ���ַ���
	wstring m_lyric_str;	//���صĸ��
	vector<CInternetCommon::ItemInfo> m_down_list;	//����������б�

	int m_item_selected{ -1 };		//��������б���ѡ�е���Ŀ
	bool m_download_translate{ false };
	bool m_save_to_song_folder{ true };		//�Ƿ񱣴浽��������Ŀ¼
	CodeType m_save_code{};		//����ı����ʽ
	int m_search_max_item{ 30 };		//���Ҹ���ʱ���ص��������

	CMenu m_menu;
	CListCtrlEx m_down_list_ctrl;
	CButton m_download_translate_chk;
	CComboBox m_save_code_combo;
	//CToolTipCtrl m_tool_tip;		//���ָ��ʱ�Ĺ�����ʾ
	CLinkCtrl m_unassciate_lnk;

	CWinThread* m_pSearchThread;		//������ʵ��߳�
	CWinThread* m_pDownThread;			//���ظ�ʵ��߳�

	void ShowDownloadList();		//�����������ʾ����
	bool SaveLyric(const wchar_t* path, CodeType code_type);	//������

	void SaveConfig() const;
	void LoadConfig();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	bool IsItemSelectedValid() const;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSearchButton2();
	afx_msg void OnEnChangeTitleEdit1();
	afx_msg void OnEnChangeArtistEdit1();
	afx_msg void OnNMClickLyricDownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLyricDownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDownloadSelected();
	afx_msg void OnBnClickedDownloadTranslateCheck1();
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnSearchComplate(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg LRESULT OnDownloadComplate(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedSaveToSongFolder1();
	afx_msg void OnBnClickedSaveToLyricFolder1();
	afx_msg void OnBnClickedSelectedSaveAs();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnLdLyricDownload();
	afx_msg void OnLdLyricSaveas();
	afx_msg void OnLdCopyTitle();
	afx_msg void OnLdCopyArtist();
	afx_msg void OnLdCopyAlbum();
	afx_msg void OnLdCopyId();
	afx_msg void OnLdViewOnline();
	afx_msg void OnNMDblclkLyricDownList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickUnassociateLink(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLdPreview();
};
