#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_GameCore.h"
#include "K_Network.h"


class Sample : public K_GameCore
{
public:
	bool Init()override;
	bool Frame()override;
	bool Render()override;
	bool Release()override;

public:
	void	ChatMsg(USER_PACKET& t);
	void	NameReq(USER_PACKET& t);

public:
	LRESULT	MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	bool	Run() override;
	void	Print(const WCHAR* msg, ...);
	void	PRINT(std::list<std::wstring> myArguments);

private:
	K_Network	m_Net;
	HWND		m_hEdit;
	HWND		m_hListbox;
	HWND		m_hSendBtn;
	bool		m_bNameSend = false;
};