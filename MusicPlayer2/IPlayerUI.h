#pragma once
#include "stdafx.h"

class IPlayerUI
{
public:
	struct UIButton		//�����л��Ƶİ�ť
	{
		CRect rect;				//��ť�ľ�������
		bool hover{ false };	//����Ƿ�ָ��ť
		bool pressed{ false };	//��ť�Ƿ���
		bool enable{ true };	//��ť�Ƿ�����
	};

public:
	IPlayerUI(){}
	~IPlayerUI(){}

	virtual void Init(CDC* pDC) = 0;
	virtual void DrawInfo(bool reset = false) = 0;
	virtual void ClearInfo() = 0;

	virtual void LButtonDown(CPoint point) = 0;
	virtual void RButtonUp(CPoint point) = 0;
	virtual void MouseMove(CPoint point) = 0;
	virtual void LButtonUp(CPoint point) = 0;
	virtual void OnSizeRedraw(int cx, int cy) = 0;

	virtual CRect GetThumbnailClipArea() = 0;

	virtual void UpdateToolTipPosition() = 0;
	virtual void UpdatePlayPauseButtonTip() = 0;
	virtual void UpdateFullScreenTip() = 0;

	virtual bool SetCursor() = 0;
	virtual void MouseLeave() = 0;
};