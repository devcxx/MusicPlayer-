#include "stdafx.h"
#include "CMiniModeUI.h"
#include "PlayListCtrl.h"


CMiniModeUI::CMiniModeUI(SMiniModeUIData& ui_data, CWnd* pMaineWnd)
	: m_ui_data(ui_data), CPlayerUIBase(theApp.m_ui_data, pMaineWnd)
{
	m_font_time.CreatePointFont(80, CCommon::LoadText(IDS_DEFAULT_FONT));
}


CMiniModeUI::~CMiniModeUI()
{
}

void CMiniModeUI::Init(CDC * pDC)
{
	CPlayerUIBase::Init(pDC);
	m_first_draw = true;
}

bool CMiniModeUI::PointInControlArea(CPoint point) const
{
	bool point_in_control = false;
	for (const auto& btn : m_buttons)
	{
		if(btn.first!=BTN_COVER)
			point_in_control |= (btn.second.rect.PtInRect(point) != FALSE);
	}
	return point_in_control;
}

void CMiniModeUI::_DrawInfo(bool reset)
{
	//����ר������
	int cover_side = m_ui_data.window_height - 2 * m_ui_data.margin;
	CRect cover_rect{CPoint(m_ui_data.margin, m_ui_data.margin), CSize(cover_side, cover_side)};
	if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
	{
		m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), cover_rect.TopLeft(), cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
	}
	else		//ר�����治����ʱ��ʾĬ��ר������ͼ��
	{
		if (IsDrawBackgroundAlpha())
			m_draw.FillAlphaRect(cover_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
		else
			m_draw.FillRect(cover_rect, m_colors.color_spectrum_back);

		//ʹͼ���ھ��ο��о���
		CSize icon_size = theApp.m_icon_set.default_cover_small.GetSize();
		CRect icon_rect;
		icon_rect.left = cover_rect.left + (cover_rect.Width() - icon_size.cx) / 2;
		icon_rect.right = icon_rect.left + icon_size.cx;
		icon_rect.top = cover_rect.top + (cover_rect.Height() - icon_size.cy) / 2;
		icon_rect.bottom = icon_rect.top + icon_size.cy;

		m_draw.DrawIcon(theApp.m_icon_set.default_cover_small.GetIcon(), icon_rect.TopLeft(), icon_rect.Size());
	}
	m_buttons[BTN_COVER].rect = cover_rect;

	//���Ʋ��ſ��ư�ť
	CRect rc_tmp;
	rc_tmp.MoveToXY(m_ui_data.window_height, m_ui_data.margin);
	rc_tmp.right = rc_tmp.left + theApp.DPI(27);
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(22);
	DrawUIButton(rc_tmp, m_buttons[BTN_PREVIOUS], theApp.m_icon_set.previous);

	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	if(CPlayer::GetInstance().IsPlaying())
		DrawUIButton(rc_tmp, m_buttons[BTN_PLAY_PAUSE], theApp.m_icon_set.pause);
	else
		DrawUIButton(rc_tmp, m_buttons[BTN_PLAY_PAUSE], theApp.m_icon_set.play);

	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	DrawUIButton(rc_tmp, m_buttons[BTN_NEXT], theApp.m_icon_set.next);


	//����Ƶ�׷���
	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	rc_tmp.right = rc_tmp.left + theApp.DPI(30);

	//if (IsDrawBackgroundAlpha())
	//	m_draw.FillAlphaRect(rc_tmp, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	//else
	//	m_draw.FillRect(rc_tmp, m_colors.color_spectrum_back);
	m_draw.SetDrawArea(rc_tmp);

	static const int DATA_ROW{ 16 };			//Ƶ���������ݵ�����������Ϊ2�������η���С�ڻ����SPECTRUM_ROW��
	float spectral_data[DATA_ROW]{};
	float spectral_data_peak[DATA_ROW]{};
	CRect rects[DATA_ROW];		//ÿ�����εľ�������

	int width{ rc_tmp.Width()/9 };	//ÿ�����εĿ��
	int gap{ rc_tmp.Width() / 22 };	//���εļ��
	rects[0] = rc_tmp;
	rects[0].right = rects[0].left + width - gap;
	for (int i{ 1 }; i < DATA_ROW; i++)
	{
		rects[i] = rects[0];
		rects[i].left += (i * width);
		rects[i].right += (i * width);
	}

	for (int i{}; i < 64; i++)
	{
		spectral_data[i / (64 / DATA_ROW)] += CPlayer::GetInstance().GetSpectralData()[i];
		spectral_data_peak[i / (64 / DATA_ROW)] += CPlayer::GetInstance().GetSpectralPeakData()[i];
	}
	for (int i{}; i < DATA_ROW; i++)
	{
		CRect rect_tmp{ rects[i] };
		int spetral_height = static_cast<int>(spectral_data[i] * rc_tmp.Height() / 60 * theApp.m_app_setting_data.sprctrum_height / 100);
		int peak_height = static_cast<int>(spectral_data_peak[i] * rc_tmp.Height() / 60 * theApp.m_app_setting_data.sprctrum_height / 100);
		if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
		rect_tmp.top = rect_tmp.bottom - spetral_height;
		if (rect_tmp.top < 0) rect_tmp.top = 0;
		m_draw.FillRect(rect_tmp, m_colors.color_spectrum, true);

		CRect rc_peak = rect_tmp;
		rc_peak.bottom = rect_tmp.bottom - peak_height - theApp.DPI(2);
		rc_peak.top = rc_peak.bottom - theApp.DPI(1);
		m_draw.FillRect(rc_peak, m_colors.color_spectrum, true);
	}

	//���Ʋ���ʱ��
	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	rc_tmp.right = m_ui_data.widnow_width - 3 * theApp.DPI(20) - 4 * m_ui_data.margin;
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(16);
	CString str;
	if (m_ui_data.m_show_volume)
		str.Format(CCommon::LoadText(IDS_VOLUME, _T(": %d%%")), CPlayer::GetInstance().GetVolume());
	else if (CPlayer::GetInstance().IsError())
		str = CCommon::LoadText(IDS_PLAY_ERROR);
	else
		str = CPlayer::GetInstance().GetTimeString().c_str();
	m_draw.SetFont(&m_font_time);
	m_draw.DrawWindowText(rc_tmp, str, m_colors.color_text, Alignment::CENTER);
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());

	//���ƽ�����
	rc_tmp.MoveToY(rc_tmp.bottom);
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(6);
	CRect progress_rect = rc_tmp;
	int progress_height = theApp.DPI(2);
	progress_rect.top = progress_rect.top + (rc_tmp.Height() - progress_height) / 2;
	progress_rect.bottom = progress_rect.top + progress_height;

	if (IsDrawBackgroundAlpha())
		m_draw.FillAlphaRect(progress_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(progress_rect, m_colors.color_spectrum_back);

	m_buttons[BTN_PROGRESS].rect = progress_rect;
	m_buttons[BTN_PROGRESS].rect.InflateRect(0, theApp.DPI(2));

	double progress = static_cast<double>(CPlayer::GetInstance().GetCurrentPosition()) / CPlayer::GetInstance().GetSongLength();
	progress_rect.right = progress_rect.left + static_cast<int>(progress * progress_rect.Width());
	if (progress_rect.right > progress_rect.left)
		m_draw.FillRect(progress_rect, m_colors.color_spectrum);

	//�������Ͻǰ�ť
	rc_tmp.right = m_ui_data.widnow_width - m_ui_data.margin;
	rc_tmp.left = rc_tmp.right - theApp.DPI(20);;
	rc_tmp.top = m_ui_data.margin;
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(20);
	DrawTextButton(rc_tmp, m_buttons[BTN_CLOSE], _T("��"));

	rc_tmp.MoveToX(rc_tmp.left - rc_tmp.Width() - m_ui_data.margin);
	DrawTextButton(rc_tmp, m_buttons[BTN_RETURN], _T("��"));

	rc_tmp.MoveToX(rc_tmp.left - rc_tmp.Width() - m_ui_data.margin);
	DrawTextButton(rc_tmp, m_buttons[BTN_SHOW_PLAYLIST], _T("��"));

	//������ʾ�ı���Ϣ
	rc_tmp.MoveToXY(m_ui_data.window_height, m_ui_data.margin + theApp.DPI(22));
	rc_tmp.right = m_ui_data.widnow_width - m_ui_data.margin;
	rc_tmp.bottom = m_ui_data.window_height;
	if (CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric())
	{
		wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
		m_draw.DrawWindowText(rc_tmp, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER);
	}
	else
	{
		Time time{ CPlayer::GetInstance().GetCurrentPosition() };
		CLyrics::Lyric current_lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0) };	//��ȡ�����
		int progress{ CPlayer::GetInstance().m_Lyrics.GetLyricProgress(time) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
		bool no_lyric{ false };
		//�����ǰһ����Ϊ�գ��ҳ����˳�����20�룬����ʾ���
		no_lyric = (current_lyric.text.empty() && CPlayer::GetInstance().GetCurrentPosition() - current_lyric.time.time2int() > 20000) || progress >= 1000;
		
		if (CPlayer::GetInstance().m_Lyrics.IsEmpty() || no_lyric)	//û�и��ʱ��ʾ���ŵ��ļ���
		{
			//���ڲ��ŵ��ļ����Թ�������ʽ��ʾ���������Ҫ��ǿ��ˢ�£������ù���λ��
			static CDrawCommon::ScrollInfo scroll_info;
			m_draw.DrawScrollText(rc_tmp, CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str(),
				m_colors.color_text, theApp.DPI(1), true, scroll_info, reset);
		}
		else		//��ʾ���
		{
			COLORREF color2 = (theApp.m_lyric_setting_data.lyric_karaoke_disp ? m_colors.color_text_2 : m_colors.color_text);
			if (current_lyric.text.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
				current_lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
			m_draw.DrawWindowText(rc_tmp, current_lyric.text.c_str(), m_colors.color_text, color2, progress, true);
		}
	}
}

