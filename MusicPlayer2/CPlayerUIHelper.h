#pragma once
#include "ColorConvert.h"

struct UIColors		//������ɫ
{
	COLORREF color_text;				//�ı���ɫ
	COLORREF color_text_lable;			//��ǩ�ı�����ɫ
	COLORREF color_text_2;				//���δ�����ı�����ɫ
	COLORREF color_text_heighlight;		//���ָ��ʱ�ı�����ɫ
	COLORREF color_back;				//������ɫ
	COLORREF color_lyric_back;			//��ʽ��汳����ɫ
	COLORREF color_control_bar_back;	//������������ɫ
	COLORREF color_spectrum;			//Ƶ�׷������ε���ɫ
	COLORREF color_spectrum_cover;		//��ר������ʱ��Ƶ�׷������ε���ɫ
	COLORREF color_spectrum_back;		//Ƶ�׷����ı�����ɫ
	COLORREF color_button_back;			//��ʷ��밴ť�ı���ɫ
	COLORREF color_button_pressed;		//��ť���µ���ɫ
};

class CPlayerUIHelper
{
public:
	CPlayerUIHelper();
	~CPlayerUIHelper();

	static UIColors GetUIColors(const ColorTable& colorTable);
};

