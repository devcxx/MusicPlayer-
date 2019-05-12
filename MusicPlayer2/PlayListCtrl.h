//�����б�ר�õĿؼ��࣬��CListCtrlEx����������ʵ������CListCtrl�ؼ�������ɫ
//�Լ������ʾ��Ϣ
#pragma once
#include "AudioCommon.h"
#include "ColorConvert.h"
#include "ListCtrlEx.h"
#include "Common.h"

// CPlayListCtrl

class CPlayListCtrl : public CListCtrlEx
{
	DECLARE_DYNAMIC(CPlayListCtrl)

public:
	CPlayListCtrl(const vector<SongInfo>& all_song_info);
	virtual ~CPlayListCtrl();

	void EnableTip(bool enable = true) { m_bEnableTips = enable; }		//�����Ƿ�����ʾ

	static wstring GetDisplayStr(const SongInfo& song_info, DisplayFormat display_format);		//����display_formatָ������ʾ��ʽ������һ����Ŀ��ʾ���ַ���
	void ShowPlaylist(DisplayFormat display_format, bool search_result = false);		//��ʾ�����б�
	void QuickSearch(const wstring& key_words);		//���ݹؼ���ִ�п��ٲ��ң������ļ������������⡢�����Һͳ�Ƭ�������ҵ�����Ŀ����ű�����m_search_result��
	void GetItemSelectedSearched(vector<int>& item_selected);		//��ȡ��������״̬�²����б�ѡ�е���Ŀ

	void AdjustColumnWidth();

protected:

	CToolTipCtrl m_toolTip;		//�ı���ʾ��
	int m_nItem;				//����к�
	//int m_nSubItem;			//����к�
	bool m_bEnableTips{ false };	//�Ƿ����ı���ʾ

	const vector<SongInfo>& m_all_song_info;		//���沥���б������и�������Ϣ
	vector<int> m_search_result;					//���������������ĸ������
	bool m_searched{ false };

protected:
	void CalculateColumeWidth(vector<int>& width);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


