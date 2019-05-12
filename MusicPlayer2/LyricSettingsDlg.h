#pragma once
#include "afxwin.h"
#include "FolderBrowserDlg.h"
#include "TabDlg.h"

// CLyricSettingsDlg �Ի���

class CLyricSettingsDlg : public CTabDlg
{
	DECLARE_DYNAMIC(CLyricSettingsDlg)

public:
	CLyricSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLyricSettingsDlg();

	bool FontChanged() const { return m_font_changed; }

	//ѡ�����õ�����
	LyricSettingData m_data;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRIC_SETTING_DIALOG };
#endif

protected:
	CButton m_karaoke_disp_check;
	CButton m_lyric_fuzzy_match_check;
	CButton m_lyric_double_line_chk;
	CButton m_show_album_cover_in_cortana_check;
	CButton m_cortana_icon_beat_check;
	CComboBox m_cortana_color_combo;
	CButton m_lyric_compatible_mode_chk;
	CButton m_keep_display_chk;
	CButton m_show_spectrum_chk;

	CToolTipCtrl m_tool_tip;
	bool m_font_changed{ false };

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void EnableControl();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKaraokeDisp();
	afx_msg void OnBnClickedExploreLyricButton();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedLyricFuzzyMatch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_show_lyric_in_cortana_check;
	afx_msg void OnBnClickedShowLyricInCortana();
	afx_msg void OnBnClickedLyricDoubleLineCheck();
	afx_msg void OnCbnSelchangeCortanaColorCombo();
	afx_msg void OnBnClickedShowAlbumCoverInCortana();
	afx_msg void OnBnClickedCortanaIconDeatCheck();
	afx_msg void OnBnClickedLyricCompatibleMode();
	afx_msg void OnBnClickedSetFont();
	afx_msg void OnBnClickedKeepDisplayCheck();
	afx_msg void OnBnClickedShowSpectrumInCortana();
};
