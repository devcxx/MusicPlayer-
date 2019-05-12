
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

//�����ҪΪWindows XP���룬��ȥ������һ�д����ע��
//#define COMPILE_IN_WIN_XP

//�Զ�������ļ�
#include <conio.h >
#include <vector>
#include <io.h>
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <deque>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cmath>
#include <unordered_map>
using std::ofstream;
using std::ifstream;
using std::string;
using std::wstring;
using std::vector;
using std::deque;
using std::map;

//����BASS��Ƶ���֧��
#include <bass.h>
#ifdef _M_X64
#pragma comment(lib,"bass_x64.lib")
#else
#pragma comment(lib,"bass.lib")
#endif

#ifndef COMPILE_IN_WIN_XP
//���ڻ�ȡϵͳ������ɫ
#pragma comment(lib,"Dwmapi.lib")
#endif

#include <afxinet.h>	//����֧��ʹ��������ص���
#include <afxwin.h>




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//�Զ���ĺ�

#ifdef UNICODE
#define _tstring  std::wstring
#else
#define _tstring  std::string
#endif // !UNICODE

//CPlayer�����õ��ĳ���
#define NEXT (-999)		//���塰��һ��������
#define PREVIOUS (-998)		//���塰��һ��������

#define MAX_NUM_LENGTH 5000		//�����ȡ��Ƶ�ļ���Ϣ���������ֵ
#define MAX_SONG_NUM 9999		//�����б����ļ����������ֵ

#define VERSION L"2.65"	//����汾
#define COMPILE_DATE L"2019/04/05"

//������ͼ��ť
#define IDT_PLAY_PAUSE 1321		//����������ͼ������/��ͣ����ť��ID
#define IDT_PREVIOUS 1322		//����������ͼ����һ������ť��ID
#define IDT_NEXT 1323			//����������ͼ����һ������ť��ID


//��ʱ��
#define TIMER_ID 1234			//���嶨ʱ����ID
#define TIMER_ELAPSE 80		//���嶨ʱ����Ӧ��ʱ���������룩
#define TIMER_ID_MINI 1235		//����ģʽ�еĶ�ʱ��ID
#define TIMER_ID_MINI2 1236		//����ģʽ�еĶ�ʱ��ID��������ʾƵ�׷���
#define TIMER_ELAPSE_MINI 200	//��������ģʽ�еĶ�ʱ����Ӧ��ʱ���������룩
#define TIMER_1_SEC 1237

//#define TIMER_ID_SPECTRAL 
//#define TIMER_ID_EXIT 1237

//Ƶ�׷���
#define FFT_SAMPLE 128			//Ƶ�׷�����������
//#define FFT_NUM 128			//Ҫʹ�õ�Ƶ�׵ĵ���
#define SPECTRUM_ROW 64			//Ƶ�׷������ε�����������Ϊ2�������η���С�ڻ����FFT_SAMPLE��

#define IDC_SAVE_COMBO_BOX 1990		//������ӵ������Ϊ���Ի����е���Ͽ��ID

//��ӵ�ϵͳ�˵��еġ�����ģʽ���˵����ID
#define IDM_MINIMODE 112

//������
#define EQU_CH_NUM 10		//������ͨ��������
const int EQU_STYLE_TABLE[9][EQU_CH_NUM]		//������Ԥ��
{
	{ 0,0,0,0,0,0,0,0,0,0 } ,			//��
	{ 4,3,3,2,2,1,0,-1,-2,-2 },		//�ŵ�
	{ 3,2,0,-1,-2,-2,-1,0,2,3 },		//����
	{ 2,1,0,-1,-1,1,3,5,3 },		//��ʿ
	{ -2,0,2,4,-1,-1,0,0,2,3 },		//ҡ��
	{ 1,0,0,1,2,1,-1,-2,-2,-2 },		//���
	{ 4,6,6,-2,-1,0,0,0,0,0 },		//�ص���
	{ -5,-5,-3,-2,-2,0,0,0,0,0 },		//��������
	{ 0,0,0,0,0,-1,-3,-5,-5,-4 }		//��������
};

#define GRAY(x) RGB(x,x,x)

#define ALBUM_COVER_NAME L"CurrentAlbumCover-MusicPlayer2-jMZB7TMf"
//#define DEFAULT_ALBUM_NAME L"cover"

//#define BACKGROUND_ALPHA 200	//���汳����͸����0~255
#define ALPHA_CHG(alpha) ((alpha)*255/100)		//��͸���ȴ�0~100ӳ�䵽0~255

#define LOCAL_DIR_STR L"%localdir%"

#define WM_OPEN_FILE_COMMAND_LINE (WM_USER+110)		//ͨ�������д��ļ�ʱ������ʵ�����ڷ��͵���Ϣ
#define MY_WM_NOTIFYICON (WM_USER+115)

#define WM_NEXT_USER_MSG (WM_USER+118)