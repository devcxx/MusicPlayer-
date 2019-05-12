#include "stdafx.h"
#include "CortanaLyric.h"
#include "PlayListCtrl.h"
#include "CPlayerUIBase.h"


CCortanaLyric::CCortanaLyric()
{
}


CCortanaLyric::~CCortanaLyric()
{
	if (m_pDC != nullptr)
		m_cortana_wnd->ReleaseDC(m_pDC);
}

void CCortanaLyric::Init()
{
	if (m_enable)
	{
		HWND hTaskBar = ::FindWindow(_T("Shell_TrayWnd"), NULL);	//�������ľ��
		HWND hCortanaBar = ::FindWindowEx(hTaskBar, NULL, _T("TrayDummySearchControl"), NULL);	//Cortana���ľ�������а���3���Ӵ��ڣ�
		m_cortana_hwnd = ::FindWindowEx(hCortanaBar, NULL, _T("Button"), NULL);	//Cortana������������Ϊ��Button���Ĵ��ڵľ��
		m_hCortanaStatic = ::FindWindowEx(hCortanaBar, NULL, _T("Static"), NULL);		//Cortana������������Ϊ��Static���Ĵ��ڵľ��
		if (m_cortana_hwnd == NULL) return;
		wchar_t buff[32];
		::GetWindowText(m_cortana_hwnd, buff, 31);		//��ȡCortana��������ԭ�����ַ����������ڳ����˳�ʱ�ָ�
		m_cortana_default_text = buff;
		m_cortana_wnd = CWnd::FromHandle(m_cortana_hwnd);		//��ȡCortana�������CWnd���ָ��
		if (m_cortana_wnd == nullptr) return;

		::GetClientRect(hCortanaBar, m_cortana_rect);	//��ȡCortana������ľ�������

		CRect cortana_rect;
		CRect cortana_static_rect;		//Cortana��������static�ؼ��ľ�������
		::GetWindowRect(hCortanaBar, cortana_rect);
		::GetWindowRect(m_hCortanaStatic, cortana_static_rect);	//��ȡCortana��������static�ؼ��ľ�������

		m_cover_width = cortana_static_rect.left - cortana_rect.left;
		if (m_cover_width < m_cortana_rect.Height() / 2)
			m_cover_width = m_cortana_rect.Height();

		m_pDC = m_cortana_wnd->GetDC();
		m_draw.Create(m_pDC, m_cortana_wnd);


		//��ȡ�������С���Ƿ�Ϊ��ɫģʽ�Ĳ����������
		m_check_dark_point.x = cortana_rect.right - 2;
		m_check_dark_point.y = cortana_rect.top + 2;

		CheckDarkMode();
		SetUIColors();
		
		//��������
		LOGFONT lf;
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);		//��ȡϵͳĬ������
		if (m_default_font.m_hObject)
			m_default_font.DeleteObject();
		m_default_font.CreatePointFont(110, lf.lfFaceName);

		InitFont();

		//ΪCortana����������һ��͸��ɫ��ʹCortana������͸��
#ifndef COMPILE_IN_WIN_XP
		if(theApp.m_nc_setting_data.cortana_opaque)
		{
			SetWindowLong(hCortanaBar, GWL_EXSTYLE, GetWindowLong(hCortanaBar, GWL_EXSTYLE) | WS_EX_LAYERED);
			::SetLayeredWindowAttributes(hCortanaBar, theApp.m_nc_setting_data.cortana_transparent_color, 0, LWA_COLORKEY);
		}
#endif // !COMPILE_IN_WIN_XP

	}
}

void CCortanaLyric::InitFont()
{
	theApp.m_font_set.cortana.SetFont(theApp.m_lyric_setting_data.cortana_font);
	FontInfo translate_font = theApp.m_lyric_setting_data.cortana_font;
	translate_font.size--;
	theApp.m_font_set.cortana_translate.SetFont(translate_font);
}

void CCortanaLyric::SetEnable(bool enable)
{
	m_enable = enable;
}

