#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NETWORK_MSG WM_USER+80

#include "K_Std.h"
#include "CallistoProtocol.h"

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

public:
	bool   NetStart(std::string ip, int iPort);
	void   MakePacket(USER_PACKET& packet, const char* msg, int iSize, short type);
	int    SendMsg(SOCKET sock, const char* msg, int iSize, short type);
	void   AddSend(SOCKET sock, const char* msg, int iSize, short type);
	int	   ErrorCheck(int iRecvBytes);
	void   SendProcess();
	void   RecvProcess();
	void   PacketProcess();

public:
	SOCKET m_Sock = 0;
	HANDLE m_hClientThread;
	std::list<USER_PACKET>  m_RecvPacketList;
	std::list<USER_PACKET>  m_SendPacketList;

public:
	bool m_bConnect = false;
};

