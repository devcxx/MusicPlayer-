//��������ڶ������ڻ�ͼ�ĺ���
#pragma once

enum class Alignment	//���뷽ʽ
{
	LEFT,
	RIGHT,
	CENTER
};

class CDrawCommon
{
public:

	//����ģʽ
	enum class StretchMode
	{
		STRETCH,		//���죬��ı����
		FILL,			//��䣬���ı��������ü�����
		FIT			//��Ӧ������ı���������ü�
	};

	//������DrawScrollText��������ʱʹ�õ�һЩ��Ҫ�ں���������Ϻ�������ڵı���
	struct ScrollInfo
	{
		int shift_cnt{};		//�ƶ��Ĵ���
		bool shift_dir{};		//�ƶ��ķ�������Ϊfalse������Ϊtrue
		int freez{};			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
		bool dir_changed{ false };	//����������˱仯����Ϊtrue
	};

	CDrawCommon();
	~CDrawCommon();

	void Create(CDC* pDC, CWnd* pMainWnd = nullptr);
	//void SetBackColor(COLORREF back_color);		//���û����ı�ʱ���ı�����ɫ
	//COLORREF GetBackColor() const { return m_backColor; }
	void SetFont(CFont* pfont);		//���û����ı�������
	void SetDC(CDC* pDC);		//���û�ͼ��DC
	CDC* GetDC() { return m_pDC; }

	//��ָ���ľ��������ڻ�������ɫ���ı�
	//rect: �ı��ľ�������
	//lpszString: Ҫ���Ƶ��ı�
	//color: �ı�����ɫ
	//align: �ı��Ķ��뷽ʽ��Ĭ��ֵ������룩
	//no_clip_area: ���Ϊtrue�������������ʱ���ƻ�ͼ����
	//multi_line: �Ƿ������ʾ
	//default_right_align: ����ı��Ŀ�ȴ��ھ��������Ƿ��Ҷ���
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, Alignment align = Alignment::LEFT, bool no_clip_area = false, bool multi_line = false, bool default_right_align = false);

	//��ָ���ľ��������ڻ��Ʒָ���ɫ���ı�
	//rect: �ı��ľ�������
	//lpszString: Ҫ���Ƶ��ı�
	//color1: �ָ�λ����ߵ��ı���ɫ
	//color2: �ָ�λ���ұߵ��ı���ɫ
	//split: ��ɫ�ָ�λ�ã�ȡֵΪ0~1000�����ڸ�ʶ�̬��ʾ��
	//center: �ı��Ƿ����
	//no_clip_area: ���Ϊtrue�������������ʱ���ƻ�ͼ����
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, bool center, bool no_clip_area = false);

	//�ڿؼ��ϻ��ƹ������ı��������Ȳ���ʱ����pixelָ���˺�������һ���ƶ�������ֵ�����resetΪtrue�����������ʼλ��
	//rect: �ı��ľ�������
	//lpszString: Ҫ���Ƶ��ı�
	//color: �ı�����ɫ
	//pixel: �˺�������һ�ι���������ֵ
	//center: �ı��Ƿ����
	//scroll_info: ��������һЩ��ǰ�ı�������״̬��Ϣ
	//reset: ���resetΪtrue��������scroll_info������������ʼλ��
	void DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);

	//�������ܺ�DrawScrollTextһ����ֻ���������ֻ������ҹ����������������
	void DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);

	static void SetDrawArea(CDC* pDC, CRect rect);
	void SetDrawArea(CRect rect);

	//����һ��λͼ
	void DrawBitmap(CBitmap& bitmap, CPoint start_point, CSize size, StretchMode stretch_mode);
	void DrawBitmap(UINT bitmap_id, CPoint start_point, CSize size, StretchMode stretch_mode);
	void DrawBitmap(HBITMAP hbitmap, CPoint start_point, CSize size, StretchMode stretch_mode);

	void DrawIcon(HICON hIcon, CPoint start_point, CSize size);

	void FillRect(CRect rect, COLORREF color, bool no_clip_area = false);
	void FillAlphaRect(CRect rect, COLORREF color, BYTE alpha, bool no_clip_area = false);		//���һ����͸���ľ��Σ�����http://blog.csdn.net/lee353086/article/details/38311421��

	void DrawRectTopFrame(CRect rect, COLORREF color, int pilex = 1);

	CSize GetTextExtent(LPCTSTR str);

	//��ͼƬ���쵽ָ���ߴ�(https://blog.csdn.net/sichuanpb/article/details/22986877)
	static bool BitmapStretch(CImage *pImage, CImage *ResultImage, CSize size);

	static HICON LoadIconResource(UINT id, int width, int height);

private:
	CDC* m_pDC{};		//���ڻ�ͼ��CDC���ָ��
	CWnd* m_pMainWnd{};	//��ͼ���ڵľ��
	//COLORREF m_backColor{ RGB(255,255,255) };
	CFont* m_pfont{};
};

