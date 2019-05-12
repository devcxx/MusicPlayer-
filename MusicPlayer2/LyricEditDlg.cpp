// LyricEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "LyricEditDlg.h"
#include "afxdialogex.h"


// CLyricEditDlg �Ի���

IMPLEMENT_DYNAMIC(CLyricEditDlg, CDialog)

CLyricEditDlg::CLyricEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LYRIC_EDIT_DIALOG, pParent)
{

}

CLyricEditDlg::~CLyricEditDlg()
{
}

void CLyricEditDlg::OpreateTag(TagOpreation operation)
{
	int start, end;			//���ѡ�е���ʼ�Ľ���λ��
	int tag_index;		//Ҫ������ʱ���ǩ��λ��
	m_lyric_edit.GetSel(start, end);
	tag_index = m_lyric_string.rfind(L"\r\n", start - 1);	//�ӹ��λ����ǰ����\r\n��λ��
	if (tag_index == string::npos)
		tag_index = 0;									//���û���ҵ����������λ������ǰ��
	else
		tag_index += 2;

	Time time_tag{ CPlayer::GetInstance().GetCurrentPosition() };		//��ȡ��ǰ����ʱ��
	wchar_t time_tag_str[16];
	swprintf_s(time_tag_str, L"[%.2d:%.2d.%.2d]", time_tag.min, time_tag.sec, time_tag.msec / 10);

	int index2;		//��ǰʱ���ǩ�Ľ���λ��
	int tag_length;		//��ǰʱ���ǩ�ĳ���
	int tag_index2;		//������ڴ�ʱ���ǩ��ʼ��λ��
	index2 = m_lyric_string.find(L']', tag_index);
	tag_length = index2 - tag_index + 1;
	switch (operation)
	{
	case TagOpreation::INSERT:			//����ʱ���ǩ���ڹ�������е�����߲��룩
		m_lyric_string.insert(tag_index, time_tag_str);
		break;
	case TagOpreation::REPLACE:			//�滻ʱ���ǩ���滻��������е���ߵı�ǩ��
		m_lyric_string.replace(tag_index,tag_length, time_tag_str, wcslen(time_tag_str));
		break;
	case TagOpreation::DELETE_:			//ɾ��ʱ���ǩ��ɾ��������ڴ��ı�ǩ��
		tag_index2 = m_lyric_string.rfind(L'[', start);
		if (tag_index2 < tag_index) tag_index2 = tag_index;
		index2 = m_lyric_string.find(L']', tag_index2);
		tag_length = index2 - tag_index2 + 1;
		m_lyric_string.erase(tag_index2, tag_length);
		break;
	}
	

	int next_index;			//��һ�е���ʼλ��
	next_index = m_lyric_string.find(L"\r\n", start);
	if (next_index == string::npos)
	{
		if (operation != TagOpreation::DELETE_)
			m_lyric_string += L"\r\n";				//���û���ҵ���˵����ǰ�Ѿ������һ���ˣ���ĩβ���ϻ��з�
		next_index = m_lyric_string.size() - 1;
	}
	else
	{
		next_index += 2;
	}

	m_lyric_edit.SetWindowText(m_lyric_string.c_str());
	if (operation != TagOpreation::DELETE_)
		m_lyric_edit.SetSel(next_index, next_index);
	else
		m_lyric_edit.SetSel(tag_index, tag_index);
	m_lyric_edit.SetFocus();
	m_modified = true;
	UpdateStatusbarInfo();
}

