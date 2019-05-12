#pragma once
#include "DrawCommon.h"
#include "ColorConvert.h"
#include "MusicPlayer2.h"

class CCortanaLyric
{
public:
	CCortanaLyric();
	~CCortanaLyric();

	void Init();	//��ʼ������ȡCortana��������������
	static void InitFont();
	void SetEnable(bool enable);

	void DrawInfo();

	void ResetCortanaText();		//��Cortana��������ı��ָ�ΪĬ��
	void AlbumCoverEnable(bool enable);
	void SetBeatAmp(int beat_amp);
	void SetUIColors();

private:
	struct UIColors		//������ɫ
	{
		COLORREF text_color;		//�Ѳ��Ÿ�ʵ���ɫ
		COLORREF text_color2;		//δ���Ÿ�ʵ���ɫ
		COLORREF info_text_color;	//������Ϣ�ı�����ɫ
		COLORREF back_color;		//����ɫ
		COLORREF sprctrum_color;
		bool dark;			//�Ƿ�ʹ����ɫ��Ϊ����ɫ����
	};

private:
	void CheckDarkMode();			//���Cortana�������Ƿ�Ϊ��ɫģʽ

	//��Cortana�������ϻ����ı�
	//str:	Ҫ���Ƶ��ַ���
	//align:	���뷽ʽ
	void DrawCortanaTextSimple(LPCTSTR str, Alignment align);

	/* ��Cortana�������ϻ��ƹ�����ʾ���ı�
	������
		str:	Ҫ���Ƶ��ַ���
		reset:	���Ϊtrue�����ù���λ��
		scroll_pixel:	�ı�����һ���ƶ�������ֵ�����ֵԽ�������Խ�죩
	*/
	void DrawCortanaText(LPCTSTR str, bool reset, int scroll_pixel);
	
	/* ��Cortana�������ϻ��ƶ�̬��ʾ��ʵ��ı�
	������
		str:	Ҫ���Ƶ��ַ���
		progress:	��ǰ��ʵĽ��ȣ���ΧΪ0~1000��
	*/
	void DrawCortanaText(LPCTSTR str, int progress);

	//��Cortana��������˫����ʾ���
	void DrawLyricDoubleLine(LPCTSTR lyric, LPCTSTR next_lyric, int progress);

	//��Cortana����������ʾ������ĸ��
	void DrawLyricWithTranslate(LPCTSTR lyric, LPCTSTR translate, int progress);

	void DrawAlbumCover(const CImage& album_cover);

	void DrawSpectrum();

	CRect TextRect() const;
	CRect CoverRect() const;

private:
	bool m_enable;
	HWND m_cortana_hwnd{};		//Cortana�ľ��
	HWND m_hCortanaStatic;
	wstring m_cortana_default_text;	//Cortana��������ԭ�����ı�
	CDrawCommon m_draw;		//������Cortana�������л�ͼ�Ķ���
	CWnd* m_cortana_wnd{};		//Cortana�������ָ��
	CFont m_default_font;		//��Cortana��������ԭ��������
	CRect m_cortana_rect;		//Cortana�������ľ�������
	int m_cover_width;
	CDC* m_pDC{};				//��Cortana�������л�ͼ��DC

	bool m_dark_mode{ true };			//Cortana�������Ƿ�����ɫģʽ

	CPoint m_check_dark_point{};			//�����ж�Cortana�������Ƿ�Ϊ��ɫģʽ�ĵ��λ��
	//COLORREF m_back_color;
	const COLORREF m_border_color{ GRAY(180) };		//ǳɫģʽʱ�߿����ɫ

	UIColors m_colors;

	bool m_show_album_cover{ false };			//�Ƿ���Cortanaͼ�괦��ʾר������
	int m_beat_amp{0};						//С��ͼ�������ķ�ֵ��ȡֵΪ0~1000

public:
	//bool m_cortana_disabled;
};

