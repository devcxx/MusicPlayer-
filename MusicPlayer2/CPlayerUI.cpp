#include "stdafx.h"
#include "CPlayerUI.h"


CPlayerUI::CPlayerUI(UIData& ui_data, CWnd* pMainWnd)
	: CPlayerUIBase(ui_data, pMainWnd)
{
}


CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::_DrawInfo(bool reset)
{
	CRect draw_rect{ m_draw_rect };
	draw_rect.MoveToXY(0, 0);

	CPoint text_start{ draw_rect.left + SpectralSize().cx + Margin() + EdgeMargin(true), draw_rect.top + EdgeMargin(false) };		//�ı�����ʼ����
	int text_height{ DPI(18) };		//�ı��ĸ߶�

	//ȫ��ģʽʱ�����Ͻǻ���ʱ��
	if (m_ui_data.full_screen)
	{
		DrawCurrentTime();
	}


	//�������Ͻ�ͼ��
	int top_right_icon_size = DrawTopRightIcons();

	//��ʾ������Ϣ
	m_draw.SetFont(&theApp.m_font_set.normal.GetFont(m_ui_data.full_screen));
	//m_draw.SetBackColor(color_back);
	CRect tmp{ text_start, CSize{1,text_height} };
	tmp.right = draw_rect.right - EdgeMargin(true) - top_right_icon_size;
	DrawSongInfo(tmp, reset);

	wchar_t buff[64];
	wstring lable1_str, lable1_content;
	wstring lable2_str, lable2_content;
	wstring lable3_str, lable3_content;
	wstring lable4_str, lable4_content;
	lable1_str = CCommon::LoadText(IDS_TITLE, _T(": "));
	lable1_content = CPlayer::GetInstance().GetCurrentSongInfo().title;
	if (CPlayer::GetInstance().IsMidi())
	{
		const MidiInfo& midi_info{ CPlayer::GetInstance().GetMidiInfo() };
		lable2_str = CCommon::LoadText(IDS_RHYTHM, _T(": "));
		swprintf_s(buff, L"%d/%d (%dbpm)", midi_info.midi_position, midi_info.midi_length, midi_info.speed);
		lable2_content = buff;

		//lable3_str = _T("�ٶȣ�");
		//swprintf_s(buff, L"%d bpm", midi_info.speed);
		//lable3_content = buff;

		lable3_str = CCommon::LoadText(IDS_SOUND_FONT, _T(": ")) ;
		lable3_content = CPlayer::GetInstance().GetSoundFontName();
	}
	else
	{
		lable2_str = CCommon::LoadText(IDS_ARTIST, _T(": "));
		lable2_content = CPlayer::GetInstance().GetCurrentSongInfo().artist;
		lable3_str = CCommon::LoadText(IDS_ALBUM, _T(": "));
		lable3_content = CPlayer::GetInstance().GetCurrentSongInfo().album;
	}
	lable4_str = CCommon::LoadText(IDS_FORMAT, _T(": "));
	const BASS_CHANNELINFO channel_info{ CPlayer::GetInstance().GetChannelInfo() };
	CString chans_str;
	if (channel_info.chans == 1)
		chans_str = CCommon::LoadText(IDS_MONO);
	else if (channel_info.chans == 2)
		chans_str = CCommon::LoadText(IDS_STEREO);
	else if (channel_info.chans > 2)
		chans_str.Format(CCommon::LoadText(_T("%d "), IDS_CHANNEL), channel_info.chans);
	if (!CPlayer::GetInstance().IsMidi())
		swprintf_s(buff, L"%s %.1fkHz %dkbps %s", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, CPlayer::GetInstance().GetCurrentSongInfo().bitrate, chans_str.GetString());
	else
		swprintf_s(buff, L"%s %.1fkHz %s", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, chans_str.GetString());
	lable4_content = buff;
	//��ʾ����
	tmp.MoveToXY(text_start.x, text_start.y + text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, lable1_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = draw_rect.right - EdgeMargin(true) - top_right_icon_size;
	static CDrawCommon::ScrollInfo scroll_info2;
	m_draw.DrawScrollText2(tmp, lable1_content.c_str(), m_colors.color_text, DPI(1), false, scroll_info2, reset);
	//��ʾ������
	tmp.MoveToXY(text_start.x, text_start.y + 2 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, lable2_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = draw_rect.right - EdgeMargin(true);
	static CDrawCommon::ScrollInfo scroll_info3;
	if (CPlayer::GetInstance().IsMidi())
		m_draw.DrawWindowText(tmp, lable2_content.c_str(), m_colors.color_text);
	else
		m_draw.DrawScrollText2(tmp, lable2_content.c_str(), m_colors.color_text, DPI(1), false, scroll_info3, reset);
	//��ʾ��Ƭ��
	tmp.MoveToXY(text_start.x, text_start.y + 3 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, lable3_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = draw_rect.right - EdgeMargin(true);
	static CDrawCommon::ScrollInfo scroll_info4;
	m_draw.DrawScrollText2(tmp, lable3_content.c_str(), m_colors.color_text, DPI(1), false, scroll_info4, reset);
	//��ʾ�ļ���ʽ�ͱ�����
	tmp.MoveToXY(text_start.x, text_start.y + 4 * text_height);
	tmp.right = tmp.left + DPI(52);
	m_draw.DrawWindowText(tmp, lable4_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + DPI(52));
	tmp.right = draw_rect.right - EdgeMargin(true);
	static CDrawCommon::ScrollInfo scroll_info5;
	m_draw.DrawScrollText2(tmp, lable4_content.c_str(), m_colors.color_text, DPI(1), false, scroll_info5, reset);

	//��ʾƵ�׷���
	CRect spectral_rect{ CPoint{draw_rect.left + EdgeMargin(true), draw_rect.top + EdgeMargin(false)}, SpectralSize() };
	//���Ʊ���
	if (IsDrawBackgroundAlpha())
		m_draw.FillAlphaRect(spectral_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(spectral_rect, m_colors.color_spectrum_back);

	CRect cover_rect = spectral_rect;
	cover_rect.DeflateRect(Margin() / 2, Margin() / 2);
	m_buttons[BTN_COVER].rect = DrawAreaToClient(cover_rect, m_draw_rect);
	if (theApp.m_app_setting_data.show_album_cover)
	{
		//����ר������
		if (CPlayer::GetInstance().AlbumCoverExist())
		{
			m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), cover_rect.TopLeft(), cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
		}
		else
		{
			int cover_side = cover_rect.Height() * 3 / 4;
			int x = cover_rect.left + (cover_rect.Width() - cover_side) / 2;
			int y = cover_rect.top + (cover_rect.Height() - cover_side) / 2;
			::DrawIconEx(m_draw.GetDC()->GetSafeHdc(), x, y, theApp.m_icon_set.default_cover.GetIcon(), cover_side, cover_side, 0, NULL, DI_NORMAL);
		}
	}

	if (theApp.m_app_setting_data.show_spectrum)
	{
		const int ROWS = 31;		//Ҫ��ʾ��Ƶ�����ε�����
		int gap_width{ theApp.DPIRound(1, 0.4) };		//Ƶ�����μ�϶���
		CRect rects[ROWS];
		int width = (spectral_rect.Width() - (ROWS - 1)*gap_width) / (ROWS - 1);
		rects[0] = spectral_rect;
		rects[0].DeflateRect(Margin() / 2, Margin() / 2);
		rects[0].right = rects[0].left + width;
		for (int i{ 1 }; i < ROWS; i++)
		{
			rects[i] = rects[0];
			rects[i].left += (i * (width + gap_width));
			rects[i].right += (i * (width + gap_width));
		}
		for (int i{}; i < ROWS; i++)
		{
			float spetral_data = (CPlayer::GetInstance().GetSpectralData()[i * 2] + CPlayer::GetInstance().GetSpectralData()[i * 2 + 1]) / 2;
			float peak_data = (CPlayer::GetInstance().GetSpectralPeakData()[i * 2] + CPlayer::GetInstance().GetSpectralPeakData()[i * 2 + 1]) / 2;

			CRect rect_tmp{ rects[i] };
			int spetral_height = static_cast<int>(spetral_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			int peak_height = static_cast<int>(peak_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			if (peak_height <= 0 || CPlayer::GetInstance().IsError()) peak_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			rect_tmp.top = rect_tmp.bottom - spetral_height;
			if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
			COLORREF color;
			if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
				color = m_colors.color_spectrum_cover;
			else
				color = m_colors.color_spectrum;
			m_draw.GetDC()->FillSolidRect(rect_tmp, color);

			CRect rect_peak{ rect_tmp };
			rect_peak.bottom = rect_tmp.bottom - peak_height - theApp.DPIRound(1.1);
			rect_peak.top = rect_peak.bottom - theApp.DPIRound(1.1);
			//if (peak_height > 1)
			m_draw.GetDC()->FillSolidRect(rect_peak, color);
		}
	}

	//���ƹ�����
	CPoint point{ spectral_rect.left, spectral_rect.bottom };
	point.y += 2 * Margin();
	CRect other_info_rect{ point, CSize(draw_rect.Width() - 2 * EdgeMargin(true), DPI(24)) };
	DrawToolBar(other_info_rect, false);

	//��ʾ���
	CRect lyric_rect;
	if (IsDrawNarrowMode())
	{
		lyric_rect = other_info_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + Margin());
		if (lyric_rect.Width() >= m_progress_on_top_threshold)		//����������㹻���½���������ʾ�ڰ�ť�Ϸ������Ҳ࣬������������и���ĸ߶�
			lyric_rect.bottom += theApp.DPI(16);

		//���Ʊ���
		if (theApp.m_app_setting_data.lyric_background)
		{
			if (IsDrawBackgroundAlpha())
				m_draw.FillAlphaRect(lyric_rect, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
			else
				m_draw.FillRect(lyric_rect, m_colors.color_lyric_back);
		}

		DrawLryicCommon(lyric_rect);
	}
	else
	{
		//if (CPlayer::GetInstance().IsPlaying() || reset)
		//{

		int control_bar_height;
		if (draw_rect.Width() - 2 * EdgeMargin(true) < m_progress_on_top_threshold)		//����������Ŀ��С��һ��ֵ����������߶ȣ��Ա㽫��������ʾ�ڰ�ť�Ϸ�
			control_bar_height = DPI(50);
		else
			control_bar_height = DPI(36);

		lyric_rect = draw_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + Margin() + control_bar_height);
		lyric_rect.bottom = m_draw_rect.Height()/* - Margin()*/;
		DrawLyricsArea(lyric_rect);
		//}
	}
	m_draw_data.lyric_rect = lyric_rect;
	m_draw_data.thumbnail_rect = draw_rect;
	if (!IsDrawNarrowMode())
	{
		m_draw_data.thumbnail_rect.bottom = lyric_rect.top;
	}

	//���Ʋ��ſ�����
	CRect rc_control_bar;
	if (IsDrawNarrowMode())
	{
		rc_control_bar.top = lyric_rect.bottom + Margin();
		rc_control_bar.left = Margin();
		rc_control_bar.right = draw_rect.right - Margin();
		rc_control_bar.bottom = draw_rect.bottom - Margin();
	}
	else
	{
		rc_control_bar.top = other_info_rect.bottom + Margin();
		rc_control_bar.left = EdgeMargin(true);
		rc_control_bar.right = draw_rect.right - EdgeMargin(true);
		rc_control_bar.bottom = lyric_rect.top;
	}
	DrawControlBar(rc_control_bar);

	//������������ť����Ϊ�������ϲ㣬���Ա�����������
	DrawVolumnAdjBtn();
}

void CPlayerUI::DrawLyricsArea(CRect lyric_rect)
{
	bool draw_background{ IsDrawBackgroundAlpha() };
	bool midi_lyric = IsMidiLyric();
	//��ʾ������㡱
	CRect tmp;
	tmp = lyric_rect;
	tmp.left += (Margin() + EdgeMargin(true));
	tmp.bottom = tmp.top + DPI(28);
	m_draw.SetFont(&theApp.m_font_set.normal.GetFont(m_ui_data.full_screen));
	m_draw.DrawWindowText(tmp, CCommon::LoadText(IDS_LYRIC_SHOW, _T(": ")), m_colors.color_text);
	//��ʾ���밴ť
	CRect translate_rect{ tmp };
	translate_rect.DeflateRect(DPI(4), DPI(4));
	translate_rect.right = lyric_rect.right - (Margin() + EdgeMargin(true));
	translate_rect.left = translate_rect.right - translate_rect.Height();
	DrawTranslateButton(translate_rect);

	//��������򱳾�ɫ
	CRect lyric_area = lyric_rect;
	lyric_area.DeflateRect(Margin() + EdgeMargin(true), 0);
	lyric_area.top += 2 * Margin();
	lyric_area.bottom -= (Margin() + EdgeMargin(false));
	lyric_area.top += DPI(20);
	if (theApp.m_app_setting_data.lyric_background)
	{
		if (draw_background)
			m_draw.FillAlphaRect(lyric_area, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
		else
			m_draw.FillRect(lyric_area, m_colors.color_lyric_back);
	}
	//���ø����������
	lyric_area.DeflateRect(2 * Margin(), 2 * Margin());
	//CDrawCommon::SetDrawArea(pDC, lyric_area);

	//���Ƹ���ı�
	DrawLryicCommon(lyric_area);
}

CSize CPlayerUI::SpectralSize()
{
	CSize size = m_layout.spectral_size;
	if (m_ui_data.full_screen)
	{
		size.cx = static_cast<int>(size.cx * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR);
		size.cy = static_cast<int>(size.cy * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR);
	}
	return size;
}

//void CPlayerUI::RButtonUp(CPoint point)
//{
//	CPlayerUIBase::RButtonUp(point);
//
//	if (m_buttons[BTN_REPETEMODE].rect.PtInRect(point))
//		return;
//
//	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
//	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
//
//	if (!m_draw_data.lyric_rect.PtInRect(point))
//	{
//		m_main_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, m_pMainWnd);
//	}
//	else
//	{
//		m_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, m_pMainWnd);
//	}
//
//}

void CPlayerUI::MouseMove(CPoint point)
{
	CPlayerUIBase::MouseMove(point);

	////��ʾר���������ʾ
	//if (theApp.m_nc_setting_data.show_cover_tip && theApp.m_app_setting_data.show_album_cover)
	//{
	//	CRect cover_rect{ m_draw_data.cover_rect };
	//	cover_rect.MoveToXY(m_draw_rect.left + m_draw_data.cover_rect.left, m_draw_rect.top + m_draw_data.cover_rect.top);
	//	bool show_cover_tip{ cover_rect.PtInRect(point) != FALSE };
	//	static bool last_show_cover_tip{ false };
	//	if (!last_show_cover_tip && show_cover_tip)
	//	{
	//		CString info;
	//		if (CPlayer::GetInstance().AlbumCoverExist())
	//		{
	//			info = CCommon::LoadText(IDS_ALBUM_COVER, _T(": "));
	//			//CFilePathHelper cover_path(CPlayer::GetInstance().GetAlbumCoverPath());
	//			//if (cover_path.GetFileNameWithoutExtension() == ALBUM_COVER_NAME)
	//			if (CPlayer::GetInstance().IsInnerCover())
	//			{
	//				info += CCommon::LoadText(IDS_INNER_ALBUM_COVER_TIP_INFO);
	//				switch (CPlayer::GetInstance().GetAlbumCoverType())
	//				{
	//				case 0: info += _T("jpg"); break;
	//				case 1: info += _T("png"); break;
	//				case 2: info += _T("gif"); break;
	//				}
	//			}
	//			else
	//			{
	//				info += CCommon::LoadText(IDS_OUT_IMAGE, _T("\r\n"));
	//				info += CPlayer::GetInstance().GetAlbumCoverPath().c_str();
	//			}
	//		}
	//		m_tool_tip->AddTool(m_pMainWnd, info);
	//		m_tool_tip->SetMaxTipWidth(DPI(400));
	//		m_tool_tip->Pop();
	//	}
	//	if (last_show_cover_tip && !show_cover_tip)
	//	{
	//		m_tool_tip->AddTool(m_pMainWnd, _T(""));
	//		m_tool_tip->Pop();
	//	}
	//	last_show_cover_tip = show_cover_tip;
	//}

}

void CPlayerUI::OnSizeRedraw(int cx, int cy)
{
	m_show_volume_adj = false;
	CPlayerUIBase::OnSizeRedraw(cx, cy);
}

//CRect CPlayerUI::GetThumbnailClipArea()
//{
//	//CRect info_rect;
//	//if (!IsDrawNarrowMode())
//	//	info_rect = CRect{ CPoint{ Margin(), Margin() + DPI(20) }, CSize{ m_ui_data.client_width / 2 - 2 * Margin(), m_layout.info_height2 - 3 * Margin() } };
//	//else
//	//	info_rect = CRect{ CPoint{ Margin(), DPI(20) }, CSize{ m_ui_data.client_width - 2 * Margin(), m_layout.info_height - 2 * Margin() } };
//
//	return info_rect;
//}

void CPlayerUI::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->AddTool(m_pMainWnd, str, m_buttons[btn].rect, btn + 1000);
}

void CPlayerUI::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->UpdateTipText(str, m_pMainWnd, btn + 1000);
}

void CPlayerUI::UpdateToolTipPosition()
{
	for (const auto& btn : m_buttons)
	{
		m_tool_tip->SetToolRect(m_pMainWnd, btn.first + 1000, btn.second.rect);
	}
}

