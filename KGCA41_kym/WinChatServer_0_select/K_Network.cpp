#include "K_Network.h"

bool K_Network::Init()
{
	using namespace std::placeholders;
	m_fnExecutePacket[PACKET_CHAR_MSG] = std::bind(&K_Network::ChatMsg, this, _1);
	m_fnExecutePacket[PACKET_NAME_REQ] = std::bind(&K_Network::NameReq, this, _1);

	return true;
}

bool K_Network::Frame()
{
	return true;
}

bool K_Network::Render()
{
	return true;
}

bool K_Network::Release()
{
	closesocket(m_Sock);
	return true;
}

K_Network::K_Network()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
}

K_Network::~K_Network()
{
	WSACleanup();
}

bool K_Network::NetStart(std::string ip, int iPort)
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;//목적지+포트
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(iPort);

	int iRet = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 1; }
	iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR) { return 1; }

	return true;
}

void  K_Network::MakePacket(USER_PACKET& packet, const char* msg, int iSize, short type)
{
	ZeroMemory(&packet, sizeof(USER_PACKET));
	packet.ph.len = iSize;
	packet.ph.type = type;
	memcpy(packet.msg, msg, iSize);
}

int K_Network::SendMsg(SOCKET sock, const char* data, int iSize, short type)
{
	USER_PACKET packet;
	MakePacket(packet, data, iSize, type);

	char* msgSend = (char*)&packet;
	int iSendBytes = send(sock, msgSend, packet.ph.len + PACKET_HEADER_SIZE, 0);

	if (iSendBytes == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//WSAEWOULDBLOCK 아니라면 오류!
			closesocket(sock);
			return -1;
		}
	}
	return 1;
}

void   K_Network::AddSend(const char* data, int iSize, short type)
{
	USER_PACKET packet;
	MakePacket(packet, data, iSize, type);
	m_SendPacketList.push_back(packet);
}


int K_Network::ErrorCheck(int iRecvBytes, std::list<K_User>::iterator& iter)
{
	if (iRecvBytes > 0) return 1;  // 정상적으로 받거나 보냄

	else if (iRecvBytes == 0) // 서버 정상 종료 
	{
		m_Print({ L"클라이언트 접속 종료 -> IP:" + to_mw(inet_ntoa(iter->address.sin_addr)),
				  L" PORT:" + std::to_wstring(ntohs(iter->address.sin_port)) });
		
		std::string logout = iter->m_szName;
		logout += "님이 퇴장하셨습니다.";
		m_Print({ to_mw(logout) }); 

		USER_PACKET LogOutPacket;
		MakePacket(LogOutPacket, logout.c_str(), sizeof(logout), PACKET_CHAR_MSG);
		m_SendPacketList.push_back(LogOutPacket);

		closesocket(iter->sock);
		iter = userlist.erase(iter);
		return 0;
	}

	else if (iRecvBytes == SOCKET_ERROR) // 소켓 에러, WSAEWOULDBLOCK가 아니라면 찐 에러
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			m_Print({ L"클라이언트 비정상 접속 종료 -> IP:" + to_mw(inet_ntoa(iter->address.sin_addr)),
					  L" PORT:" + std::to_wstring(ntohs(iter->address.sin_port)) });

			std::string logout = iter->m_szName;
			logout += "님이 튕기셨습니다.ㅋ";
			m_Print({ to_mw(logout) });

			USER_PACKET LogOutPacket;
			MakePacket(LogOutPacket, logout.c_str(), sizeof(logout), PACKET_CHAR_MSG);
			m_SendPacketList.push_back(LogOutPacket);

			closesocket(iter->sock);
			iter = userlist.erase(iter);
			return iError;
		}
		return iError; // WSAEWOULDBLOCK 반환
	}
}

void   K_Network::SendProcess()
{
	for (auto& packet : m_SendPacketList)
	{
		for (auto iter = userlist.begin(); userlist.end() != iter; )
		{
			int iSendBytes = send(iter->sock, (char*)&packet, PACKET_HEADER_SIZE + packet.ph.len, 0);
			int iError = ErrorCheck(iSendBytes, iter);
			if (iError != 1) // 정상적으로 작동하지 않았다면
			{
				if (iError == WSAEWOULDBLOCK) { iter++; }
				continue;
			}
			iter++;
		}
	}
	m_SendPacketList.clear();
}

