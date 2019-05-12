#pragma once
#include "MusicPlayer2.h"
#include "IPlayerUI.h"
#include "CPlayerUIHelper.h"

#define WM_MAIN_MENU_POPEDUP (WM_USER+117)		//��ʾ����ʽ���˵�����Ϣ��wParaΪ��ʾ�˵���ʾλ�õ�CPoint��ָ��

struct SLayoutData
{
	const int margin = theApp.DPI(4);							//��Ե������
	const int width_threshold = theApp.DPI(600);				//�������ͨ����ģʽ�л���խ����ģʽʱ�����ȵ���ֵ
	const int info_height = theApp.DPI(216);					//խ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	const int path_edit_height = theApp.DPI(32);				//ǰ·��Edit�ؼ�����ĸ߶�
	const int search_edit_height = theApp.DPI(26);				//����������Edit�ؼ�����ĸ߶�
	const int select_folder_width = theApp.DPI(90);	//��ѡ���ļ��С���ť�Ŀ��
	const CSize spectral_size{ theApp.DPI(120), theApp.DPI(90) };	//Ƶ�׷�������Ĵ�С
};


class CPlayerUIBase : public IPlayerUI
{
public:
	CPlayerUIBase(UIData& ui_data, CWnd* pMainWnd);
	~CPlayerUIBase();

	void SetToolTip(CToolTipCtrl* pToolTip);

public:
	virtual void Init(CDC* pDC) override;
	virtual void DrawInfo(bool reset = false) override final;
	virtual void ClearInfo() override;

	virtual void LButtonDown(CPoint point) override;
	virtual void RButtonUp(CPoint point) override;
	virtual void MouseMove(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;
	virtual void OnSizeRedraw(int cx, int cy) override;

	virtual CRect GetThumbnailClipArea() override;
	void UpdateRepeatModeToolTip();
	void UpdateSongInfoToolTip();
	void UpdatePlayPauseButtonTip() override;
	virtual void UpdateFullScreenTip() override;

	virtual bool SetCursor() override;
	virtual void MouseLeave() override;

	void ClearBtnRect();

	static bool IsMidiLyric();		//�Ƿ����MIDI���ֵĸ��

	int Margin() const;
	int EdgeMargin(bool x = true) const;
	int WidthThreshold() const;
	int DrawAreaHeight() const;		//խ����ģʽ����ʾ�����б�ʱ��ͼ���ĸ߶�

protected:
	enum BtnKey		//��ʶ��ť������
	{
		BTN_REPETEMODE,			//��ѭ��ģʽ����ť
		BTN_VOLUME,				//������ť
		BTN_VOLUME_UP,
		BTN_VOLUME_DOWN,
		BTN_TRANSLATE,			//��ʷ��밴ť
		BTN_SKIN,				//�л����水ť
		BTN_EQ,					//��Ч�趨��ť
		BTN_SETTING,			//���ð�ť
		BTN_MINI,				//����ģʽ��ť
		BTN_INFO,				//��Ŀ��Ϣ��ť
		BTN_FIND,				//���Ҹ�����ť
		BTN_STOP,				//ֹͣ
		BTN_PREVIOUS,			//��һ��
		BTN_PLAY_PAUSE,			//����/��ͣ
		BTN_NEXT,				//��һ��
		BTN_SHOW_PLAYLIST,		//��ʾ/���ز����б�
		BTN_SELECT_FOLDER,		//ѡ���ļ���
		BTN_PROGRESS,			//������
		BTN_COVER,				//ר������
		BTN_FULL_SCREEN,		//ȫ����ʾ��ť
		BTN_MENU,				//���˵���ť
		BTN_CLOSE,				//�رհ�ť������ģʽ��
		BTN_RETURN				//���ذ�ť������ģʽ��

	};

	struct DrawData
	{
		//CRect cover_rect;
		CRect lyric_rect;
		CRect thumbnail_rect;
	};

protected:
	virtual void _DrawInfo(bool reset = false) = 0;
	virtual void PreDrawInfo();
	void SetDrawRect();
	void DrawBackground();
	void DrawLryicCommon(CRect rect);
	void DrawSongInfo(CRect rect, bool reset = false);
	void DrawToolBar(CRect rect, bool draw_translate_button);
	void DrawVolumnAdjBtn();
	void DrawControlBar(CRect rect);
	void DrawProgressBar(CRect rect);
	void DrawTranslateButton(CRect rect);
	int DrawTopRightIcons();			//�������Ͻǵ�ͼ�ꡣ�����ܿ��
	void DrawCurrentTime();				//�����Ͻǻ��Ƶ�ǰϵͳʱ��

	void DrawUIButton(CRect rect, UIButton& btn, const IconRes& icon);
	void DrawControlButton(CRect rect, UIButton& btn, const IconRes& icon);
	void DrawTextButton(CRect rect, UIButton& btn, LPCTSTR text, bool back_color = false);

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) = 0;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) = 0;
	virtual void UpdateToolTipPosition() = 0;

	virtual void AddToolTips();			//Ϊÿһ����ť��������ʾ�����ڰ�ť�ľ�������ֻ���ڵ�һ�λ�ͼ֮�����ȷ�������Դ˺��������ڵ�һ�λ�ͼ֮����ã�

	static CRect DrawAreaToClient(CRect rect, CRect draw_area);
	static CRect ClientAreaToDraw(CRect rect, CRect draw_area);

	bool IsDrawNarrowMode();			//�Ƿ�ʹ��խ����ģʽ��ͼ
	bool IsDrawBackgroundAlpha() const;	//�Ƿ���Ҫ����͸������

	int DPI(int pixel);
	int DPI(double pixel);

private:
	void DrawLyricTextMultiLine(CRect rect);
	void DrawLyricTextSingleLine(CRect rect);
	void DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress);
	void SetRepeatModeToolTipText();
	void SetSongInfoToolTipText();
	void SetCoverToolTipText();
	void DrawControlBarBtn(CRect rect, UIButton& btn, const IconRes& icon);

protected:
	CWnd* m_pMainWnd = nullptr;
	CDC* m_pDC;
	UIColors m_colors;
	CDrawCommon m_draw;		//���ڻ����ı��Ķ���
	SLayoutData m_layout;
	//CFont m_font_time;
	DrawData m_draw_data;

	CMenu m_popup_menu;			//����Ҽ��˵�
	CMenu m_main_popup_menu;

	CToolTipCtrl* m_tool_tip = nullptr;

	CString m_repeat_mode_tip;
	CString m_info_tip;
	CString m_cover_tip;

	int m_lyric_text_height;
	UIData& m_ui_data;

	//UI ����
	CRect m_draw_rect;						//��ͼ����
	bool m_show_volume_adj{ false };		//��ʾ����������ť

	std::map<BtnKey, UIButton> m_buttons;

	const int m_progress_on_top_threshold = theApp.DPI(350);		//���������Ŀ��С�ڴ�ֵ������������ʾ�ڲ��ſ��ư�ť���Ϸ�

	bool m_first_draw{ true };

};

