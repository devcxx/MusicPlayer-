#include "stdafx.h"
#include "DrawCommon.h"


CDrawCommon::CDrawCommon()
{
}

CDrawCommon::~CDrawCommon()
{
}

void CDrawCommon::Create(CDC * pDC, CWnd * pMainWnd)
{
	m_pDC = pDC;
	m_pMainWnd = pMainWnd;
	if(m_pMainWnd != nullptr)
		m_pfont = m_pMainWnd->GetFont();
}

//void CDrawCommon::SetBackColor(COLORREF back_color)
//{
//	m_backColor = back_color;
//}

void CDrawCommon::SetFont(CFont * pfont)
{
	m_pfont = pfont;
}

void CDrawCommon::SetDC(CDC * pDC)
{
	m_pDC = pDC;
}

void CDrawCommon::DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, Alignment align, bool no_clip_area, bool multi_line, bool default_right_align)
{
	m_pDC->SetTextColor(color);
	m_pDC->SetBkMode(TRANSPARENT);
	if (m_pfont != nullptr)
		m_pDC->SelectObject(m_pfont);
	//���û�ͼ�ļ�������
	if (!no_clip_area)
	{
		SetDrawArea(m_pDC, rect);
	}
	CSize text_size = m_pDC->GetTextExtent(lpszString);
	//�ñ���ɫ����������
	//m_pDC->FillSolidRect(rect, m_backColor);
	UINT format{};
	switch (align)
	{
	case Alignment::RIGHT: format = DT_RIGHT; break;
	case Alignment::CENTER: format = DT_CENTER; break;
	}
	if (multi_line)
	{
		CRect text_rect{ rect };
		int height = m_pDC->DrawText(lpszString, text_rect, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		if (height < rect.Height())
			rect.top += (rect.Height() - height) / 2;
		rect.bottom = rect.top + height;
		m_pDC->DrawText(lpszString, rect, format | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
	}
	else
	{
		if (text_size.cx > rect.Width())		//����ı���ȳ����˾�������Ŀ�ȣ������˾���ʱ�����
		{
			format = (default_right_align ? DT_RIGHT : 0);
		}
		m_pDC->DrawText(lpszString, rect, format | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	}
}

void CDrawCommon::DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, bool center, bool no_clip_area)
{
	if (split < 0) split = 0;
	if (split > 1000) split = 1000;
	m_pDC->SetBkMode(TRANSPARENT);
	if (m_pfont != nullptr)
		m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	if (!no_clip_area)
	{
		SetDrawArea(m_pDC, rect);
	}
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//���㱳�����ֺ͸������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };		//�������ֵ�����
	CRect text_f_rect{ CPoint{ text_left, text_top }, CSize{ text_size.cx * split / 1000, text_size.cy } };		//�������ֵ�����
																												//����ı���ȴ��ڿؼ���ȣ���Ҫ���ݷָ��λ�ù����ı�
	if (text_size.cx > rect.Width())
	{
		//����ָ��λ�ã���ʽ��ȣ�ʣ�µĿ���Ѿ�С�ڿؼ���ȵ�һ�룬��ʱʹ�ı��Ҳ�Ϳؼ��Ҳ����
		if (text_rect.Width() - text_f_rect.Width() < rect.Width() / 2)
		{
			text_rect.MoveToX(rect.left - (text_rect.Width() - rect.Width()));
			text_f_rect.MoveToX(text_rect.left);
		}
		//�ָ�λ��ʣ�µĿ�Ȼ�û�е�С�ڿؼ���ȵ�һ�룬���Ƿָ�λ�õĿ���Ѿ����ڿؼ���ȵ�һ��ʱ����Ҫ�ƶ��ı�ʹ�ָ�λ�������ڿؼ����м�
		else if (text_f_rect.Width() > rect.Width() / 2)
		{
			text_rect.MoveToX(rect.left - (text_f_rect.Width() - rect.Width() / 2));
			text_f_rect.MoveToX(text_rect.left);
		}
		//�ָ�λ�û������ؼ���ȵ�һ��ʱ��ʹ�ı����Ϳؼ�������
		else
		{
			text_rect.MoveToX(rect.left);
			text_f_rect.MoveToX(rect.left);
		}
	}

	//�ñ���ɫ����������
	//m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->SetTextColor(color2);
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ʊ�������
	if (color1 != color2)
	{
		m_pDC->SetTextColor(color1);
		m_pDC->DrawText(lpszString, text_f_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ƹ�������
	}
}

void CDrawCommon::DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset)
{
	//static int shift_cnt;		//�ƶ��Ĵ���
	//static bool shift_dir;		//�ƶ��ķ�������Ϊfalse������Ϊtrue
	//static int freez;			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
	//static bool dir_changed{ false };	//����������˱仯����Ϊtrue
	if (reset)
	{
		scroll_info.shift_cnt = 0;
		scroll_info.shift_dir = false;
		scroll_info.freez = 20;
		scroll_info.dir_changed = false;
	}
	m_pDC->SetTextColor(color);
	m_pDC->SetBkMode(TRANSPARENT);
	if (m_pfont != nullptr)
		m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	SetDrawArea(m_pDC, rect);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//�������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };
	//����ı���ȴ��ڿؼ���ȣ��͹����ı�
	if (text_size.cx > rect.Width())
	{
		text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
		if ((text_rect.right < rect.right || text_rect.left > rect.left))		//�ƶ����߽�ʱ������
		{
			if (!scroll_info.dir_changed)
			{
				scroll_info.shift_dir = !scroll_info.shift_dir;
				scroll_info.freez = 20;		//�任����ʱ��΢��ͣ����һ��ʱ��
			}
			scroll_info.dir_changed = true;
		}
		else
		{
			scroll_info.dir_changed = false;
		}
	}
	//�ñ���ɫ����������
	//m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);
	if (scroll_info.freez <= 0)		//��freezΪ0��ʱ��Ź���
	{
		if (scroll_info.shift_dir)
			scroll_info.shift_cnt--;
		else
			scroll_info.shift_cnt++;
	}
	else
	{
		scroll_info.freez--;
	}
}

void CDrawCommon::DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo & scroll_info, bool reset)
{
	if (reset)
	{
		scroll_info.shift_cnt = 0;
		scroll_info.freez = 20;
	}
	m_pDC->SetTextColor(color);
	m_pDC->SetBkMode(TRANSPARENT);
	if (m_pfont != nullptr)
		m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	SetDrawArea(m_pDC, rect);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//�������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };
	//����ı���ȴ��ڿؼ���ȣ��͹����ı�
	if (text_size.cx > rect.Width())
	{
		text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
		if ((text_rect.right < rect.right || text_rect.left > rect.left))		//�ƶ������߽�ʱ��ͣ������freez��20��ʼ�ݼ�
		{
			scroll_info.shift_cnt--;	//���ı������ƶ�һ�Σ���ֹ�����ж�Ϊ�����߽�
			text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
			scroll_info.freez = 20;		//�任����ʱ��΢��ͣ����һ��ʱ��
		}
	}
	//�ñ���ɫ����������
	//m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);
	if (scroll_info.freez <= 0)		//��freezΪ0��ʱ��Ź���
	{
		scroll_info.shift_cnt++;
	}
	else
	{
		scroll_info.freez--;
		if (scroll_info.freez == 10)		//��freez�ݼ���һ��ʱ���ı���λ
			scroll_info.shift_cnt = 0;
	}
}

//void CDrawCommon::FillRect(CDC * pDC, CRect rect, COLORREF color)
//{
//	CBrush BGBrush, *pOldBrush;
//	BGBrush.CreateSolidBrush(color);
//	pOldBrush = pDC->SelectObject(&BGBrush);
//	pDC->FillRect(&rect, &BGBrush);
//	pDC->SelectObject(pOldBrush);
//	BGBrush.DeleteObject();
//}

void CDrawCommon::SetDrawArea(CDC * pDC, CRect rect)
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	pDC->SelectClipRgn(&rgn);
}

