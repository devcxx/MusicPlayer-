#pragma once
#include "DrawCommon.h"
#include "CPlayerUIBase.h"


class CPlayerUI : public CPlayerUIBase
{
public:
	CPlayerUI(UIData& ui_data, CWnd* pMainWnd);
	~CPlayerUI();

	//virtual void RButtonUp(CPoint point) override;
	virtual void MouseMove(CPoint point) override;
	virtual void OnSizeRedraw(int cx, int cy) override;

	//virtual CRect GetThumbnailClipArea() override;

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) override;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) override;
	virtual void UpdateToolTipPosition() override;

private:
	virtual void _DrawInfo(bool reset = false) override;		//������Ϣ

	void DrawLyricsArea(CRect lyric_rect);			//��ͨģʽ�»��Ƹ������
	CSize SpectralSize();
};

