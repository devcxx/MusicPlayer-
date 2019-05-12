//һ����CStatic��������ʵ�ֽ��������ܵ���
#pragma once
#include "Time.h"

// CProgressStatic

class CProgressStatic : public CStatic
{
	DECLARE_DYNAMIC(CProgressStatic)

public:
	CProgressStatic();
	virtual ~CProgressStatic();

	void SetRange(int range);
	void SetPos(int pos);
	int GetPos() const { return m_pos; }
	void SetColor(COLORREF color);
	void SetBackColor(COLORREF back_color);
	void SetProgressBarHeight(int height);
	void SetSongLength(int song_length) { m_song_length = song_length; }
	CToolTipCtrl* GetToolTip() { return &m_toolTip; }

protected:
	int m_range{ 1 };			//���������ܳ���
	int m_pos{};				//��������ǰ��λ��
	COLORREF m_progress_color{ RGB(124,248,128) };		//����������ɫ
	COLORREF m_back_color{ RGB(255,255,255) };			//����������ɫ
	int m_progress_height{};			//�������ĸ߶�
	int m_last_progress_length{ -1 };	//������һ�ν������ĳ���
	int m_song_length;			//�������ȵĺ����������������ָ�����ʱ���㶨λ�����ּ��룩

	CToolTipCtrl m_toolTip;		//�ı���ʾ��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