void CDrawCommon::SetDrawArea(CRect rect)
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	m_pDC->SelectClipRgn(&rgn);
}

void CDrawCommon::DrawBitmap(CBitmap & bitmap, CPoint start_point, CSize size, StretchMode stretch_mode)
{
	CDC memDC;

	//��ȡͼ��ʵ�ʴ�С
	BITMAP bm;
	GetObject(bitmap, sizeof(BITMAP), &bm);

	memDC.CreateCompatibleDC(m_pDC);
	memDC.SelectObject(&bitmap);
	// �������б���ͼƬʧ��
	m_pDC->SetStretchBltMode(HALFTONE);
	m_pDC->SetBrushOrg(0, 0);
	CSize draw_size;
	if (size.cx == 0 || size.cy == 0)		//���ָ����sizeΪ0����ʹ��λͼ��ʵ�ʴ�С����
	{
		draw_size = CSize(bm.bmWidth, bm.bmHeight);
	}
	else
	{
		draw_size = size;
		if (stretch_mode == StretchMode::FILL)
		{
			SetDrawArea(m_pDC, CRect(start_point, draw_size));
			float w_h_ratio, w_h_ratio_draw;		//ͼ��Ŀ�߱ȡ����ƴ�С�Ŀ�߱�
			w_h_ratio = static_cast<float>(bm.bmWidth) / bm.bmHeight;
			w_h_ratio_draw = static_cast<float>(size.cx) / size.cy;
			if (w_h_ratio > w_h_ratio_draw)		//���ͼ��Ŀ�߱ȴ��ڻ�������Ŀ�߱ȣ�����Ҫ�ü����ߵ�ͼ��
			{
				int image_width;		//���������ź�Ŀ��
				image_width = bm.bmWidth * draw_size.cy / bm.bmHeight;
				start_point.x -= ((image_width - draw_size.cx) / 2);
				draw_size.cx = image_width;
			}
			else
			{
				int image_height;		//���������ź�ĸ߶�
				image_height = bm.bmHeight * draw_size.cx / bm.bmWidth;
				start_point.y -= ((image_height - draw_size.cy) / 2);
				draw_size.cy = image_height;
			}
		}
		else if (stretch_mode == StretchMode::FIT)
		{
			draw_size = CSize(bm.bmWidth, bm.bmHeight);
			float w_h_ratio, w_h_ratio_draw;		//ͼ��Ŀ�߱ȡ����ƴ�С�Ŀ�߱�
			w_h_ratio = static_cast<float>(bm.bmWidth) / bm.bmHeight;
			w_h_ratio_draw = static_cast<float>(size.cx) / size.cy;
			if (w_h_ratio > w_h_ratio_draw)		//���ͼ��Ŀ�߱ȴ��ڻ�������Ŀ�߱�
			{
				draw_size.cy = draw_size.cy * size.cx / draw_size.cx;
				draw_size.cx = size.cx;
				start_point.y += ((size.cy - draw_size.cy) / 2);
			}
			else
			{
				draw_size.cx = draw_size.cx * size.cy / draw_size.cy;
				draw_size.cy = size.cy;
				start_point.x += ((size.cx - draw_size.cx) / 2);
			}
		}
	}

	m_pDC->StretchBlt(start_point.x, start_point.y, draw_size.cx, draw_size.cy, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	memDC.DeleteDC();
}

void CDrawCommon::DrawBitmap(UINT bitmap_id, CPoint start_point, CSize size, StretchMode stretch_mode)
{
	CDC memDC;
	CBitmap bitmap;
	bitmap.LoadBitmap(bitmap_id);
	DrawBitmap(bitmap, start_point, size, stretch_mode);
}

void CDrawCommon::DrawBitmap(HBITMAP hbitmap, CPoint start_point, CSize size, StretchMode stretch_mode)
{
	//CDC memDC;
	CBitmap bitmap;
	if (!bitmap.Attach(hbitmap))
		return;
	DrawBitmap(bitmap, start_point, size, stretch_mode);
	bitmap.Detach();
}

void CDrawCommon::DrawIcon(HICON hIcon, CPoint start_point, CSize size)
{
	if(size.cx==0||size.cy==0)
		::DrawIconEx(m_pDC->GetSafeHdc(), start_point.x, start_point.y, hIcon, 0, 0, 0, NULL, DI_NORMAL);
	else
		::DrawIconEx(m_pDC->GetSafeHdc(), start_point.x, start_point.y, hIcon, size.cx, size.cy, 0, NULL, DI_NORMAL);
}

void CDrawCommon::FillRect(CRect rect, COLORREF color, bool no_clip_area)
{
	if(!no_clip_area)
		SetDrawArea(m_pDC, rect);
	m_pDC->FillSolidRect(rect, color);
}

void CDrawCommon::FillAlphaRect(CRect rect, COLORREF color, BYTE alpha, bool no_clip_area)
{
	if (alpha == 0)
		return;
	if (alpha == 255)
	{
		FillRect(rect, color, no_clip_area);
		return;
	}
	if (!no_clip_area)
		SetDrawArea(m_pDC, rect);
	CDC cdc;
	if(!cdc.CreateCompatibleDC(m_pDC))
		return;

	CBitmap bitmap, *pOldBitmap;
	bitmap.CreateCompatibleBitmap(m_pDC, rect.Width(), rect.Height());
	CRect src(rect);
	src.MoveToXY(0, 0);
	pOldBitmap = cdc.SelectObject(&bitmap);
	cdc.FillSolidRect(src, color); //͸��ɫ

	if (::AlphaBlend == 0)
	{
		m_pDC->BitBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, &cdc, src.left, src.top, SRCINVERT);
	}
	else
	{
		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(bf));
		bf.SourceConstantAlpha = alpha; //͸���̶�//ֵԽ��Խ��͸��
		bf.BlendOp = AC_SRC_OVER;
		::AlphaBlend(m_pDC->GetSafeHdc(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			cdc.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf);
	}
	cdc.SelectObject(pOldBitmap);
}

