#pragma once
#include "DrawCommon.h"
#include "CPlayerUIBase.h"


class CPlayerUI2 : public CPlayerUIBase
{
public:
	CPlayerUI2(UIData& ui_data, CWnd* pMainWnd);
	~CPlayerUI2();

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) override;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) override;
	virtual void UpdateToolTipPosition() override;

private:
	virtual void _DrawInfo(bool reset = false) override;		//������Ϣ
};