bool CLyricEditDlg::SaveLyric(const wchar_t * path, CodeType code_type)
{
	if (path[0] == L'\0')		//�������ʱ���ݵ�·���Ŀ��ַ������򽫱���·������Ϊ��ǰ��������·��
	{
		const SongInfo& song{ CPlayer::GetInstance().GetCurrentSongInfo() };
		if (!song.is_cue)
		{
			m_lyric_path = CPlayer::GetInstance().GetCurrentDir() + CPlayer::GetInstance().GetFileName();
			int index = m_lyric_path.rfind(L'.');
			m_lyric_path = m_lyric_path.substr(0, index);
		}
		else
		{
			m_lyric_path = CPlayer::GetInstance().GetCurrentDir() + song.artist + L" - " + song.title;
		}
		m_lyric_path += L".lrc";
		m_original_lyric_path = m_lyric_path;
		SetDlgItemText(IDC_LYRIC_PATH_EDIT2, m_lyric_path.c_str());
		path = m_lyric_path.c_str();
	}
	bool char_connot_convert;
	string lyric_str = CCommon::UnicodeToStr(m_lyric_string, code_type, &char_connot_convert);
	if (char_connot_convert)	//���ļ��а���Unicode�ַ�ʱ��ѯ���û��Ƿ�Ҫѡ��һ��Unicode�����ʽ�ٱ���
	{
		CString info;
		info.LoadString(IDS_STRING103);		//��string table�����ַ���
		if (MessageBox(info, NULL, MB_OKCANCEL | MB_ICONWARNING) != IDOK) return false;		//����û������ȡ����ť���򷵻�false
	}
	ofstream out_put{ path, std::ios::binary };
	out_put << lyric_str;
	m_modified = false;
	m_lyric_saved = true;
	UpdateStatusbarInfo();
	return true;
}

void CLyricEditDlg::UpdateStatusbarInfo()
{
	CString str;
	//��ʾ�ַ���
	str = CCommon::LoadTextFormat(IDS_CHARACTER_TOTAL, { m_lyric_string.size() });
	m_status_bar.SetText(str, 0, 0);

	//��ʾ�Ƿ��޸�
	m_status_bar.SetText(m_modified ? CCommon::LoadText(IDS_MODIFIED) : CCommon::LoadText(IDS_UNMODIFIED), 1, 0);

	//��ʾ�����ʽ
	str = CCommon::LoadText(IDS_ENCODE_FORMAT, _T(": "));
	switch (m_code_type)
	{
	case CodeType::ANSI: str += _T("ANSI"); break;
	case CodeType::UTF8: str += _T("UTF8"); break;
	case CodeType::UTF8_NO_BOM: str += CCommon::LoadText(IDS_UTF8_NO_BOM); break;
	case CodeType::UTF16: str += _T("UTF16"); break;
	}
	m_status_bar.SetText(str, 2, 0);
}

void CLyricEditDlg::StatusBarSetParts(int width)
{
	int nParts[3] = { width - theApp.DPI(240), width - theApp.DPI(160), -1 }; //�ָ�ߴ�
	m_status_bar.SetParts(3, nParts); //�ָ�״̬��
}

void CLyricEditDlg::OpenLyric(const wchar_t * path)
{
	m_lyric_path = path;
	CLyrics lyrics{ m_lyric_path };					//���ļ�
	m_lyric_string = lyrics.GetLyricsString();
	m_code_type = lyrics.GetCodeType();
}

void CLyricEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_lyric_edit);
}


BEGIN_MESSAGE_MAP(CLyricEditDlg, CDialog)
	//ON_BN_CLICKED(IDC_INSERT_TAG_BUTTON, &CLyricEditDlg::OnBnClickedInsertTagButton)
	//ON_BN_CLICKED(IDC_REPLACE_TAG_BUTTON, &CLyricEditDlg::OnBnClickedReplaceTagButton)
	//ON_BN_CLICKED(IDC_DELETE_TAG__BUTTON, &CLyricEditDlg::OnBnClickedDeleteTag)
	//ON_BN_CLICKED(IDC_SAVE_LYRIC_BUTTON, &CLyricEditDlg::OnBnClickedSaveLyricButton)
	//ON_BN_CLICKED(IDC_SAVE_AS_BUTTON5, &CLyricEditDlg::OnBnClickedSaveAsButton5)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT1, &CLyricEditDlg::OnEnChangeEdit1)
	ON_WM_CLOSE()
	//ON_BN_CLICKED(IDC_OPEN_LYRIC_BUTTON, &CLyricEditDlg::OnBnClickedOpenLyricButton)
	ON_COMMAND(ID_LYRIC_OPEN, &CLyricEditDlg::OnLyricOpen)
	ON_COMMAND(ID_LYRIC_SAVE, &CLyricEditDlg::OnLyricSave)
	ON_COMMAND(ID_LYRIC_SAVE_AS, &CLyricEditDlg::OnLyricSaveAs)
	ON_COMMAND(ID_LYRIC_FIND, &CLyricEditDlg::OnLyricFind)
	ON_COMMAND(ID_LYRIC_REPLACE, &CLyricEditDlg::OnLyricReplace)
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, &CLyricEditDlg::OnFindReplace)
	ON_COMMAND(ID_FIND_NEXT, &CLyricEditDlg::OnFindNext)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_COMMAND(ID_LE_TRANSLATE_TO_SIMPLIFIED_CHINESE, &CLyricEditDlg::OnLeTranslateToSimplifiedChinese)
	ON_COMMAND(ID_LE_TRANSLATE_TO_TRANDITIONAL_CHINESE, &CLyricEditDlg::OnLeTranslateToTranditionalChinese)
	ON_COMMAND(ID_LYRIC_INSERT_TAG, &CLyricEditDlg::OnLyricInsertTag)
	ON_COMMAND(ID_LYRIC_REPLACE_TAG, &CLyricEditDlg::OnLyricReplaceTag)
	ON_COMMAND(ID_LYRIC_DELETE_TAG, &CLyricEditDlg::OnLyricDeleteTag)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