void CDrawCommon::DrawRectTopFrame(CRect rect, COLORREF color, int pilex)
{
	SetDrawArea(m_pDC, rect);
	CPen aPen, *pOldPen;
	aPen.CreatePen(PS_SOLID, pilex, color);
	pOldPen = m_pDC->SelectObject(&aPen);

	m_pDC->MoveTo(rect.TopLeft());
	m_pDC->LineTo(rect.right, rect.top);

	m_pDC->SelectObject(pOldPen);
}

CSize CDrawCommon::GetTextExtent(LPCTSTR str)
{
	if (m_pfont != nullptr)
		m_pDC->SelectObject(m_pfont);
	return m_pDC->GetTextExtent(str);
}

bool CDrawCommon::BitmapStretch(CImage * pImage, CImage * ResultImage, CSize size)
{
	if (pImage->IsDIBSection())
	{
		// ȡ�� pImage �� DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC()); // Image ������Լ��� DC, ���Ա��ʹ�� FromHandle ȡ�Ì����� DC

		CBitmap *bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// �����µ� CImage
		//ResultImage->Create(size.cx, size.cy, bmpInfo.bmBitsPixel);
		ResultImage->Create(size.cx, size.cy, 24);		//���ǽ�Ŀ��ͼƬת����24λͼ
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		// �� Destination ���^С�ĕr��, ������ Destination DC �ϵ� Stretch Blt mode �Q���Ƿ�Ҫ�������h���c���YӍ
		ResultImageDC->SetStretchBltMode(HALFTONE); // ʹ�����Ʒ�|�ķ�ʽ
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL); // �{�� Brush �����c

		// �� pImage ���� ResultImage ����
		BOOL rtn = StretchBlt(*ResultImageDC, 0, 0, size.cx, size.cy, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		// pImage->Draw(*ResultImageDC,0,0,StretchWidth,StretchHeight,0,0,pImage->GetWidth(),pImage->GetHeight());

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
	return true;
}

HICON CDrawCommon::LoadIconResource(UINT id, int width, int height)
{
	return (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id), IMAGE_ICON, width, height, 0);
}
