// EditEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "EditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
END_MESSAGE_MAP()



// CEditEx ��Ϣ�������




BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//��Ctrl+Aȫѡ
	if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'A'))
	{
		SetSel(0, -1);
		return TRUE;
	}
	return CEdit::PreTranslateMessage(pMsg);
}
