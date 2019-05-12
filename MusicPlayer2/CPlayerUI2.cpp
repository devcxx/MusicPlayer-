#include "stdafx.h"
#include "CPlayerUI2.h"


CPlayerUI2::CPlayerUI2(UIData& ui_data, CWnd* pMainWnd)
	: CPlayerUIBase(ui_data, pMainWnd)
{
	//m_title_font.CreatePointFont(100, CCommon::LoadText(IDS_DEFAULT_FONT));
	//m_artist_font.CreatePointFont(90, CCommon::LoadText(IDS_DEFAULT_FONT));

}


CPlayerUI2::~CPlayerUI2()
{
}

void CPlayerUI2::_DrawInfo(bool reset)
{
	CRect draw_rect = m_draw_rect;
	draw_rect.MoveToXY(0, 0);

	if (!IsDrawNarrowMode())
	{
		bool right_lyric = (!m_ui_data.narrow_mode && !m_ui_data.show_playlist) || draw_rect.Width() > DPI(600);
		CRect info_rect{ draw_rect };

		//�����ռ�����ʾ���ſ�����
		if (draw_rect.Width() - 2 * EdgeMargin(true) < m_progress_on_top_threshold)		//����������Ŀ��С��һ��ֵ����������߶ȣ��Ա㽫��������ʾ�ڰ�ť�Ϸ�
			info_rect.bottom -= (EdgeMargin(false) + DPI(50));
		else
			info_rect.bottom -= (EdgeMargin(false) + DPI(36));
		if (right_lyric)
			info_rect.right = info_rect.left + info_rect.Width() / 2;

		wchar_t buff[64];
		CRect rc_tmp;

		//ȫ��ģʽʱ�����Ͻǻ���ʱ��
		if(m_ui_data.full_screen)
		{
			DrawCurrentTime();
		}

		//�������Ͻ�ͼ��
		int top_right_icon_size = DrawTopRightIcons();

		//���Ʋ���״̬
		int text_height{ DPI(18) };
		rc_tmp.MoveToXY(EdgeMargin(true), EdgeMargin(false));
		rc_tmp.right = draw_rect.right - EdgeMargin(true) - top_right_icon_size;
		rc_tmp.bottom = rc_tmp.top + text_height;
		DrawSongInfo(rc_tmp, reset);

		//������Ŀ��ʽ
		rc_tmp.MoveToX(EdgeMargin(true));
		rc_tmp.MoveToY(rc_tmp.bottom);
		const BASS_CHANNELINFO channel_info{ CPlayer::GetInstance().GetChannelInfo() };
		CString chans_str;
		if (channel_info.chans == 1)
			chans_str = CCommon::LoadText(IDS_MONO);
		else if (channel_info.chans == 2)
			chans_str = CCommon::LoadText(IDS_STEREO);
		else if (channel_info.chans > 2)
			chans_str.Format(CCommon::LoadText(_T("%d "), IDS_CHANNEL), channel_info.chans);
		if (!CPlayer::GetInstance().IsMidi())
		{
			swprintf_s(buff, L"%s %.1fkHz %dkbps %s", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f,
				CPlayer::GetInstance().GetCurrentSongInfo().bitrate, chans_str.GetString());
		}
		else
		{
			const MidiInfo& midi_info{ CPlayer::GetInstance().GetMidiInfo() };
			swprintf_s(buff, L"%s %.1fkHz %s %dbpm %d/%d", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f,
				chans_str.GetString(), midi_info.speed, midi_info.midi_position, midi_info.midi_length);
		}

		static CDrawCommon::ScrollInfo scroll_info2;
		m_draw.DrawScrollText(rc_tmp, buff, m_colors.color_text, DPI(1.5), false, scroll_info2, reset);

		//����ר�������λ��
		int bottom_height;		//ר������ײ�����ͼ��ѯ�ʵľ���
		if (!right_lyric)
			bottom_height = static_cast<int>(info_rect.Height() * 0.41);
		else
			bottom_height = static_cast<int>(info_rect.Height() * 0.35);

		CRect cover_frame_rect{ CPoint(EdgeMargin(true), text_height * 2 + (EdgeMargin(false) - Margin())), CSize(info_rect.Width() - 2 * EdgeMargin(true), info_rect.Height() - text_height * 2 - bottom_height - (EdgeMargin(false) - Margin())) };
		int cover_side = min(cover_frame_rect.Width(), cover_frame_rect.Height());
		CPoint start_point;
		start_point.x = cover_frame_rect.left + (cover_frame_rect.Width() - cover_side) / 2;
		start_point.y = cover_frame_rect.top + (cover_frame_rect.Height() - cover_side) / 2;
		CRect cover_rect{ start_point, CSize(cover_side, cover_side) };
		cover_rect.DeflateRect(DPI(12), DPI(12));

		//���Ʊ���
		rc_tmp = cover_rect;
		//rc_tmp.bottom += Margin() / 2;
		if (IsDrawBackgroundAlpha())
			m_draw.FillAlphaRect(rc_tmp, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
		else
			m_draw.FillRect(rc_tmp, m_colors.color_spectrum_back);

		//����ר������
		cover_rect.DeflateRect(Margin() / 2, Margin() / 2);
		m_buttons[BTN_COVER].rect = DrawAreaToClient(cover_rect, m_draw_rect);
		m_draw_data.thumbnail_rect = cover_rect;
		if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
		{
			m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), cover_rect.TopLeft(), cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
		}
		else
		{
			CRect rect = cover_rect;
			int cover_margin = static_cast<int>(cover_rect.Width() * 0.13);
			rect.DeflateRect(cover_margin, cover_margin);
			m_draw.DrawIcon(theApp.m_icon_set.default_cover.GetIcon(), rect.TopLeft(), rect.Size());
		}

		////���Ʋ��Ž���
		//CRect progress_rect = cover_rect;
		//progress_rect.top = cover_rect.bottom;
		//progress_rect.bottom = progress_rect.top + Margin() / 2;
		//double progress = static_cast<double>(CPlayer::GetInstance().GetCurrentPosition()) / CPlayer::GetInstance().GetSongLength();
		//progress_rect.right = progress_rect.left + static_cast<int>(progress * cover_rect.Width());
		//if(progress_rect.right>progress_rect.left)
		//	m_draw.FillRect(progress_rect, m_colors.color_spectrum);

		int text_height2 = DPI(22);

		//����Ƶ�׷���
		CRect rc_spectrum_area;
		rc_spectrum_area.MoveToXY(EdgeMargin(true), info_rect.bottom - bottom_height + text_height2);
		rc_spectrum_area.right = info_rect.right - EdgeMargin(true);

		if (theApp.m_app_setting_data.show_spectrum)
		{
			int spectrum_height;
			if (!right_lyric)
				spectrum_height = max(text_height2 + static_cast<int>(info_rect.Height() * 0.1), info_rect.bottom - rc_spectrum_area.top - DPI(128));
			else
				spectrum_height = info_rect.bottom - rc_spectrum_area.top - DPI(32);

			rc_spectrum_area.bottom = rc_spectrum_area.top + spectrum_height;

			if (spectrum_height > text_height2 + DPI(6))
			{
				m_draw.SetDrawArea(m_draw.GetDC(), rc_spectrum_area);
				rc_spectrum_area.left += static_cast<int>(info_rect.Width()*0.09);
				rc_spectrum_area.right -= static_cast<int>(info_rect.Width()*0.05);

				//����Ƶ����������߶�
				CRect rc_spectrum{ rc_spectrum_area };
				const int max_height = max(rc_spectrum_area.Width() / 3, DPI(78));
				if (rc_spectrum.Height() > max_height)
				{
					rc_spectrum.top += (rc_spectrum_area.Height() * 2 / 3 - max_height * 2 / 3);
					rc_spectrum.bottom = rc_spectrum.top + max_height;
				}

				CRect rc_spectrum_top = rc_spectrum;
				rc_spectrum_top.bottom = rc_spectrum.top + (rc_spectrum.Height() * 2 / 3);

				const int ROWS = 64;		//Ҫ��ʾ��Ƶ�����ε�����
				int gap_width{ info_rect.Width() / 200 };		//Ƶ�����μ�϶���
				CRect rects[ROWS];
				int width = (rc_spectrum_top.Width() - (ROWS - 1)*gap_width) / (ROWS - 1);
				rects[0] = rc_spectrum_top;
				rects[0].right = rects[0].left + width;
				for (int i{ 1 }; i < ROWS; i++)
				{
					rects[i] = rects[0];
					rects[i].left += (i * (width + gap_width));
					rects[i].right += (i * (width + gap_width));
				}
				for (int i{}; i < ROWS; i++)
				{
					float spetral_data = CPlayer::GetInstance().GetSpectralData()[i];
					float peak_data = CPlayer::GetInstance().GetSpectralPeakData()[i];

					CRect rect_tmp{ rects[i] };
					int spetral_height = static_cast<int>(spetral_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
					int peak_height = static_cast<int>(peak_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
					if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
					if (peak_height <= 0 || CPlayer::GetInstance().IsError()) peak_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
					rect_tmp.top = rect_tmp.bottom - spetral_height;
					if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
					m_draw.FillRect(rect_tmp, m_colors.color_spectrum, true);
					//���Ƶ�Ӱ
					CRect rc_invert = rect_tmp;
					rc_invert.bottom = rect_tmp.top + rect_tmp.Height() * 2 / 3;
					rc_invert.MoveToY(rect_tmp.bottom + gap_width);
					m_draw.FillAlphaRect(rc_invert, m_colors.color_spectrum, 96, true);

					//���ƶ���
					CRect rect_peak{ rect_tmp };
					rect_peak.bottom = rect_tmp.bottom - peak_height - gap_width;
					rect_peak.top = rect_peak.bottom - max(theApp.DPIRound(1.1), gap_width / 2);
					m_draw.FillRect(rect_peak, m_colors.color_spectrum, true);
					////���ƶ��˵�Ӱ
					//CRect rc_peak_invert = rect_peak;
					//rc_peak_invert.MoveToY(rc_invert.top + peak_height + theApp.DPIRound(1.1));
					//m_draw.FillAlphaRect(rc_peak_invert, m_colors.color_spectrum, 96);
				}
			}
		}
		else
		{
			if (!right_lyric)
				rc_spectrum_area.bottom = rc_spectrum_area.top + text_height2;
			else
				rc_spectrum_area.bottom = info_rect.bottom - DPI(32);
		}


		//���Ʊ����������
		rc_tmp.MoveToXY(EdgeMargin(true), info_rect.bottom - bottom_height);
		rc_tmp.right = info_rect.right - EdgeMargin(true);
		rc_tmp.bottom = rc_tmp.top + text_height2;
		m_draw.SetFont(&theApp.m_font_set.title.GetFont(theApp.m_ui_data.full_screen));
		static CDrawCommon::ScrollInfo scroll_info_title;
		m_draw.DrawScrollText(rc_tmp, CPlayer::GetInstance().GetCurrentSongInfo().title.c_str(), m_colors.color_text, DPI(1), true, scroll_info_title, reset);

		rc_tmp.MoveToY(rc_tmp.bottom);
		m_draw.SetFont(&theApp.m_font_set.normal.GetFont(theApp.m_ui_data.full_screen));
		static CDrawCommon::ScrollInfo scroll_info_artist;
		m_draw.DrawScrollText(rc_tmp, CPlayer::GetInstance().GetCurrentSongInfo().artist.c_str(), m_colors.color_text, DPI(1), true, scroll_info_artist, reset);

		//���ƿ�����
		rc_tmp.MoveToY(rc_spectrum_area.bottom + DPI(4));
		rc_tmp.bottom = rc_tmp.top + DPI(24);
		rc_tmp.right = draw_rect.right - EdgeMargin(true);
		DrawToolBar(rc_tmp, true);

		//m_draw_data.info_rect = m_draw_rect;
		//m_draw_data.info_rect.bottom = m_draw_data.info_rect.top + rc_tmp.bottom;
		//m_draw_data.lyric_rect = m_draw_rect;
		//m_draw_data.lyric_rect.top = m_draw_data.info_rect.bottom + 1;

		//���Ƹ��
		//bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };

		int lyric_margin;
		if (!right_lyric)
		{
			rc_tmp.MoveToX(EdgeMargin(true));
			rc_tmp.MoveToY(rc_tmp.bottom + Margin());
			rc_tmp.right = info_rect.right - EdgeMargin(true);
			rc_tmp.bottom = info_rect.bottom - Margin();

			lyric_margin = Margin();
		}
		else
		{
			rc_tmp.MoveToX(info_rect.right);
			rc_tmp.MoveToY(2 * text_height2 + EdgeMargin(false));
			rc_tmp.right = draw_rect.right - EdgeMargin(true);
			rc_tmp.bottom = rc_spectrum_area.bottom;

			lyric_margin = 2 * Margin();
		}

		m_draw_data.lyric_rect = rc_tmp;
		m_draw_data.lyric_rect.InflateRect(Margin(), Margin());

		CRect background_rect = rc_tmp;

		rc_tmp.DeflateRect(lyric_margin, lyric_margin);
		if (rc_tmp.bottom > rc_tmp.top + m_lyric_text_height / 2)
		{
			if (theApp.m_app_setting_data.lyric_background)
			{
				if (IsDrawBackgroundAlpha())
					m_draw.FillAlphaRect(background_rect, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
				else
					m_draw.FillRect(background_rect, m_colors.color_lyric_back);
			}

			DrawLryicCommon(rc_tmp);
		}

		//��������������ť
		DrawVolumnAdjBtn();

		//���Ʋ��ſ��ư�ť
		rc_tmp = draw_rect;
		rc_tmp.left += EdgeMargin(true);
		rc_tmp.right -= EdgeMargin(true);
		rc_tmp.top = info_rect.bottom;
		rc_tmp.bottom -= EdgeMargin(false);
		DrawControlBar(rc_tmp);
	}

	//խ����ģʽʱ
	else
	{
		CRect info_rect{ draw_rect };
		info_rect.bottom -= DPI(36);

		if (m_ui_data.full_screen)
		{
			DrawCurrentTime();
		}

		//����ר������
		CRect rc_tmp = info_rect;
		const int cover_side = DPI(150);
		rc_tmp.DeflateRect(EdgeMargin(true), EdgeMargin(false));
		rc_tmp.right = rc_tmp.left + cover_side;
		rc_tmp.bottom = rc_tmp.top + cover_side;
		if (IsDrawBackgroundAlpha())
			m_draw.FillAlphaRect(rc_tmp, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
		else
			m_draw.FillRect(rc_tmp, m_colors.color_spectrum_back);

		rc_tmp.DeflateRect(Margin() / 2, Margin() / 2);
		m_buttons[BTN_COVER].rect= DrawAreaToClient(rc_tmp, m_draw_rect);
		m_draw_data.thumbnail_rect = rc_tmp;
		if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
		{
			m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), rc_tmp.TopLeft(), rc_tmp.Size(), theApp.m_app_setting_data.album_cover_fit);
		}
		else
		{
			CRect rect = rc_tmp;
			int cover_margin = static_cast<int>(rc_tmp.Width() * 0.13);
			rect.DeflateRect(cover_margin, cover_margin);
			m_draw.DrawIcon(theApp.m_icon_set.default_cover.GetIcon(), rect.TopLeft(), rect.Size());
		}

		//�������Ͻ�ͼ��
		int top_right_icon_size = DrawTopRightIcons();


		//���Ʋ���״̬
		int text_height{ DPI(18) };		//�ı��ĸ߶�
		rc_tmp.MoveToX(cover_side + EdgeMargin(true) + Margin());
		rc_tmp.MoveToY(EdgeMargin(false));
		rc_tmp.right = info_rect.right - EdgeMargin(true) - top_right_icon_size;
		rc_tmp.bottom = rc_tmp.top + text_height;
		DrawSongInfo(rc_tmp, reset);

		//���Ʊ����������
		int text_height2 = DPI(22);
		rc_tmp.MoveToXY(cover_side + EdgeMargin(true) + Margin(), rc_tmp.bottom + DPI(4));
		rc_tmp.right = info_rect.right - EdgeMargin(true) - top_right_icon_size;
		rc_tmp.bottom = rc_tmp.top + text_height2;
		m_draw.SetFont(&theApp.m_font_set.title.GetFont(theApp.m_ui_data.full_screen));
		static CDrawCommon::ScrollInfo scroll_info_title;
		m_draw.DrawScrollText(rc_tmp, CPlayer::GetInstance().GetCurrentSongInfo().title.c_str(), m_colors.color_text, DPI(1), true, scroll_info_title, reset);

		rc_tmp.MoveToY(rc_tmp.bottom);
		m_draw.SetFont(&theApp.m_font_set.normal.GetFont(theApp.m_ui_data.full_screen));
		static CDrawCommon::ScrollInfo scroll_info_artist;
		m_draw.DrawScrollText(rc_tmp, CPlayer::GetInstance().GetCurrentSongInfo().artist.c_str(), m_colors.color_text, DPI(1), true, scroll_info_artist, reset);

		//���ƹ�����
		rc_tmp.MoveToY(rc_tmp.bottom + DPI(4));
		rc_tmp.bottom = rc_tmp.top + DPI(24);
		rc_tmp.right = info_rect.right - EdgeMargin(true);
		DrawToolBar(rc_tmp, true);

		//���Ƹ��
		rc_tmp.MoveToY(rc_tmp.bottom + Margin());
		rc_tmp.bottom = cover_side + EdgeMargin(false);

		if (theApp.m_app_setting_data.lyric_background)
		{
			if (IsDrawBackgroundAlpha())
				m_draw.FillAlphaRect(rc_tmp, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
			else
				m_draw.FillRect(rc_tmp, m_colors.color_lyric_back);
		}
		m_draw_data.lyric_rect = rc_tmp;
		rc_tmp.DeflateRect(Margin(), m_layout.margin);
		DrawLryicCommon(rc_tmp);

		//��������������ť
		DrawVolumnAdjBtn();

		//�沥���ƿ�����
		rc_tmp.top = cover_side + EdgeMargin(false) + Margin();
		rc_tmp.left = EdgeMargin(true);
		rc_tmp.right = draw_rect.right - EdgeMargin(true);
		rc_tmp.bottom = draw_rect.bottom - EdgeMargin(false);
		DrawControlBar(rc_tmp);
	}
}


void CPlayerUI2::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->AddTool(m_pMainWnd, str, m_buttons[btn].rect, btn + 2000);
}

void CPlayerUI2::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->UpdateTipText(str, m_pMainWnd, btn + 2000);
}

void CPlayerUI2::UpdateToolTipPosition()
{
	for (const auto& btn : m_buttons)
	{
		m_tool_tip->SetToolRect(m_pMainWnd, btn.first + 2000, btn.second.rect);
	}
}
