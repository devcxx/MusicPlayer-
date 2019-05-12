// ProgressStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "ProgressStatic.h"


// CProgressStatic

IMPLEMENT_DYNAMIC(CProgressStatic, CStatic)

CProgressStatic::CProgressStatic()
{
	m_toolTip.Create(this, TTS_ALWAYSTIP);
}

CProgressStatic::~CProgressStatic()
{
}


BEGIN_MESSAGE_MAP(CProgressStatic, CStatic)
	ON_WM_PAINT()
//	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CProgressStatic ��Ϣ�������




void CProgressStatic::SetRange(int range)
{
	m_range = range;
}

void CProgressStatic::SetPos(int pos)
{
	m_pos = pos;
	CRect rect;
	this->GetClientRect(&rect);
	int length;
	length = rect.Width() * m_pos / m_range;	//��ȡ��������ȵ�����ֵ
	//ֻ�е��������Ľ�����Ҫ�ı�ʱ��������ؼ����ȸı�ʱ�����»��ƣ�����ˢ�¹�Ƶ��
	if (length != m_last_progress_length)
	{
		m_last_progress_length = length;
		Invalidate();
	}
}

void CProgressStatic::SetColor(COLORREF color)
{
	m_progress_color = color;
	Invalidate();
}

void CProgressStatic::SetBackColor(COLORREF back_color)
{
	m_back_color = back_color;
	Invalidate();
}

void CProgressStatic::SetProgressBarHeight(int height)
{
	m_progress_height = height;
	Invalidate();
}

void CProgressStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	CRect rect;
	this->GetClientRect(&rect);
	int max_length = rect.Width();

	////���ڷ�windows10ʱΪ������������WS_CLIPCHILDREN���ԣ�����������Ҫ�ֶ��ػ�ؼ�����
	//if (!CWinVersionHelper::IsWindows10OrLater())
	//	DrawThemeParentBackground(m_hWnd, dc.GetSafeHdc(), &rect);	//�ػ�ؼ�����

	//�����εĸ߶ȸ���Ϊ��������ʾ�ĸ߶�
	if (m_progress_height < rect.Height() && m_progress_height > 0)
	{
		rect.top = (rect.Height() - m_progress_height) / 2;
		rect.bottom = rect.top + m_progress_height;
	}
	//��������ɫ
	CBrush BGBrush, *pOldBrush;
	BGBrush.CreateSolidBrush(m_back_color);
	pOldBrush = dc.SelectObject(&BGBrush);
	dc.FillRect(&rect, &BGBrush);
	dc.SelectObject(pOldBrush);
	BGBrush.DeleteObject();
	//��ȡ�������ĳ���
	int length;
	length = rect.Width() * m_pos / m_range;
	if (length > max_length) length = max_length;
	rect.right = rect.left + length;
	//����������ɫ
	if (!rect.IsRectEmpty())
	{
		CBrush GBrush;
		GBrush.CreateSolidBrush(m_progress_color);
		pOldBrush = dc.SelectObject(&GBrush);
		dc.FillRect(&rect, &GBrush);
		dc.SelectObject(pOldBrush);
		GBrush.DeleteObject();
	}
}


//void CProgressStatic::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CRect rect;
//	this->GetClientRect(&rect);
//	m_pos = point.x*m_range / rect.Width();		//������󽫽��������õ��������λ��
//	Invalidate();
//	CStatic::OnLButtonUp(nFlags, point);
//}


void CProgressStatic::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//Ϊ�ؼ�����SS_NOTIFY����
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	m_toolTip.AddTool(this, CCommon::LoadText(IDS_SEEK_TO));

	CStatic::PreSubclassWindow();
}


void CProgressStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	this->GetClientRect(&rect);
	m_pos = point.x*m_range / rect.Width();		//������󽫽��������õ��������λ��
	Invalidate();

	CStatic::OnLButtonDown(nFlags, point);
}


BOOL CProgressStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));	//�����ָ���������ָ������Ϊ����ͼ��
	return TRUE;
	//return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CProgressStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ָ�������ʱ��ʾ��λ�����ּ���
	CRect rect;
	this->GetClientRect(&rect);
	__int64 song_pos;
	song_pos = static_cast<__int64>(point.x) * m_song_length / rect.Width();
	Time song_pos_time;
	song_pos_time.int2time(static_cast<int>(song_pos));
	CString str;
	static int last_sec{};
	if (last_sec != song_pos_time.sec)		//ֻ�����ָ��λ�ö�Ӧ�������仯�˲Ÿ��������ʾ
	{
		str.Format(CCommon::LoadText(IDS_SEEK_TO_MINUTE_SECOND), song_pos_time.min, song_pos_time.sec);
		m_toolTip.UpdateTipText(str, this);
		last_sec = song_pos_time.sec;
	}

	CStatic::OnMouseMove(nFlags, point);
}


BOOL CProgressStatic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_toolTip.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE)
	{
		m_toolTip.RelayEvent(pMsg);
	}


	return CStatic::PreTranslateMessage(pMsg);
}