void CMiniModeUI::RButtonUp(CPoint point)
{
}

void CMiniModeUI::LButtonUp(CPoint point)
{
	for (auto& btn : m_buttons)
	{
		btn.second.hover = false;
		btn.second.pressed = false;
		
		if (btn.second.rect.PtInRect(point))
		{
			switch (btn.first)
			{
			case BTN_PREVIOUS:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PREVIOUS);
				break;
			case BTN_PLAY_PAUSE:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PLAY_PAUSE);
				break;
			case BTN_NEXT:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_NEXT);
				break;
			case BTN_SHOW_PLAYLIST:
				m_pMainWnd->SendMessage(WM_COMMAND, ID_SHOW_PLAY_LIST);
				break;
			case BTN_RETURN:
				//m_buttons[BTN_RETURN].hover = false;
				m_pMainWnd->SendMessage(WM_COMMAND, IDOK);
				break;
			case BTN_CLOSE:
				if (theApp.m_general_setting_data.minimize_to_notify_icon)
					m_pMainWnd->ShowWindow(HIDE_WINDOW);
				else
					m_pMainWnd->SendMessage(WM_COMMAND, ID_MINI_MODE_EXIT);
				break;
			default:
				break;
			case BTN_COVER:
				break;
			case BTN_PROGRESS:
			{
				int ckick_pos = point.x - btn.second.rect.left;
				double progress = static_cast<double>(ckick_pos) / btn.second.rect.Width();
				CPlayer::GetInstance().SeekTo(progress);
			}
				break;
			}

		}
	}
}