void CCortanaLyric::DrawInfo()
{
	if (!m_enable)
		return;

	bool is_midi_lyric = CPlayerUIBase::IsMidiLyric();

	//��ʹ�ü���ģʽ��ʾ��ʣ�ֱ����С���������ڻ�ͼ
	if(!theApp.m_lyric_setting_data.cortana_lyric_compatible_mode)
	{
		m_draw.SetFont(&theApp.m_font_set.cortana.GetFont());
		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//ʹ��m_draw��ͼ
		m_draw.SetDC(&MemDC);
		m_draw.FillRect(m_cortana_rect, m_colors.back_color);

		if(theApp.m_lyric_setting_data.cortana_show_spectrum)
			DrawSpectrum();
		
		if (is_midi_lyric)
		{
			wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
			DrawCortanaTextSimple(current_lyric.c_str(), Alignment::LEFT);
		}
		else
		{
			Time time{ CPlayer::GetInstance().GetCurrentPosition() };
			int progress = CPlayer::GetInstance().m_Lyrics.GetLyricProgress(time);
			CLyrics::Lyric lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0);
			bool no_lyric{ false };
			//�����ǰһ����Ϊ�գ��ҳ����˳�����20�룬����ʾ���
			no_lyric = (lyric.text.empty() && CPlayer::GetInstance().GetCurrentPosition() - lyric.time.time2int() > 20000) || progress >= 1000;

			if (!CPlayer::GetInstance().m_Lyrics.IsEmpty() && !no_lyric)		//�и��ʱ��ʾ���
			{
				if (CPlayer::GetInstance().m_Lyrics.IsTranslated() && theApp.m_ui_data.show_translate)
				{
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					if (lyric.translate.empty()) lyric.translate = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					DrawLyricWithTranslate(lyric.text.c_str(), lyric.translate.c_str(), progress);
				}
				else if (!theApp.m_lyric_setting_data.cortana_lyric_double_line)
				{
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					DrawCortanaText(lyric.text.c_str(), progress);
				}
				else
				{
					wstring next_lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 1).text;
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
					if (next_lyric.empty()) next_lyric = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
					DrawLyricDoubleLine(lyric.text.c_str(), next_lyric.c_str(), progress);
				}
			}
			else			//û�и��ʱ��Cortana���������Թ����ķ�ʽ��ʾ��ǰ���Ÿ������ļ���
			{
				static int index{};
				static wstring song_name{};
				//�����ǰ���ŵĸ��������仯��DrawCortanaText�����ĵ�2����Ϊtrue�������ù���λ��
				if (index != CPlayer::GetInstance().GetIndex() || song_name != CPlayer::GetInstance().GetFileName())
				{
					DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str()), true, theApp.DPI(2));
					index = CPlayer::GetInstance().GetIndex();
					song_name = CPlayer::GetInstance().GetFileName();
				}
				else
				{
					DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str()), false, theApp.DPI(2));
				}
			}
		}

		//����Ƶ�ף�����Ƶ�׷�ֵʹCortanaͼ����ʾ��̬Ч��
		float spectrum_avr{};		//ȡǰ��N��Ƶ��Ƶ��ֵ��ƽ��ֵ
		const int N = 8;
		for (int i{}; i < N; i++)
			spectrum_avr += CPlayer::GetInstance().GetFFTData()[i];
		spectrum_avr /= N;
		int spetraum = static_cast<int>(spectrum_avr * 4000);		//�����˺ź������ֵ���Ե���Cortanaͼ������ʱ���ŵĴ�С
		SetBeatAmp(spetraum);
		//��ʾר�����棬���û��ר�����棬����ʾCortanaͼ��
		AlbumCoverEnable(theApp.m_lyric_setting_data.cortana_show_album_cover/* && CPlayer::GetInstance().AlbumCoverExist()*/);
		DrawAlbumCover(CPlayer::GetInstance().GetAlbumCover());
	
		if (!m_colors.dark && !theApp.m_nc_setting_data.cortana_opaque)		//����ɫģʽ�£��������������Ʊ߿�
		{
			CRect rect{ m_cortana_rect };
			rect.left += m_cover_width;
			m_draw.DrawRectTopFrame(rect, m_border_color);
		}
		CDrawCommon::SetDrawArea(m_pDC, m_cortana_rect);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_pDC->BitBlt(0, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}

	//ʹ�ü���ģʽ��ʾ��ʣ���С�������������ı�
	else
	{
		CWnd* pWnd = CWnd::FromHandle(m_hCortanaStatic);
		if (pWnd != nullptr)
		{
			static wstring str_disp_last;
			wstring str_disp;
			if (is_midi_lyric)
			{
				str_disp = CPlayer::GetInstance().GetMidiLyric();
			}
			else if (!CPlayer::GetInstance().m_Lyrics.IsEmpty())		//�и��ʱ��ʾ���
			{
				Time time{ CPlayer::GetInstance().GetCurrentPosition() };
				str_disp = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0).text;
				if (str_disp.empty())
					str_disp = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
			}
			else
			{
				//û�и��ʱ��ʾ��ǰ���Ÿ���������
				str_disp = CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")).GetString() + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format);
			}

			if(str_disp != str_disp_last)
			{
				pWnd->SetWindowText(str_disp.c_str());
				pWnd->Invalidate();
				str_disp_last = str_disp;
			}
		}
	}
}

