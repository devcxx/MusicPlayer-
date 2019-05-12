#pragma once

struct WinVersion
{
public:
	WinVersion();

	int m_major_version;
	int m_minor_version;
	int m_build_number;
};

class CWinVersionHelper
{
public:
	~CWinVersionHelper();

	static bool IsWindows10FallCreatorOrLater();		//�жϵ�ǰWindows�汾�Ƿ�ΪWin10�＾�����߸��»���µİ汾
	static bool IsWindowsVista();
	static bool IsWindows7();					//�ж�Windows�汾�Ƿ�ΪWindows7
	static bool IsWindows7OrLater();
	static bool IsWindows8Or8point1();			//�ж�Windows�汾�Ƿ�ΪWindows8��Windows8.1
	static bool IsWindows8OrLater();
	static bool IsWindows10OrLater();
	static bool IsWindows10Version1809OrLater();

	static int GetMajorVersion() { return m_version.m_major_version; }
	static int GetMinorVersion() { return m_version.m_minor_version; }
	static int GetBuildNumber() { return m_version.m_build_number; }

private:
	CWinVersionHelper();
	static WinVersion m_version;
};

