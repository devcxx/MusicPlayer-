#include "stdafx.h"
#include "CMainDialogBase.h"
#include "Common.h"
#include "MusicPlayer2.h"

IMPLEMENT_DYNAMIC(CMainDialogBase, CDialog)

CMainDialogBase::CMainDialogBase(UINT nIDTemplate, CWnd * pParent)
	: CDialog(nIDTemplate, pParent)
{
}


CMainDialogBase::~CMainDialogBase()
{
}

void CMainDialogBase::SetFullScreen(bool full_screen)
{
	m_bFullScreen = full_screen;
	if (full_screen)
	{
		//��ȡϵͳ��Ļ���  
		int g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		//��ǰ�˵����ĸ߶ȼ�ȥһ�в˵����ĸ߶�
		int height_comp = 0;
		if(theApp.m_ui_data.show_menu_bar)
			height_comp = CCommon::GetMenuBarHeight(m_hWnd) - theApp.DPIRound(19);


		//��m_struOldWndpl�õ���ǰ���ڵ���ʾ״̬�ʹ���λ�ã��Թ��˳�ȫ����ʹ��  
		GetWindowPlacement(&m_struOldWndpl);

		//���������ȫ����ʾ�ͻ�����Ӧ�����õĴ��ڴ�С����ҪΪ�˽�����Ҫ��ʾ�Ĵ���߿�Ȳ����ų�����Ļ��  
		CRect rectWholeDlg;
		CRect rectClient;
		GetWindowRect(&rectWholeDlg);//�õ���ǰ������ܵ��������Ļ������  
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);//�õ����ڿͻ�������  
		ClientToScreen(&rectClient);//���ͻ�����Դ��������תΪ�����Ļ����  
		m_rectFullScreen.left = rectWholeDlg.left - rectClient.left;
		m_rectFullScreen.top = rectWholeDlg.top - rectClient.top + height_comp;
		m_rectFullScreen.right = rectWholeDlg.right + g_iCurScreenWidth - rectClient.right;
		m_rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom;

		//���ô��ڶ��������Ϊȫ������׼��������ȫ��״̬  
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = m_rectFullScreen;
		SetWindowPlacement(&struWndpl);//�ú�������ָ�����ڵ���ʾ״̬����ʾ��Сλ�õȣ������Ǹó�����Ϊ��Ҫ�ĺ���  
	}
	else
	{
		SetWindowPlacement(&m_struOldWndpl);
	}
}
BEGIN_MESSAGE_MAP(CMainDialogBase, CDialog)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


void CMainDialogBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bFullScreen)
	{
		lpMMI->ptMaxSize.x = m_rectFullScreen.Width();
		lpMMI->ptMaxSize.y = m_rectFullScreen.Height();
		lpMMI->ptMaxPosition.x = m_rectFullScreen.left;
		lpMMI->ptMaxPosition.y = m_rectFullScreen.top;
		lpMMI->ptMaxTrackSize.x = m_rectFullScreen.Width();
		lpMMI->ptMaxTrackSize.y = m_rectFullScreen.Height();
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}