void CCortanaLyric::DrawCortanaTextSimple(LPCTSTR str, Alignment align)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CRect text_rect{ TextRect() };
		m_draw.DrawWindowText(text_rect, str, m_colors.info_text_color, align, false, false, true);
	}
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, bool reset, int scroll_pixel)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		static CDrawCommon::ScrollInfo cortana_scroll_info;
		CRect text_rect{ TextRect() };
		m_draw.DrawScrollText(text_rect, str, m_colors.info_text_color, scroll_pixel, false, cortana_scroll_info, reset);
	}
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CRect text_rect{ TextRect() };
		if(theApp.m_lyric_setting_data.lyric_karaoke_disp)
			m_draw.DrawWindowText(text_rect, str, m_colors.text_color, m_colors.text_color2, progress, false);
		else
			m_draw.DrawWindowText(text_rect, str, m_colors.text_color, m_colors.text_color, progress, false);
	}
}

void CCortanaLyric::DrawLyricDoubleLine(LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_draw.SetFont(&theApp.m_font_set.cortana.GetFont());
		static bool swap;
		static int last_progress;
		if (last_progress > progress)
		{
			swap = !swap;
		}
		last_progress = progress;

		CRect text_rect{ TextRect() };
		CRect up_rect{ text_rect }, down_rect{ text_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
		up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
		down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
		//������һ���ʵ��ı�������Ҫ�Ŀ�ȣ��Ӷ�ʵ����һ�и���Ҷ���
		int width;
		if (!swap)
			width = m_draw.GetTextExtent(next_lyric).cx;
		else
			width = m_draw.GetTextExtent(lyric).cx;
		if(width<m_cortana_rect.Width())
			down_rect.left = down_rect.right - width;

		//m_draw.FillRect(m_cortana_rect, m_colors.back_color);
		if (!swap)
		{
			if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
				m_draw.DrawWindowText(up_rect, lyric, m_colors.text_color, m_colors.text_color2, progress, false);
			else
				m_draw.DrawWindowText(up_rect, lyric, m_colors.text_color, m_colors.text_color, progress, false);

			m_draw.DrawWindowText(down_rect, next_lyric, m_colors.text_color2);
		}
		else
		{
			m_draw.DrawWindowText(up_rect, next_lyric, m_colors.text_color2);

			if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
				m_draw.DrawWindowText(down_rect, lyric, m_colors.text_color, m_colors.text_color2, progress, false);
			else
				m_draw.DrawWindowText(down_rect, lyric, m_colors.text_color, m_colors.text_color, progress, false);
		}
	}
}

