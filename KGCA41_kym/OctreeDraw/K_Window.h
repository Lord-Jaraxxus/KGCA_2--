#pragma once
#include "K_Std.h"

class K_Window 
{
public: 
	HINSTANCE	m_hInstance; // 접수번호
	HWND		m_hWnd;		 // 등기번호
	RECT		m_rtWindow;
	RECT		m_rtClient;
	DWORD		m_csStyle;
	UINT		m_iClientWidth;
	UINT		m_iClientHeight;

	bool m_bGameRun;
	float m_fGameTimer = 0.0f;
	float m_ElapseTimer = 0.0f;

#ifdef _DEBUG
	std::vector<UINT> msglist;
#endif

public:
	bool SetWindow(HINSTANCE hInstance, const WCHAR* szTitle, UINT iWidth = 1600, UINT iHeight = 900);
	ATOM MyRegisterClass();
	BOOL InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool Run();
	void CenterWindow();

public:
	virtual bool TCoreInit();
	virtual bool TCoreFrame();
	virtual bool TCoreRender();
	virtual bool TCoreRelease();

public:
	K_Window();

}; 

