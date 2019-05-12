#include "stdafx.h"
#include "StaticEx.h"
#include "DrawCommon.h"


CStaticEx::CStaticEx()
{
}


CStaticEx::~CStaticEx()
{
}

void CStaticEx::SetTextColor(COLORREF text_color)
{
	m_text_color = text_color;
}

void CStaticEx::SetBackgroundColor(COLORREF back_color)
{
	m_back_color = back_color;
	m_transparent = false;
}
BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CStaticEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	CDrawCommon draw;
	draw.Create(&dc, this);

	CRect rect;
	GetClientRect(rect);
	CString str;
	GetWindowText(str);
	if(!m_transparent)
		draw.FillRect(rect, m_back_color);
	else
		DrawThemeParentBackground(m_hWnd, dc.GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
	draw.DrawWindowText(rect, str, m_text_color);
}
