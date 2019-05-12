#pragma once
#include "ListCtrlEx.h"
#include "AudioCommon.h"
#include "ColorConvert.h"
#include "ListCtrlEx.h"
#include "Common.h"
#include <boost/shared_ptr.hpp>

struct DownloadItem {
	void* ctrl;
	SongInfo* song;
	int id;
};

typedef boost::shared_ptr<DownloadItem> DownItemPtr;

class CDownloadListCtrl :
	public CListCtrlEx
{
	DECLARE_DYNAMIC(CDownloadListCtrl)

public:
	CDownloadListCtrl();
	~CDownloadListCtrl();

	void AdjustColumnWidth();

	void EnableTip(bool enable = true) { m_bEnableTips = enable; }		//�����Ƿ�����ʾ

	void Download(const SongInfo& si);

	void UpdateDownList();

protected:

	CToolTipCtrl m_toolTip;		//�ı���ʾ��
	int m_nItem;				//����к�
								//int m_nSubItem;			//����к�
	bool m_bEnableTips{ false };	//�Ƿ����ı���ʾ

	vector<SongInfo> m_all_song_info;		//���沥���б������и�������Ϣ
	vector<int> m_search_result;					//���������������ĸ������
	bool m_searched{ false };
	vector <DownItemPtr> m_downItems;


	void CalculateColumeWidth(vector<int>& width);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

};