CRect CMiniModeUI::GetThumbnailClipArea()
{
	return CRect();
}

void CMiniModeUI::UpdateSongInfoTip(LPCTSTR str_tip)
{
	UpdateMouseToolTip(BTN_COVER, str_tip);
}

void CMiniModeUI::UpdatePlayPauseButtonTip()
{
	if (CPlayer::GetInstance().IsPlaying() && !CPlayer::GetInstance().IsError())
		UpdateMouseToolTip(BTN_PLAY_PAUSE, CCommon::LoadText(IDS_PAUSE));
	else
		UpdateMouseToolTip(BTN_PLAY_PAUSE, CCommon::LoadText(IDS_PLAY));
}

void CMiniModeUI::PreDrawInfo()
{
	//������ɫ
	m_colors = CPlayerUIHelper::GetUIColors(theApp.m_app_setting_data.theme_color);
	//���û�ͼ����
	m_draw_rect = CRect(CPoint(0, 0), CSize(m_ui_data.widnow_width, m_ui_data.window_height));
}

void CMiniModeUI::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->AddTool(m_pMainWnd, str, m_buttons[btn].rect, btn + 1);
}

void CMiniModeUI::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
{
	//if (m_buttons[btn].hover)
	//{
		m_tool_tip->UpdateTipText(str, m_pMainWnd, btn + 1);
	//}
}

void CMiniModeUI::UpdateToolTipPosition()
{
}

void CMiniModeUI::AddToolTips()
{
	AddMouseToolTip(BTN_PREVIOUS, CCommon::LoadText(IDS_PREVIOUS));
	AddMouseToolTip(BTN_PLAY_PAUSE, CPlayer::GetInstance().IsPlaying() ? CCommon::LoadText(IDS_PAUSE) : CCommon::LoadText(IDS_PLAY));
	AddMouseToolTip(BTN_NEXT, CCommon::LoadText(IDS_NEXT));
	AddMouseToolTip(BTN_SHOW_PLAYLIST, CCommon::LoadText(IDS_SHOW_HIDE_PLAYLIST));
	AddMouseToolTip(BTN_RETURN, CCommon::LoadText(IDS_BACK_TO_NARMAL));
	AddMouseToolTip(BTN_CLOSE, CCommon::LoadText(IDS_CLOSE));
	AddMouseToolTip(BTN_COVER, _T(""));
	AddMouseToolTip(BTN_PROGRESS, CCommon::LoadText(IDS_SEEK_TO));
}
