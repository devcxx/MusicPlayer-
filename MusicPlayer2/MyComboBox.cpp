#include "stdafx.h"
#include "MyComboBox.h"
#include "MusicPlayer2.h"

IMPLEMENT_DYNAMIC(CMyComboBox, CComboBox)

CMyComboBox::CMyComboBox()
{
}


CMyComboBox::~CMyComboBox()
{
}

void CMyComboBox::SetReadOnly(bool read_only)
{
	//((CEdit*)GetWindow(GW_CHILD))->SetReadOnly(read_only);		//��Endit�ؼ���Ϊֻ��
	m_read_only = read_only;
}

void CMyComboBox::SetEditReadOnly(bool read_only)
{
	((CEdit*)GetWindow(GW_CHILD))->SetReadOnly(read_only);		//��Endit�ؼ���Ϊֻ��
}

BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
END_MESSAGE_MAP()



BOOL CMyComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_read_only)
	{
		if (pMsg->message == WM_MOUSEWHEEL)		//���ֻ��������Ӧ��������Ϣ
			return TRUE;
		if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK || pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONUP)
		{
			CPoint point1 = pMsg->pt;
			CPoint point;
			GetCursorPos(&point);
			//��ȡ��ͷ�����λ��
			GetWindowRect(m_arrow_rect);
			m_arrow_rect.left = m_arrow_rect.right - theApp.DPI(18);
			if(m_arrow_rect.PtInRect(point))		//������ָ���λ��λ�ڼ�ͷ��������Ӧ���ϵ������Ϣ
				return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}
