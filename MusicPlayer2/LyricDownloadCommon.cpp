#include "stdafx.h"
#include "LyricDownloadCommon.h"
#include <json/json.h>
#include <fmt/format.h>
#include "restclient-cpp/restclient.h"
#include "strcpcvt.h"
#include "base64.h"

CLyricDownloadCommon::CLyricDownloadCommon()
{
}


CLyricDownloadCommon::~CLyricDownloadCommon()
{
}


bool CLyricDownloadCommon::DownloadLyric(const wstring & song_id, wstring & result, bool download_translate)
{
	wstring lyric_url;
	if (!download_translate)
		lyric_url = L"http://music.163.com/api/song/media?id=" + song_id;
	else
		lyric_url = L"http://music.163.com/api/song/lyric?os=osx&id=" + song_id + L"&lv=-1&kv=-1&tv=-1";
	return CInternetCommon::GetURL(lyric_url, result);
}

const std::string KG_LYRIC_SEARCH_API = "http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword={0}&duration={1}&hash={2}";
const std::string KG_KYRIC_DOWN_API = "http://lyrics.kugou.com/download?ver=1&client=pc&id={0}&accesskey={1}&fmt=lrc&charset=utf8";
bool CLyricDownloadCommon::DownloadKgLyric(const SongInfo& song, wstring& result)
{
	std::wstring keyword = song.title;
	int duration = song.duration;
	std::wstring hash = song.kg_hash;
	std::string search_api = fmt::format(KG_LYRIC_SEARCH_API, cvt::ws2s(keyword), duration, cvt::ws2s(hash));

	RestClient::Response resp = RestClient::get(cvt::s2utf8(search_api));
	Json::Reader reader;
	Json::Value root, info, down;
	std::string ansi_ret = cvt::utf8ts(resp.body);
	reader.parse(ansi_ret, root);

	if (!root["candidates"].empty()) {
		const Json::Value first = root["candidates"][0];

		std::string id, accesskey;
		id = first["id"].asString();
		accesskey = first["accesskey"].asString();

		std::string down_api = fmt::format(KG_KYRIC_DOWN_API, id, accesskey);
		RestClient::Response resp2 = RestClient::get(cvt::s2utf8(down_api));
		std::string ansi_ret2 = cvt::utf8ts(resp2.body);

		reader.parse(ansi_ret2, down);

		const Json::Value& content = down["content"];
		if (!content.empty()) {
			std::string lyr_base64 = down["content"].asString();
			std::string lyr_ansi = base64::base64_decode(lyr_base64);
			result = cvt::utf8tws(lyr_ansi);
			return true;
		}
	}

	

	return false;
}

bool CLyricDownloadCommon::DisposeLryic(wstring & lyric_str)
{
	size_t index1 = lyric_str.find('[');	//���ҵ�1���������ţ���Ϊ��ʿ�ʼ��λ��
	if (index1 == string::npos)
	{
		return false;
	}
	lyric_str = lyric_str.substr(index1, lyric_str.size() - index1 - 13);
	if (!lyric_str.empty() && lyric_str.back() == L'\"')
		lyric_str.pop_back();

	for (int i{}; i < static_cast<int>(lyric_str.size() - 1); i++)
	{
		//�������к����ַ�����\r\n����\n\n��������ת����Ϊ����ת���ַ�\r\n��ɾ������������ַ�
		if (i < static_cast<int>(lyric_str.size() - 3))
		{
			if ((lyric_str[i] == '\\' && lyric_str[i + 1] == 'r' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n')
				|| (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n'))
			{
				lyric_str[i] = '\r';
				lyric_str[i + 1] = '\n';
				lyric_str.erase(i + 2, 2);
			}
		}
		//�������к����ַ�����\r��������ת����Ϊ����ת���ַ�\r\n
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'r')
		{
			lyric_str[i] = '\r';
			lyric_str[i + 1] = '\n';
		}
		//�������к����ַ�����\n��������ת����Ϊ����ת���ַ�\r\n
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n')	//������ı��еġ�\n��ת���ɻس���\r\n
		{
			lyric_str[i] = '\r';
			lyric_str[i + 1] = '\n';
		}
		//�������к����ַ�����\"������ɾ����б�ܡ�\��
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == '\"')
		{
			lyric_str.erase(i, 1);
		}
	}
	return true;
}

void CLyricDownloadCommon::AddLyricTag(wstring& lyric_str, const wstring & song_id, const wstring & title, const wstring & artist, const wstring & album)
{
	wstring tag_info{};
	CString tmp;
	tmp.Format(_T("[id:%s]\r\n"), song_id.c_str());
	tag_info += tmp;
	if (lyric_str.find(L"[ti:") == wstring::npos)
	{
		tmp.Format(_T("[ti:%s]\r\n"), title.c_str());
		tag_info += tmp;
	}
	if (lyric_str.find(L"[ar:") == wstring::npos)
	{
		tmp.Format(_T("[ar:%s]\r\n"), artist.c_str());
		tag_info += tmp;
	}
	if (lyric_str.find(L"[al:") == wstring::npos)
	{
		tmp.Format(_T("[al:%s]\r\n"), album.c_str());
		tag_info += tmp;
	}
	lyric_str = tag_info + lyric_str;
}

