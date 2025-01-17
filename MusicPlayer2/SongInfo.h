#pragma once
#include "stdafx.h"
#include "Time.h"
#include "Common.h"
#include "Resource.h"

struct KgSongInfo
{
	int pay_type_320;
	int m4afilesize;
	int price_sq;
	int filesize;
	string source;
	int bitrate;
	string topic;
	int price;
	string accompany;
	int old_cpy;
	int fail_process_sq;
	string singername;
	int pay_type;
	int sourceid;
	string topic_url;
	int fail_process_320;
	int pkg_price;
	int feetype;
	string filename;
	int price_320;
	string extname;
	string hash320;
	string mvhash;
	int privilege;
	int album_audio_id;
	string album_id;
	int ownercount;
	int rp_publish;
	string rp_type;
	int audio_id;
	int filesize320;
	int isnew;
	int duration;
	int pkg_price_sq;
	int pkg_price_320;
	int srctype;
	string songname;
	int fail_process;
	string sqhash;
	string album_name;
	string hash;
	int pay_type_sq;
	int privilege320;
	int sqprivilege;
	int sqfilesize;
	string othername;

	int timelength;
	string audio_name;
	int have_album;
// 	string album_name;
// 	string album_id;
	string img;
	int have_mv;
	string video_id;
	string author_name;
	string song_name;
	string lyrics;
	string author_id;
	int privilege2;
	string play_url;
// 	int bitrate;
};

//一首歌曲的信息
struct SongInfo
{
	SongInfo(bool ini = true)
	{
		if (ini)
		{
			title = CCommon::LoadText(IDS_DEFAULT_TITLE);
			artist = CCommon::LoadText(IDS_DEFAULT_ARTIST);
			album = CCommon::LoadText(IDS_DEFAULT_ALBUM);
			year = CCommon::LoadText(IDS_DEFAULT_YEAR);
			genre = CCommon::LoadText(IDS_DEFAULT_GENRE);
		}
	}



	wstring file_name{};	//歌曲的文件名
	wstring play_url{};		// URL资源
	wstring lyric_file{};	//匹配的歌词文件的路径
	Time lengh;			//歌曲的长度
	int duration;		// 歌曲时长（毫秒）
	int filesize{};		// 文件大小
	int bitrate{};		//比特率
	wstring title;		//标题
	wstring artist;		//艺术家
	wstring album;		//唱片集
	wstring year;		//年份
	wstring comment;	//注释
	wstring genre;		//流派
	wstring kg_hash;		// 酷狗hash
	BYTE genre_idx{ 255 };		//以字节表示的流派号
	BYTE track{};		//音轨序号
	int tag_type{};		//标签的类型（0：其他；1：ID3v1；2：ID3v2）
	bool info_acquired{ false };		//如果已经获取到了信息，则为ture
	Time start_pos{};		//音频的起始位置，用于cue分轨
	Time end_pos{};
	bool is_cue{ false };		//如果曲目是cue分轨，则为true
	wstring album_url{};		//专辑封面保存的文件名
	wstring song_id{};			//歌曲对应的网易云音乐中的歌曲ID
	int listen_time{};			//歌曲累计听的时间（单位为秒）

	//根据文件名的比较函数，用于以文件名排序
	static bool ByFileName(const SongInfo& a, const SongInfo& b) { return a.file_name < b.file_name; }
	//根据标题的比较函数，用于以标题排序
	static bool ByTitle(const SongInfo& a, const SongInfo& b) { return a.title < b.title; }
	//根据艺术家的比较函数，用于以艺术家排序
	static bool ByArtist(const SongInfo& a, const SongInfo& b) { return a.artist < b.artist; }
	//根据唱片集的比较函数，用于以唱片集排序
	static bool ByAlbum(const SongInfo& a, const SongInfo& b) { return a.album < b.album; }
	//根据音轨序号的比较函数，用于以音轨序号排序
	static bool ByTrack(const SongInfo& a, const SongInfo& b) { return a.track < b.track; }

	//从另一个SongInfo对象复制标签信息
	void CopyAudioTag(const SongInfo& song_info)
	{
		title = song_info.title;
		artist = song_info.artist;
		album = song_info.album;
		year = song_info.year;
		comment = song_info.comment;
		genre = song_info.genre;
		genre_idx = song_info.genre_idx;
		track = song_info.track;
		tag_type = song_info.tag_type;
	}

	bool IsTitleEmpty() const
	{
		return title.empty() || title == wstring(CCommon::LoadText(IDS_DEFAULT_TITLE));
	}

	bool IsArtistEmpty() const
	{
		return artist.empty() || artist == wstring(CCommon::LoadText(IDS_DEFAULT_ARTIST));
	}

	bool IsAlbumEmpty() const
	{
		return album.empty() || album == wstring(CCommon::LoadText(IDS_DEFAULT_ALBUM));
	}

	bool IsYearEmpty() const
	{
		return year.empty() || year == wstring(CCommon::LoadText(IDS_DEFAULT_YEAR));
	}

	bool IsGenreEmpty() const
	{
		return genre.empty() || genre == wstring(CCommon::LoadText(IDS_DEFAULT_GENRE));
	}

	wstring GetTitle() const
	{
		return title.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_TITLE)) : title;
	}

	wstring GetArtist() const
	{
		return artist.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_ARTIST)) : artist;
	}

	wstring GetAlbum() const
	{
		return album.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_ALBUM)) : album;
	}

	wstring GetYear() const
	{
		return year.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_YEAR)) : year;
	}

	wstring GetGenre() const
	{
		return genre.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_GENRE)) : genre;
	}
};

