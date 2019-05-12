#include "stdafx.h"
#include "CListBoxEnhanced.h"
#include "MusicPlayer2.h"


CListBoxEnhanced::CListBoxEnhanced()
	: CListCtrlEx()
{
}


CListBoxEnhanced::~CListBoxEnhanced()
{
}

void CListBoxEnhanced::AddString(LPCTSTR str)
{
	int cnt = GetItemCount();
	InsertItem(cnt, str);
}

void CListBoxEnhanced::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	ModifyStyle(0, LVS_NOCOLUMNHEADER);

	CRect rect;
	GetWindowRect(rect);
	InsertColumn(0, _T("text"), LVCFMT_LEFT, rect.Width() - theApp.DPI(20) - 1);


	CListCtrlEx::PreSubclassWindow();
}
BEGIN_MESSAGE_MAP(CListBoxEnhanced, CListCtrlEx)
	ON_NOTIFY_REFLECT(NM_CLICK, &CListBoxEnhanced::OnNMClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CListBoxEnhanced::OnNMRClick)
END_MESSAGE_MAP()


void CListBoxEnhanced::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pParent{ GetParent() };
	if (pParent != nullptr)
		pParent->SendMessage(WM_LISTBOX_SEL_CHANGED, (WPARAM)this, (LPARAM)pNMItemActivate->iItem);

	*pResult = 0;
}


void CListBoxEnhanced::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pParent{ GetParent() };
	if (pParent != nullptr)
		pParent->SendMessage(WM_LISTBOX_SEL_CHANGED, (WPARAM)this, (LPARAM)pNMItemActivate->iItem);

	*pResult = 0;
}
