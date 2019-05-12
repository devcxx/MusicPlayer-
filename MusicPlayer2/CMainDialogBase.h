#pragma once
#include <afxwin.h>
class CMainDialogBase :
	public CDialog
{
	DECLARE_DYNAMIC(CMainDialogBase)
public:
	CMainDialogBase(UINT nIDTemplate, CWnd *pParent = NULL);
	~CMainDialogBase();

protected:
	void SetFullScreen(bool full_screen);		//���ô���ȫ��(����:https://www.cnblogs.com/weixinhum/p/3916673.html)

private:
	bool m_bFullScreen{ false };
	CRect m_rectFullScreen;
	WINDOWPLACEMENT m_struOldWndpl;//�ṹ�а������йش�������Ļ��λ�õ���Ϣ

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