void   K_Network::RecvProcess()
{
	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_SET(m_Sock, &m_rSet);
	FD_SET(m_Sock, &m_wSet);
	for (auto& user : userlist)
	{
		FD_SET(user.sock, &m_rSet);
		FD_SET(user.sock, &m_wSet);
	}
	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1000; // 1/1000000초
	int iRet = select(0, &m_rSet, &m_wSet, NULL, &time);
	if (iRet == SOCKET_ERROR) { return; }
	if (iRet == 0) { return; } // timeout


	// 리스닝 소켓 검사, 새로 접속한 유저가 있으면 돌아감
	if (FD_ISSET(m_Sock, &m_rSet))
	{
		SOCKADDR_IN clientaddr;
		int length = sizeof(clientaddr);
		SOCKET clientSock = accept(m_Sock, (sockaddr*)&clientaddr, &length);
		if (clientSock == SOCKET_ERROR)
		{
			closesocket(m_Sock);
			WSACleanup();
			return;
		}
		m_Print({ L"클라이언트 접속 -> IP:" + to_mw(inet_ntoa(clientaddr.sin_addr)),
				   L" PORT:" + std::to_wstring(ntohs(clientaddr.sin_port)) });

		u_long iMode = TRUE;
		ioctlsocket(clientSock, FIONBIO, &iMode);

		K_User user;
		user.sock = clientSock;
		user.address = clientaddr;
		userlist.push_back(user);

		SendMsg(clientSock, nullptr, 0, PACKET_NICKNAME_REQ);
	}

	// 유저 리스트를 빙빙 돔
	for (auto iter = userlist.begin(); userlist.end() != iter; )
	{
		USER_PACKET packet;
		ZeroMemory(&packet, sizeof(USER_PACKET));

		// 읽을 걸 보낸( == send한) 소켓이 있으면 돌아감
		if (FD_ISSET(iter->sock, &m_rSet))
		{
			int iRecvPacketSize = PACKET_HEADER_SIZE;

			int iRecvBytes = recv(iter->sock, iter->szRecvMsg, PACKET_HEADER_SIZE - iter->iTotalRecvBytes, 0);
			int iError = ErrorCheck(iRecvBytes, iter);
			if (iError != 1) // 정상적으로 작동하지 않았다면
			{
				if (iError == WSAEWOULDBLOCK) { iter++; }
				continue;
			}

			iter->iTotalRecvBytes += iRecvBytes;

			// 패킷 헤더만큼 Recv를 받아온 후 돌아감
			if (iter->iTotalRecvBytes == PACKET_HEADER_SIZE)
			{
				memcpy(&packet.ph, iter->szRecvMsg, PACKET_HEADER_SIZE);

				char* msg = (char*)&packet;
				int iNumRecvByte = 0;
				do {
					int iRecvBytes = recv(iter->sock, &packet.msg[iNumRecvByte], packet.ph.len - iNumRecvByte, 0);
					iError = ErrorCheck(iRecvBytes, iter);
					if (iError != 1) // 정상적으로 작동하지 않았다면
					{
						if (iError == WSAEWOULDBLOCK) { iter++; }
						continue;
					}

					iNumRecvByte += iRecvBytes;
				} while ((packet.ph.len - PACKET_HEADER_SIZE) > iNumRecvByte);
			}

			// 만약 받은 패킷이 채팅이라면.. 아니 여기서 이러면 안될 것 같긴 한데 방법이 딱히 없네
			if (packet.ph.type == PACKET_CHAR_MSG)
			{
				std::string pMsg = "[";
				pMsg += iter->m_szName;
				pMsg += "] : ";
				pMsg += packet.msg;
				ZeroMemory(packet.msg, sizeof(pMsg));
				memcpy(packet.msg, pMsg.c_str(), pMsg.size());
				packet.ph.len = sizeof(packet.msg);
			}

			if (iRecvBytes > 0) { m_RecvPacketList.push_back(packet); }
			iter->iTotalRecvBytes = 0;
		}

		// 만약 iter소켓이 받을 준비가 되어있고 닉네임을 보냈던거라면
		if (FD_ISSET(iter->sock, &m_wSet))
		{
			if (packet.ph.type == PACKET_NAME_REQ)
			{
				//ZeroMemory(iter->m_szName, sizeof(packet.msg));
				memcpy(iter->m_szName, packet.msg, sizeof(iter->m_szName));
				SendMsg(iter->sock, packet.msg, sizeof(packet.msg), PACKET_NAME_ACK);
			}
		}
		iter++;
	}
}

// m_RecvPacketList에 들어온 패킷을 콜백 시스템으로 처리
void K_Network::PacketProcess()
{
	for (auto& packet : m_RecvPacketList)
	{
		K_Network::FunIter iter = m_fnExecutePacket.find(packet.ph.type);
		if (iter != m_fnExecutePacket.end())
		{
			K_Network::CallFuction call = iter->second;
			call(packet);
		}
	}
	m_RecvPacketList.clear();
}