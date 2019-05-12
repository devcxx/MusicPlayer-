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

//һ�׸�������Ϣ
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



	wstring file_name{};	//�������ļ���
	wstring lyric_file{};	//ƥ��ĸ���ļ���·��
	Time lengh;			//�����ĳ���
	int duration;		// ����ʱ�������룩
	int filesize{};		// �ļ���С
	int bitrate{};		//������
	wstring title;		//����
	wstring artist;		//������
	wstring album;		//��Ƭ��
	wstring year;		//���
	wstring comment;	//ע��
	wstring genre;		//����
	wstring kg_hash;		// �ṷhash
	BYTE genre_idx{ 255 };		//���ֽڱ�ʾ�����ɺ�
	BYTE track{};		//�������
	int tag_type{};		//��ǩ�����ͣ�0��������1��ID3v1��2��ID3v2��
	bool info_acquired{ false };		//����Ѿ���ȡ������Ϣ����Ϊture
	Time start_pos{};		//��Ƶ����ʼλ�ã�����cue�ֹ�
	Time end_pos{};
	bool is_cue{ false };		//�����Ŀ��cue�ֹ죬��Ϊtrue
	wstring album_url{};		//ר�����汣����ļ���
	wstring song_id{};			//������Ӧ�������������еĸ���ID
	int listen_time{};			//�����ۼ�����ʱ�䣨��λΪ�룩

	//�����ļ����ıȽϺ������������ļ�������
	static bool ByFileName(const SongInfo& a, const SongInfo& b) { return a.file_name < b.file_name; }
	//���ݱ���ıȽϺ����������Ա�������
	static bool ByTitle(const SongInfo& a, const SongInfo& b) { return a.title < b.title; }
	//���������ҵıȽϺ���������������������
	static bool ByArtist(const SongInfo& a, const SongInfo& b) { return a.artist < b.artist; }
	//���ݳ�Ƭ���ıȽϺ����������Գ�Ƭ������
	static bool ByAlbum(const SongInfo& a, const SongInfo& b) { return a.album < b.album; }
	//����������ŵıȽϺ����������������������
	static bool ByTrack(const SongInfo& a, const SongInfo& b) { return a.track < b.track; }

	//����һ��SongInfo�����Ʊ�ǩ��Ϣ
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

