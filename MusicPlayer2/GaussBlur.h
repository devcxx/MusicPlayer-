//ʵ�ָ�˹ģ�����࣬�㷨���ԣ�http://www.cnblogs.com/hoodlum1980/p/4528486.html
#pragma once

class CGaussBlurThreadParams
{
public:
	int r;
	double* pTempl;
	LPBYTE pSrc;	//Src  λͼ��λͼ������� ���������̣߳�pSrc��pDest ����ͬ�ģ�
	LPBYTE pDest;	//Dest λͼ��λͼ�������

	int width;		//ͼ����
	int height;		//ͼ��߶ȣ��Ѿ���ȡ����ֵ��

	//������з�Χ��[rowBegin, rowEnd) �������� rowEnd ��ÿ���̲߳�ͬ��
	int rowBegin;
	int rowEnd;
	int stride;		//ɨ���п�ȣ�bytes��
	int pixelSize;	//���ش�С =bpp/8;

	bool bHorz;		//true-ˮƽģ����false-����ģ��
};


class CGaussBlur
{
public:
	CGaussBlur();
	~CGaussBlur();

	void SetSigma(double sigma);		//���ø�˹ģ���뾶
	void Reset();
	void DoGaussBlur(const CImage& image_src, CImage& image_dest);		//��image_srcִ�и�˹ģ�������������image_dest��

protected:
	int m_r;		//����ģ�������εı߳�Ϊ (2 * r + 1)
	double m_sigma;		//��˹�뾶�������ƽ������
	double* m_pTempl;	//ģ��T[r+1];

	bool Filter(LPCVOID pSrc, LPVOID pDest, int width, int height, int bpp);

	DWORD WINAPI GaussBlurThreadProc8(LPVOID lpParameters);
	DWORD WINAPI GaussBlurThreadProc24(LPVOID lpParameters);
};

