#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
	int    SendMsg(SOCKET sock, const char* msg, short type);


public:
	SOCKET m_Sock = 0;
	HANDLE m_hClientThread;
	std::list<USER_PACKET>  m_PacketList;
};

