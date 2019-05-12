#include "stdafx.h"
#include "ListCtrlEx.h"
#include "MusicPlayer2.h"

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
	: m_theme_color(theApp.m_app_setting_data.theme_color)
{
	
	//��ʼ����ɫ
	//m_theme_color.original_color = GRAY(180);
	//CColorConvert::ConvertColor(m_theme_color);
}


CListCtrlEx::~CListCtrlEx()
{
}


//void CListCtrlEx::SetColor(const ColorTable & colors)
//{
//	m_theme_color = colors;
//	if (m_hWnd != NULL)
//		Invalidate();
//}

void CListCtrlEx::GetItemSelected(vector<int>& item_selected) const
{
	item_selected.clear();
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			item_selected.push_back(nItem);
		}
	}
}

int CListCtrlEx::GetCurSel() const
{
	vector<int> item_selected;
	GetItemSelected(item_selected);
	if (!item_selected.empty())
		return item_selected[0];
	else
		return -1;
}

void CListCtrlEx::SetCurSel(int select)
{
	int size = GetItemCount();
	if (select >= 0 && select < size)
	{
		SetItemState(select, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);	//ѡ����
		EnsureVisible(select, FALSE);		//ʹѡ���б��ֿɼ�
	}
	else
	{
		//ȡ�������е�ѡ��
		for(int i{}; i<size; i++)
			SetItemState(i, 0, LVIS_SELECTED);
	}
}

void CListCtrlEx::SelectAll()
{
	int itemCnt = GetItemCount();
	for (int i = 0; i < itemCnt; i++)
	{
		SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}
}

bool CListCtrlEx::SetRowHeight(int height)
{
	if (height > 0 && height <= 512)
	{
		CImageList imgList;		//ΪClistCtrl����һ��ͼ���б��������и�
		BOOL rtn = imgList.Create(1, height, ILC_COLOR, 1, 1);
		if (rtn != FALSE)
		{
			SetImageList(&imgList, LVSIL_SMALL);
			return true;
		}
	}
	return false;
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW lplvdr = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW& nmcd = lplvdr->nmcd;
	static bool this_item_select = false;
	switch (lplvdr->nmcd.dwDrawStage)	//�ж�״̬   
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:			//���Ϊ��ITEM֮ǰ��Ҫ������ɫ�ĸı�
		this_item_select = false;
		if (IsWindowEnabled())
		{
			//��ѡ�������Ǹ�����ʱ������ɫ
			if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED && nmcd.dwItemSpec == m_highlight_item)
			{
				this_item_select = true;
				SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = m_theme_color.light3;
				lplvdr->clrTextBk = m_theme_color.dark1;
			}
			//����ѡ���е���ɫ
			else if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)
			{
				this_item_select = true;
				SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = m_theme_color.dark3;
				lplvdr->clrTextBk = m_theme_color.light2;
			}
			//���ø����е���ɫ
			else if (nmcd.dwItemSpec == m_highlight_item)
			{
				lplvdr->clrText = m_theme_color.dark2;
				//lplvdr->clrText = 0;
				lplvdr->clrTextBk = m_theme_color.light3;
			}
			//����ż���е���ɫ
			else if (nmcd.dwItemSpec % 2 == 0)
			{
				lplvdr->clrText = CColorConvert::m_gray_color.dark3;
				lplvdr->clrTextBk = CColorConvert::m_gray_color.light3;
			}
			//���������е���ɫ
			else
			{
				lplvdr->clrText = CColorConvert::m_gray_color.dark3;
				lplvdr->clrTextBk = CColorConvert::m_gray_color.light4;
			}

			//�ñ���ɫ��䵥Ԫ����ȥ��ÿ��ǰ��Ŀհ�
			CRect rect = nmcd.rc;
			CDC* pDC = CDC::FromHandle(nmcd.hdc);		//��ȡ��ͼDC
			pDC->FillSolidRect(rect, lplvdr->clrTextBk);
		}
		else		//���ؼ�������ʱ����ʾ�ı���Ϊ��ɫ
		{
			lplvdr->clrText = GRAY(140);
			lplvdr->clrTextBk = GRAY(240);
		}
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPOSTPAINT:
		if (this_item_select)
			SetItemState(nmcd.dwItemSpec, 0xFF, LVIS_SELECTED);
		//*pResult = CDRF_DODEFAULT;
		break;
	}
}


void CListCtrlEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	CListCtrl::PreSubclassWindow();

	SetBkColor(m_background_color);
	//SetHightItem(-1);
	SetRowHeight(theApp.DPI(22));
}


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
	CListCtrl::OnLButtonDown(nFlags, point);
}


void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
	CListCtrl::OnRButtonDown(nFlags, point);
}


BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR)		//�����б�ؼ��ļ�����Ϣ����ֹÿ�ΰ���һ����ʱ�б�ѡ���л��������ġ��ѿ������߿�
	//	return TRUE;

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlEx::OnSetFocus(CWnd* pOldWnd)
{
	CListCtrl::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������

	SendMessage(WM_KILLFOCUS);				//��ֹ�б�ؼ���ȡ���㣬��ֹѡ���л�����ѿ������߿�
}
