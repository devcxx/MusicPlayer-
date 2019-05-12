#pragma once
#include "IPlayerUI.h"
#include "MusicPlayer2.h"
#include "CPlayerUIHelper.h"
#include "CPlayerUIBase.h"


class CMiniModeUI :	public CPlayerUIBase
{
public:

	struct SMiniModeUIData
	{
		int widnow_width = theApp.DPI(304);
		int window_height = theApp.DPI(44);
		int window_height2 = theApp.DPI(336);
		int margin = theApp.DPI(3);
		bool m_show_volume{ false };	//����ָʾ�Ƿ�����ʾʱ��Ŀؼ���ʾ������������������ʱ��1.5���ڣ��˱�����ֵΪtrue
	};

public:
	CMiniModeUI(SMiniModeUIData& ui_data, CWnd* pMainWnd);
	~CMiniModeUI();

	virtual void Init(CDC* pDC) override;

	bool PointInControlArea(CPoint point) const;		//�ж�һ�����λ���Ƿ��ڿؼ�����

	virtual void RButtonUp(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;

	virtual CRect GetThumbnailClipArea() override;

	void UpdateSongInfoTip(LPCTSTR str_tip);
	void UpdatePlayPauseButtonTip();

private:
	virtual void _DrawInfo(bool reset = false) override;
	virtual void PreDrawInfo() override;
	void AddMouseToolTip(BtnKey btn, LPCTSTR str) override;		//Ϊһ����ť��������ʾ
	void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) override;
	virtual void UpdateToolTipPosition() override;

	virtual void AddToolTips() override;

private:
	SMiniModeUIData& m_ui_data;
	CFont m_font_time;
};

