#pragma once
#include "InternetCommon.h"

class CCoverDownloadCommon
{
public:
	CCoverDownloadCommon();
	~CCoverDownloadCommon();

	static wstring GetAlbumCoverURL(const wstring& song_id);	//����һ�׸�����ID����ȡר�����������
};

