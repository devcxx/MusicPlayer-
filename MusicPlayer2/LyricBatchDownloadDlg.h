#pragma once
#include "afxwin.h"
#include "Common.h"
#include "afxcmn.h"
#include "LyricDownloadCommon.h"
#include "LyricDownloadDlg.h"
#include "ListCtrlEx.h"

// CLyricBatchDownloadDlg �Ի���

class CLyricBatchDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLyricBatchDownloadDlg)

public:
	CLyricBatchDownloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLyricBatchDownloadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRIC_BATCH_DOWN_DIALOG };
#endif

#define WM_BATCH_DOWNLOAD_COMPLATE (WM_USER+103)		//����������������Ϣ

	//������������̴߳������ݵĽṹ��
	struct ThreadInfo
	{
		CListCtrl* list_ctrl;
		CStatic* static_ctrl;
		const vector<SongInfo>* playlist;
		bool skip_exist;
		bool download_translate;
		bool save_to_song_folder;
		CodeType save_code;
		HWND hwnd;
		//bool exit;
	};

	//�����̺߳���
	static UINT ThreadFunc(LPVOID lpParam);

	ThreadInfo m_thread_info;

	static bool SaveLyric(const wchar_t* path, const wstring& lyric_wcs, CodeType code_type, bool* char_cannot_convert);		//�����ʣ��������ʱ���ΪANSI��ʽʱ���޷�ת����Unicode�ַ�����char_cannot_convert��Ϊtrue

protected:
	CButton m_skip_exist_check;
	CComboBox m_save_code_combo;
	CListCtrlEx m_song_list_ctrl;
	CButton m_download_translate_chk;
	CStatic m_info_static;

	bool m_skip_exist{ true };		//�������и�ʵ���Ŀ
	CodeType m_save_code{};		//����ı����ʽ
	bool m_download_translate{ false };		//�Ƿ����ظ�ʷ���
	bool m_save_to_song_folder{ true };		//�Ƿ񱣴浽��������Ŀ¼
	const vector<SongInfo>& m_playlist{ CPlayer::GetInstance().GetPlayList() };	//�����б������

	bool m_lyric_path_not_exit{ false };

	CWinThread* m_pThread{};		//���ظ�ʵ��߳�

	void SaveConfig() const;
	void LoadConfig();

	void EnableControls(bool enable);		//���û���ÿؼ�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartDownload();
	afx_msg void OnBnClickedSkipExistCheck();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedDownloadTrasnlateCheck2();
protected:
	afx_msg LRESULT OnBatchDownloadComplate(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedSaveToSongFolder();
	afx_msg void OnBnClickedSaveToLyricFolder();
};
