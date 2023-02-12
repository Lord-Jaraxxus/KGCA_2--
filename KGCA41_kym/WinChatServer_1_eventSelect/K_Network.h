#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NETWORK_MSG WM_USER+80

#include "K_Std.h"
#include "CallistoProtocol.h"

struct K_User
{
	SOCKET sock;
	SOCKADDR_IN address;
	char m_szName[9] = { 0, };
	char szRecvMsg[255] = { 0, };
	int iTotalRecvBytes = 0;
};


class PrintWrapper
{
public:
	void operator()(std::list<std::wstring> myArguments)
	{
		func(myArguments);
	}
	std::function< void(std::list<std::wstring> myArguments)> func;
};


class K_Network
{
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();

public:
	K_Network();
	virtual ~K_Network();

public:		// 콜백 시스템
	typedef std::function<void(USER_PACKET& t)> CallFuction;
	typedef std::map<DWORD, CallFuction>::iterator FunIter;
	std::map<DWORD, CallFuction> m_fnExecutePacket;
public:
	void	ChatMsg(USER_PACKET& t);
	void	NameReq(USER_PACKET& t);

public:
	bool   NetStart(std::string ip, int iPort);
	void   MakePacket(USER_PACKET& packet, const char* msg, int iSize, short type);
	int    SendMsg(SOCKET sock, const char* msg, int iSize, short type);
	void   AddSend(const char* msg, int iSize, short type);
	int	   ErrorCheck(int iRecvBytes, std::list<K_User>::iterator &iter);
	void   SendProcess();
	void   RecvProcess();
	void   PacketProcess();

public:
	SOCKET m_Sock = 0;
	HANDLE m_hClientThread;
	std::list<USER_PACKET>  m_RecvPacketList;
	std::list<USER_PACKET>  m_SendPacketList;

public:
	FD_SET m_rSet;
	FD_SET m_wSet;
	std::list<K_User> userlist;

public:
	PrintWrapper m_Print;

};

