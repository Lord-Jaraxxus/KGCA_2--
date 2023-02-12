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

public:		// 콜백 시스템
	typedef std::function<void(USER_PACKET& t)> CallFuction;
	typedef std::map<DWORD, CallFuction>::iterator FunIter;
	std::map<DWORD, CallFuction> m_fnExecutePacket;
public:
	void	ChatMsg(USER_PACKET& t);
	void    ChatNameReq(USER_PACKET& t);
	void	UserJoin(USER_PACKET& t);
	void	NameAck(USER_PACKET& t);

public:
	LRESULT	MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	bool	Run() override;
	void	NetRun();
	void	Print(const WCHAR* msg, ...);
	void    PacketProcess();

private:
	K_Network	m_Net;
	HWND		m_hEdit;
	HWND		m_hListbox;
	HWND		m_hSendBtn;
	bool		m_bNameSend = false;
};