void CCortanaLyric::DrawLyricWithTranslate(LPCTSTR lyric, LPCTSTR translate, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CRect text_rect{ TextRect() };
		CRect up_rect{ text_rect }, down_rect{ text_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
		up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
		down_rect.top = down_rect.bottom - (down_rect.Height() / 2);

		//m_draw.FillRect(m_cortana_rect, m_colors.back_color);
		m_draw.SetFont(&theApp.m_font_set.cortana.GetFont());
		if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
			m_draw.DrawWindowText(up_rect, lyric, m_colors.text_color, m_colors.text_color2, progress, false);
		else
			m_draw.DrawWindowText(up_rect, lyric, m_colors.text_color, m_colors.text_color, progress, false);

		m_draw.SetFont(&theApp.m_font_set.cortana_translate.GetFont());
		m_draw.DrawWindowText(down_rect, translate, m_colors.text_color, m_colors.text_color, progress, false);
	}
}

void CCortanaLyric::DrawAlbumCover(const CImage & album_cover)
{
	if (m_enable)
	{
		CRect cover_rect = CoverRect();
		m_draw.SetDrawArea(cover_rect);
		if (album_cover.IsNull() || !m_show_album_cover)
		{
			int cortana_img_id{ m_colors.dark ? IDB_CORTANA_BLACK : IDB_CORTANA_WHITE };
			if (theApp.m_lyric_setting_data.cortana_icon_beat)
			{
				m_draw.FillRect(cover_rect, (m_colors.dark ? GRAY(47) : GRAY(240)));
				CRect rect{ cover_rect };
				rect.DeflateRect(theApp.DPI(4), theApp.DPI(4));
				int inflate;
				inflate = m_beat_amp * theApp.DPI(14) / 1000;
				rect.InflateRect(inflate, inflate);
				m_draw.DrawBitmap(cortana_img_id, rect.TopLeft(), rect.Size(), CDrawCommon::StretchMode::FIT);
			}
			else
			{
				m_draw.DrawBitmap(cortana_img_id, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FIT);
			}

			if(!m_colors.dark)
				m_draw.DrawRectTopFrame(cover_rect, m_border_color);
		}
		else
		{
			m_draw.DrawBitmap(album_cover, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
	}
}

void CCortanaLyric::DrawSpectrum()
{
	CRect rc_spectrum{ TextRect() };
	m_draw.SetDrawArea(rc_spectrum);
	rc_spectrum.right += theApp.DPI(8);

	const int ROWS = 64;		//Ҫ��ʾ��Ƶ�����ε�����
	int gap_width{ rc_spectrum.Width() / 160 };		//Ƶ�����μ�϶���
	CRect rects[ROWS];
	int width = (rc_spectrum.Width() - (ROWS - 1)*gap_width) / (ROWS - 1);		//ÿ�����εĿ��
	rects[0] = rc_spectrum;
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
		m_draw.FillAlphaRect(rect_tmp, m_colors.sprctrum_color, 140, true);

		//���ƶ���
		CRect rect_peak{ rect_tmp };
		rect_peak.bottom = rect_tmp.bottom - peak_height - gap_width;
		rect_peak.top = rect_peak.bottom - max(theApp.DPIRound(1.1), gap_width / 2);
		m_draw.FillAlphaRect(rect_peak, m_colors.sprctrum_color, 140, true);
	}
}

CRect CCortanaLyric::TextRect() const
{
	CRect text_rect{ m_cortana_rect };
	text_rect.left += m_cover_width;
	text_rect.DeflateRect(theApp.DPI(4), theApp.DPI(2));
	text_rect.top -= theApp.DPI(1);
	return text_rect;
}

CRect CCortanaLyric::CoverRect() const
{
	CRect cover_rect = m_cortana_rect;
	cover_rect.right = cover_rect.left + m_cover_width;
	return cover_rect;
}

void CCortanaLyric::ResetCortanaText()
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CWnd* pWnd = CWnd::FromHandle(m_hCortanaStatic);
		if (pWnd != nullptr)
		{
			CString str;
			pWnd->GetWindowText(str);
			if (str != m_cortana_default_text.c_str())
			{
				pWnd->SetWindowText(m_cortana_default_text.c_str());
				pWnd->Invalidate();
			}
		}
		
		if (!theApp.m_lyric_setting_data.cortana_lyric_compatible_mode)
		{
			m_draw.SetFont(&m_default_font);
			COLORREF color;		//CortanaĬ���ı�����ɫ
			color = (m_dark_mode ? GRAY(173) : GRAY(16));
			m_draw.SetDC(m_pDC);
			//�Ȼ���Cortanaͼ��
			CRect cover_rect = CoverRect();
			if (m_dark_mode)
				m_draw.DrawBitmap(IDB_CORTANA_BLACK, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
			else
				m_draw.DrawBitmap(IDB_CORTANA_WHITE, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
			//�ٻ���CortanaĬ���ı�
			CRect rect{ m_cortana_rect };
			rect.left += m_cover_width;
			m_draw.FillRect(rect, (m_dark_mode ? GRAY(47) : GRAY(240)));
			m_draw.DrawWindowText(rect, m_cortana_default_text.c_str(), color);
			if (!m_dark_mode)
			{
				rect.left -= m_cover_width;
				m_draw.DrawRectTopFrame(rect, m_border_color);
			}
			//m_cortana_wnd->Invalidate();
		}
	}
}

void CCortanaLyric::CheckDarkMode()
{
	if (m_enable)
	{
		HDC hDC = ::GetDC(NULL);
		COLORREF color;
		//��ȡCortana���Ͻǵ����ɫ
		color = ::GetPixel(hDC, m_check_dark_point.x, m_check_dark_point.y);
		int brightness;
		brightness = (GetRValue(color) + GetGValue(color) + GetBValue(color)) / 3;		//R��G��B��ƽ��ֵ
		m_dark_mode = (brightness < 220);
		::ReleaseDC(NULL, hDC);
	}
}

void CCortanaLyric::AlbumCoverEnable(bool enable)
{
	bool last_enable;
	last_enable = m_show_album_cover;
	m_show_album_cover = enable;
	if (last_enable && !enable)
	{
		CRect cover_rect = CoverRect();
		CDrawCommon::SetDrawArea(m_pDC, cover_rect);
		m_pDC->FillSolidRect(cover_rect, m_colors.back_color);
	}
}

void CCortanaLyric::SetBeatAmp(int beat_amp)
{
	m_beat_amp = beat_amp;
	if (m_beat_amp < 0) m_beat_amp = 0;
	if (m_beat_amp > 2000) m_beat_amp = 2000;
}

void CCortanaLyric::SetUIColors()
{
	if (theApp.m_lyric_setting_data.cortana_color == 1)
		m_colors.dark = true;
	else if (theApp.m_lyric_setting_data.cortana_color == 2)
		m_colors.dark = false;
	else
		m_colors.dark = m_dark_mode;

	if (m_colors.dark)
	{
		m_colors.text_color = theApp.m_app_setting_data.theme_color.light3;
		m_colors.text_color2 = theApp.m_app_setting_data.theme_color.light1;
		m_colors.info_text_color = theApp.m_app_setting_data.theme_color.light3;
		m_colors.sprctrum_color = theApp.m_app_setting_data.theme_color.light1;

		DWORD dwStyle = GetWindowLong(m_hCortanaStatic, GWL_STYLE);
		if ((dwStyle & WS_VISIBLE) != 0)		//����Cortana��������static�ؼ��Ƿ���WS_VISIBLE����Ϊ��ͼ�������ò�ͬ�ı���ɫ
			m_colors.back_color = GRAY(47);	//���û�ͼ�ı�����ɫ
		else
			m_colors.back_color = GRAY(10);	//���û�ͼ�ı�����ɫ
	}
	else
	{
		m_colors.text_color = theApp.m_app_setting_data.theme_color.dark3;
		m_colors.text_color2 = theApp.m_app_setting_data.theme_color.dark1;
		m_colors.info_text_color = theApp.m_app_setting_data.theme_color.dark2;
		m_colors.back_color = GRAY(240);
		m_colors.sprctrum_color = theApp.m_app_setting_data.theme_color.dark1;
	}
}