END_MESSAGE_MAP()


// CLyricEditDlg ��Ϣ�������


BOOL CLyricEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CenterWindow();

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// ����Сͼ��
	//��ȡ�����Ϣ
	//m_lyric_string = CPlayer::GetInstance().m_Lyrics.GetLyricsString();
	//m_lyric_path = CPlayer::GetInstance().m_Lyrics.GetPathName();
	OpenLyric(CPlayer::GetInstance().m_Lyrics.GetPathName().c_str());
	m_original_lyric_path = m_lyric_path;
	//m_code_type = CPlayer::GetInstance().m_Lyrics.GetCodeType();
	m_current_song_name = CPlayer::GetInstance().GetFileName();

	//��ʼ���༭������
	m_font.CreatePointFont(100, CCommon::LoadText(IDS_DEFAULT_FONT));
	m_lyric_edit.SetFont(&m_font);

	m_lyric_edit.SetWindowText(m_lyric_string.c_str());
	SetDlgItemText(IDC_LYRIC_PATH_EDIT2, m_lyric_path.c_str());

	////��ʼ����ʾ��Ϣ
	//m_Mytip.Create(this, TTS_ALWAYSTIP);
	//m_Mytip.AddTool(GetDlgItem(IDC_INSERT_TAG_BUTTON), _T("�ڹ�������е�����߲���һ��ʱ���ǩ����ݼ���F8"));
	//m_Mytip.AddTool(GetDlgItem(IDC_REPLACE_TAG_BUTTON), _T("�滻�������������ߵ�ʱ���ǩ����ݼ���F9"));
	//m_Mytip.AddTool(GetDlgItem(IDC_DELETE_TAG__BUTTON), _T("ɾ����괦��ʱ���ǩ����ݼ���Ctrl+Del"));
	//m_Mytip.AddTool(GetDlgItem(IDC_SAVE_LYRIC_BUTTON), _T("��ݼ���Ctrl+S"));
	//m_Mytip.AddTool(GetDlgItem(ID_PLAY_PAUSE), _T("��ݼ���Ctrl+P"));
	//m_Mytip.AddTool(GetDlgItem(ID_REW), _T("��ݼ���Ctrl+��"));
	//m_Mytip.AddTool(GetDlgItem(ID_FF), _T("��ݼ���Ctrl+��"));

	////��ȡ��ʼʱ���ڵĴ�С
	//CRect rect;
	//GetWindowRect(rect);
	//m_min_size.cx = rect.Width();
	//m_min_size.cy = rect.Height();

	//��ʼ��������
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE/* | CBRS_TOP*/ | CBRS_ALIGN_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_TOP
		/*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_LYRIC_EDIT_TOOLBAR))  //ָ��������ID��
	{
		TRACE0("Failed to create toolbar/n");
		return -1;      // fail to create
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	CImageList ImageList;
	ImageList.Create(theApp.DPI(20), theApp.DPI(20), ILC_COLOR32 | ILC_MASK, 2, 2);

	//ͨ��ImageList�������ͼ����Ϊ��������ͼ��
	//���ͼ��
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_ADD_TAG));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_REPLACE_TAG));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_DELETE_TAG));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_SAVE));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_PLAY_PAUSE));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_REW));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_FF));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_FIND));
	ImageList.Add(AfxGetApp()->LoadIcon(IDI_REPLACE));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&ImageList);
	ImageList.Detach();

	//���ù������߶�
	CRect rect1;
	m_wndToolBar.GetClientRect(rect1);
	rect1.bottom = rect1.top + TOOLBAR_HEIGHT;
	m_wndToolBar.MoveWindow(rect1);

	//��ʼ��״̬��
	CRect rect;
	GetClientRect(&rect);
	rect.top = rect.bottom - theApp.DPI(20);
	m_status_bar.Create(WS_VISIBLE | CBRS_BOTTOM, rect, this, 3);

	StatusBarSetParts(rect.Width());
	UpdateStatusbarInfo();

	m_dlg_exist = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLyricEditDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();

	//CDialog::OnCancel();
}


//void CLyricEditDlg::OnBnClickedInsertTagButton()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	OpreateTag(TagOpreation::INSERT);
//}
//
//
//void CLyricEditDlg::OnBnClickedReplaceTagButton()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	OpreateTag(TagOpreation::REPLACE);
//}
//
//
//void CLyricEditDlg::OnBnClickedDeleteTag()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	OpreateTag(TagOpreation::DELETE_);
//}


//void CLyricEditDlg::OnBnClickedSaveLyricButton()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	OnLyricSave();
//}


//void CLyricEditDlg::OnBnClickedSaveAsButton5()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CLyricEditDlg::OnDestroy()
{
	// TODO: �ڴ˴������Ϣ����������
	CDialog::OnDestroy();
	m_dlg_exist = false;
	if (m_current_song_name == CPlayer::GetInstance().GetFileName() && m_lyric_saved)		//�رո�ʱ༭����ʱ������ڲ��ŵĸ���û�б䣬��ִ�й���������������³�ʼ�����
		CPlayer::GetInstance().IniLyrics(m_original_lyric_path);
}


void CLyricEditDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString lyric_str;
	m_lyric_edit.GetWindowText(lyric_str);
	m_lyric_string = lyric_str;
	m_modified = true;
	UpdateStatusbarInfo();
}


BOOL CLyricEditDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		if (pMsg->wParam == VK_F8)
		{
			OpreateTag(TagOpreation::INSERT);
			return TRUE;
		}
		if (pMsg->wParam == VK_F9)
		{
			OpreateTag(TagOpreation::REPLACE);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == VK_DELETE)
		{
			OpreateTag(TagOpreation::DELETE_);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'S')
		{
			OnLyricSave();
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'P')
		{
			SendMessage(WM_COMMAND, ID_PLAY_PAUSE);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == VK_LEFT)
		{
			SendMessage(WM_COMMAND, ID_REW);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == VK_RIGHT)
		{
			SendMessage(WM_COMMAND, ID_FF);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'F')
		{
			OnLyricFind();
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'H')
		{
			OnLyricReplace();
			return TRUE;
		}
		if (pMsg->wParam == VK_F3)
		{
			OnFindNext();
			return TRUE;
		}
	}
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	//	return TRUE;
	//if (pMsg->message == WM_MOUSEMOVE)
	//	m_Mytip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


void CLyricEditDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_modified)
	{
		int rtn = MessageBox(CCommon::LoadText(IDS_LYRIC_SAVE_INRUARY), NULL, MB_YESNOCANCEL | MB_ICONWARNING);
		switch (rtn)
		{
		case IDYES: SaveLyric(m_lyric_path.c_str(), m_code_type);
		case IDNO: m_modified = false; break;
		default: return;
		}
	}

	CDialog::OnClose();
}


//void CLyricEditDlg::OnBnClickedOpenLyricButton()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CLyricEditDlg::OnLyricOpen()
{
	// TODO: �ڴ���������������
	if (m_modified)
	{
		int rtn = MessageBox(CCommon::LoadText(IDS_LYRIC_SAVE_INRUARY), NULL, MB_YESNOCANCEL | MB_ICONWARNING);
		switch (rtn)
		{
		case IDYES: SaveLyric(m_lyric_path.c_str(), m_code_type); m_modified = false; break;
		case IDNO: break;
		default: return;
		}
	}

	//���ù�����
	CString szFilter = CCommon::LoadText(IDS_LYRIC_FILE_FILTER);
	//������ļ��Ի���
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	//��ʾ���ļ��Ի���
	if (IDOK == fileDlg.DoModal())
	{
		//m_lyric_path = fileDlg.GetPathName();	//��ȡ�򿪵��ļ�·��
		OpenLyric(fileDlg.GetPathName());
		SetDlgItemText(IDC_LYRIC_PATH_EDIT2, m_lyric_path.c_str());
		m_lyric_edit.SetWindowText(m_lyric_string.c_str());
	}
}


void CLyricEditDlg::OnLyricSave()
{
	// TODO: �ڴ���������������
	if (m_modified)
		SaveLyric(m_lyric_path.c_str(), m_code_type);
}


void CLyricEditDlg::OnLyricSaveAs()
{
	// TODO: �ڴ���������������
	//���챣���ļ��Ի���
	CFileDialog fileDlg(FALSE, _T("txt"), m_lyric_path.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, CCommon::LoadText(IDS_LYRIC_FILE_FILTER), this);
	//Ϊ�����Ϊ���Ի������һ�����ѡ���
	fileDlg.AddComboBox(IDC_SAVE_COMBO_BOX);
	//Ϊ���ѡ��������Ŀ
	fileDlg.AddControlItem(IDC_SAVE_COMBO_BOX, 0, CCommon::LoadText(IDS_SAVE_AS_ANSI));
	fileDlg.AddControlItem(IDC_SAVE_COMBO_BOX, 1, CCommon::LoadText(IDS_SAVE_AS_UTF8));
	//Ϊ���ѡ�������Ĭ��ѡ�е���Ŀ
	DWORD default_selected{ 0 };
	if (m_code_type == CodeType::UTF8 || m_code_type == CodeType::UTF8_NO_BOM)
		default_selected = 1;
	fileDlg.SetSelectedControlItem(IDC_SAVE_COMBO_BOX, default_selected);

	//��ʾ�����ļ��Ի���
	if (IDOK == fileDlg.DoModal())
	{
		DWORD selected_item;
		fileDlg.GetSelectedControlItem(IDC_SAVE_COMBO_BOX, selected_item);	//��ȡ�������ʽ����ѡ�е���Ŀ
		CodeType save_code{};
		switch (selected_item)
		{
		case 0: save_code = CodeType::ANSI; break;
		case 1: save_code = CodeType::UTF8; break;
		default: break;
		}
		SaveLyric(fileDlg.GetPathName().GetString(), save_code);
	}
}


void CLyricEditDlg::OnLyricFind()
{
	// TODO: �ڴ���������������
	if (m_pFindDlg == nullptr)
	{
		m_pFindDlg = new CFindReplaceDialog;
		m_pFindDlg->Create(TRUE, NULL, NULL, FR_DOWN | FR_HIDEWHOLEWORD | FR_HIDEMATCHCASE, this);
	}
	m_pFindDlg->ShowWindow(SW_SHOW);
	m_pFindDlg->SetActiveWindow();
}


void CLyricEditDlg::OnLyricReplace()
{
	// TODO: �ڴ���������������
	if (m_pReplaceDlg == nullptr)
	{
		m_pReplaceDlg = new CFindReplaceDialog;
		m_pReplaceDlg->Create(FALSE, NULL, NULL, FR_DOWN | FR_HIDEWHOLEWORD | FR_HIDEMATCHCASE, this);
	}
	m_pReplaceDlg->ShowWindow(SW_SHOW);
	m_pReplaceDlg->SetActiveWindow();
}


afx_msg LRESULT CLyricEditDlg::OnFindReplace(WPARAM wParam, LPARAM lParam)
{
	if (m_pFindDlg != nullptr)
	{
		m_find_str = m_pFindDlg->GetFindString();
		m_find_down = (m_pFindDlg->SearchDown() != 0);
		if (m_pFindDlg->FindNext())		//������һ��ʱ
		{
			OnFindNext();
		}
		if (m_pFindDlg->IsTerminating())	//�رմ���ʱ
		{
			//m_pFindDlg->DestroyWindow();
			m_pFindDlg = nullptr;
		}
	}
	//delete m_pFindDlg;

	if (m_pReplaceDlg != nullptr)
	{
		m_find_str = m_pReplaceDlg->GetFindString();
		m_replace_str = m_pReplaceDlg->GetReplaceString();
		if (m_pReplaceDlg->FindNext())		//������һ��ʱ
		{
			OnFindNext();
		}
		if (m_pReplaceDlg->ReplaceCurrent())	//�滻��ǰʱ
		{
			if (m_find_flag)
			{
				m_lyric_string.replace(m_find_index, m_find_str.size(), m_replace_str.c_str(), m_replace_str.size());	//�滻�ҵ����ַ���
				m_lyric_edit.SetWindowText(m_lyric_string.c_str());
				m_modified = true;
				UpdateStatusbarInfo();
				OnFindNext();
				m_lyric_edit.SetSel(m_find_index, m_find_index + m_replace_str.size());	//ѡ���滻���ַ���
				SetActiveWindow();		//���༭���������û����
			}
			else
			{
				OnFindNext();
			}
		}
		if (m_pReplaceDlg->ReplaceAll())		//�滻ȫ��ʱ
		{
			int replace_count{};	//ͳ���滻���ַ����ĸ���
			while (true)
			{
				int index_offset = m_replace_str.size() - m_find_str.size();
				if (index_offset < 0)
					index_offset = 0;
				m_find_index = m_lyric_string.find(m_find_str, m_find_index + index_offset + 1);	//�����ַ���
				if (m_find_index == string::npos)
					break;
				m_lyric_string.replace(m_find_index, m_find_str.size(), m_replace_str.c_str(), m_replace_str.size());	//�滻�ҵ����ַ���
				replace_count++;
			}
			m_lyric_edit.SetWindowText(m_lyric_string.c_str());
			m_modified = true;
			UpdateStatusbarInfo();
			if (replace_count != 0)
			{
				CString info;
				info = CCommon::LoadTextFormat(IDS_REPLACE_COMPLETE_INFO, { replace_count });
				MessageBox(info, NULL, MB_ICONINFORMATION);
			}
		}
		if (m_pReplaceDlg->IsTerminating())
		{
			m_pReplaceDlg = nullptr;
		}
	}
	return 0;
}


void CLyricEditDlg::OnFindNext()
{
	// TODO: �ڴ���������������
	if (m_find_down)
		m_find_index = m_lyric_string.find(m_find_str, m_find_index + 1);	//������
	else
		m_find_index = m_lyric_string.rfind(m_find_str, m_find_index - 1);	//��ǰ����
	if (m_find_index == string::npos)
	{
		CString info;
		info = CCommon::LoadTextFormat(IDS_CONNOT_FIND_STRING, { m_find_str });
		MessageBox(info, NULL, MB_OK | MB_ICONINFORMATION);
		m_find_flag = false;
	}
	else
	{
		m_lyric_edit.SetSel(m_find_index, m_find_index + m_find_str.size());		//ѡ���ҵ����ַ���
		SetActiveWindow();		//���༭��������Ϊ�����
		m_lyric_edit.SetFocus();
		m_find_flag = true;
	}
}


void CLyricEditDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ƴ�����С��С
	lpMMI->ptMinTrackSize.x = theApp.DPI(300);		//������С���
	lpMMI->ptMinTrackSize.y = theApp.DPI(300);		//������С�߶�

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CLyricEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//���ڴ�С�仯ʱ����״̬���Ĵ�С��λ��
	if (nType != SIZE_MINIMIZED && m_status_bar.m_hWnd)
	{
		CRect rect;
		rect.right = cx;
		rect.bottom = cy;
		rect.top = rect.bottom - STATUSBAR_HEIGHT;
		m_status_bar.MoveWindow(rect);
		StatusBarSetParts(cx);
	}

	//�������ڵĴ�С��λ��
	if (nType != SIZE_MINIMIZED)
	{
		CRect rect;
		CWnd* plyric_path_wnd{ GetDlgItem(IDC_LYRIC_PATH_EDIT2) };
		if (plyric_path_wnd != nullptr)
		{
			plyric_path_wnd->GetWindowRect(rect);
			ScreenToClient(&rect);
			rect.right = cx - MARGIN;
			plyric_path_wnd->MoveWindow(rect);
		}

		if (m_lyric_edit.m_hWnd != NULL)
		{
			m_lyric_edit.GetWindowRect(rect);
			ScreenToClient(&rect);
			rect.right = cx - MARGIN;
			rect.bottom = cy - STATUSBAR_HEIGHT - MARGIN;
			m_lyric_edit.MoveWindow(rect);
		}

		if (m_wndToolBar.m_hWnd != NULL)
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = cx;
			rect.bottom = TOOLBAR_HEIGHT;
			m_wndToolBar.MoveWindow(rect);
		}
	}
}


void CLyricEditDlg::OnLeTranslateToSimplifiedChinese()
{
	// TODO: �ڴ���������������
	m_lyric_string = CCommon::TranslateToSimplifiedChinese(m_lyric_string);
	m_lyric_edit.SetWindowText(m_lyric_string.c_str());
	m_modified = true;
	UpdateStatusbarInfo();
}


void CLyricEditDlg::OnLeTranslateToTranditionalChinese()
{
	// TODO: �ڴ���������������
	m_lyric_string = CCommon::TranslateToTranditionalChinese(m_lyric_string);
	m_lyric_edit.SetWindowText(m_lyric_string.c_str());
	m_modified = true;
	UpdateStatusbarInfo();
}


void CLyricEditDlg::OnLyricInsertTag()
{
	// TODO: �ڴ���������������
	OpreateTag(TagOpreation::INSERT);
}


void CLyricEditDlg::OnLyricReplaceTag()
{
	// TODO: �ڴ���������������
	OpreateTag(TagOpreation::REPLACE);
}


void CLyricEditDlg::OnLyricDeleteTag()
{
	// TODO: �ڴ���������������
	OpreateTag(TagOpreation::DELETE_);
}

BOOL CLyricEditDlg::OnToolTipText(UINT, NMHDR * pNMHDR, LRESULT * pResult)
{
	TOOLTIPTEXT* pT = (TOOLTIPTEXT*)pNMHDR; //��pNMHDRת����TOOLTIPTEXTָ����������
	UINT nID = pNMHDR->idFrom;  //��ȡ�������ϰ�ť��ID
	CString tipInfo;
	switch (nID)
	{
	case ID_LYRIC_INSERT_TAG:
		tipInfo = CCommon::LoadText(IDS_INSERT_TIME_TAG_TIP);
		break;
	case ID_LYRIC_REPLACE_TAG:
		tipInfo = CCommon::LoadText(IDS_REPLACE_TIME_TAG_TIP);
		break;
	case ID_LYRIC_DELETE_TAG:
		tipInfo = CCommon::LoadText(IDS_DELETE_TIME_TAG_TIP);
		break;
	case ID_LYRIC_SAVE:
		tipInfo = CCommon::LoadText(IDS_SAVE_TIP);
		break;
	case ID_PLAY_PAUSE:
		tipInfo = CCommon::LoadText(IDS_PLAY_PAUSE_TIP);
		break;
	case ID_REW:
		tipInfo = CCommon::LoadText(IDS_REWIND_TIP);
		break;
	case ID_FF:
		tipInfo = CCommon::LoadText(IDS_FAST_FOWARD_TIP);
		break;
	case ID_LYRIC_FIND:
		tipInfo = CCommon::LoadText(IDS_FIND_TIP);
		break;
	case ID_LYRIC_REPLACE:
		tipInfo = CCommon::LoadText(IDS_REPLACE_TIP);
		break;
	}

	CCommon::WStringCopy(pT->szText, 80, tipInfo.GetString());

	return 0;
}
