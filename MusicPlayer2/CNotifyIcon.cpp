#include "stdafx.h"
#include "CNotifyIcon.h"
#include "MusicPlayer2.h"

CNotifyIcon::CNotifyIcon()
{
}


CNotifyIcon::~CNotifyIcon()
{
}

void CNotifyIcon::Init(HICON hIcon)
{
	m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);	//�ýṹ������Ĵ�С
	m_ntIcon.hIcon = hIcon;						//����ͼ��
	m_ntIcon.hWnd = theApp.m_pMainWnd->GetSafeHwnd();				//��������ͼ��֪ͨ��Ϣ�Ĵ��ھ��
	CCommon::WStringCopy(m_ntIcon.szTip, 128, L"MusicPlayer2");
	m_ntIcon.uCallbackMessage = MY_WM_NOTIFYICON;			//Ӧ�ó��������ϢID��
	m_ntIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;		//ͼ������ԣ����ó�ԱuCallbackMessage��hIcon��szTip��Ч

	m_notify_menu.LoadMenu(IDR_NOTIFY_MENU);
}

void CNotifyIcon::AddNotifyIcon()
{
	::Shell_NotifyIcon(NIM_ADD, &m_ntIcon);
}

void CNotifyIcon::DeleteNotifyIcon()
{
	::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);
}

void CNotifyIcon::SetIconToolTip(LPCTSTR strTip)
{
	if (m_tool_tip_str != strTip)
	{
		CCommon::WStringCopy(m_ntIcon.szTip, 128, strTip);
		::Shell_NotifyIcon(NIM_MODIFY, &m_ntIcon);
	}
	m_tool_tip_str = strTip;
}

void CNotifyIcon::OnNotifyIcon(UINT msgId, HWND hMiniMode)
{
	if (msgId == WM_LBUTTONUP)
	{
		if (hMiniMode != NULL)
		{
			::ShowWindow(hMiniMode, SW_RESTORE);
			::SetForegroundWindow(hMiniMode);
		}
		else
		{
			theApp.m_pMainWnd->ShowWindow(SW_RESTORE);
			theApp.m_pMainWnd->SetForegroundWindow();
		}

	}
	if (msgId == WM_RBUTTONUP)
	{
		theApp.m_pMainWnd->SetForegroundWindow();

		//��֪ͨ������Ҽ������Ҽ��˵�
		CPoint point;
		GetCursorPos(&point);	//��ȡ��ǰ����λ��
		m_notify_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, theApp.m_pMainWnd); //��ָ��λ����ʾ�����˵�
	}
	if (msgId == WM_LBUTTONDBLCLK)
	{
	}
}
