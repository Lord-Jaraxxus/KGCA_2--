#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include "CallistoProtocol.h"

#pragma comment(lib, "ws2_32.lib") 

int ErrorCheck(int iRecvBytes)
{
	if (iRecvBytes > 0) return 1;  // 정상적으로 받거나 보냄

	else if (iRecvBytes == 0) // 서버 정상 종료 
	{
		printf("서버 정상 종료\n");
		return 0;
	}

	else if (iRecvBytes == SOCKET_ERROR) // 소켓 에러, WSAEWOULDBLOCK가 아니라면 찐 에러
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			//WSAEWOULDBLOCK 아니라면 오류!
			if (iError == WSAENOTSOCK) 
			{
				printf("서버에서 나갔습니다.\n");
				return iError;
			}
			
			else
			{
				printf("서버 터짐!\n");
				printf("%d Error\n", iError);
				return iError;
			}
		}
		return iError; // WSAEWOULDBLOCK 반환
	}
}

int SendPacket(SOCKET sock, char* msg, short type)
{
	USER_PACKET packet;
	ZeroMemory(&packet, sizeof(USER_PACKET));
	if (msg == nullptr) packet.ph.len = 0;
	else  
	{
		packet.ph.len = strlen(msg);
		memcpy(packet.msg, msg, strlen(msg)); 
	}
	packet.ph.type = type;

	char* msgSend = (char*)&packet;
	int iSendBytes = send(sock, msgSend, PACKET_HEADER_SIZE + packet.ph.len, 0);    // 사실 한번에 안보내질때도 있다는데, 그건 뭐 나중에..
	// 아니 근데 여기서 우드블록 안뜨나? 논블록해놨으면 뜰것같은데;;
	if (iSendBytes == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			//WSAEWOULDBLOCK 아니라면 오류!
			closesocket(sock);
			return iError;
		}
	}
	return 1;
}


// 시작함수
DWORD WINAPI SendThread(LPVOID lpThreadParameter)
{
	SOCKET sock = (SOCKET)lpThreadParameter;
	while (1)
	{
		char szSendMsg[256] = { 0, };
		fgets(szSendMsg, 256, stdin);
		szSendMsg[strlen(szSendMsg) - 1] = 0;
		if (strcmp(szSendMsg, "exit") == 0 || strcmp(szSendMsg, "나가기") == 0)
		{
			break;
		}

		int iError = SendPacket(sock, szSendMsg, PACKET_CHAR_MSG);
		if (iError != 1)
		{
			printf("%d Send Error\n", iError);
			break; // return 1? 아님 이대로 브레이크?
		}
	}
	closesocket(sock);
};




int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	};

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("192.168.0.46"); // 구한말때 함수긴 한데, 일단 이걸로
	sa.sin_port = htons(10000);

	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		printf("%d Error\n", iError);
		std::cout << "3초 후에 꺼집니다.\n";
		Sleep(3000);
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	printf("서버 접속!\n");


	//win api
	DWORD dwThreadID;
	HANDLE hClient = CreateThread(0, 0, SendThread, (LPVOID)sock, CREATE_SUSPENDED, &dwThreadID);

	// 소켓을 논블록 소켓으로 변경
	u_long iMode = TRUE;
	ioctlsocket(sock, FIONBIO, &iMode);

	int iTotalRecvBytes = 0;

	while (1)
	{
		Sleep(100);

		char szRecvMsg[256] = { 0, };
		int iRecvBytes = recv(sock, szRecvMsg, PACKET_HEADER_SIZE, 0); // 패킷 헤더만 일단 갖고옴
		// 근데 이거 만약 2바이트만 읽어오고 다음에 4바이트 읽어오고 하면어케됨? -> 아 그래서.. 귀찮은데 일단 하지마

		int iError = ErrorCheck(iRecvBytes);
		if (iError == WSAEWOULDBLOCK) continue;
		else if (iError != 1) break;

		iTotalRecvBytes += iRecvBytes;

		if (iTotalRecvBytes == PACKET_HEADER_SIZE) // 헤더를 다 갖고왔을 경우, 여기서부터 시작
		{
			PACKET_HEADER ph;
			ZeroMemory(&ph, sizeof(PACKET_HEADER));
			memcpy(&ph, szRecvMsg, PACKET_HEADER_SIZE);

			int iNumRecvByte = 0;

			switch (ph.type)
			{

			case PACKET_NICKNAME_REQ: // 첫 접속시 닉네임 등록 요청
			{
				printf("닉네임을 입력해주세요! : ");
				char szName[256] = { 0, };
				fgets(szName, 256, stdin);
				szName[strlen(szName) - 1] = 0;
				SendPacket(sock, szName, PACKET_NAME_REQ);
				ResumeThread(hClient);
			}break;

			case PACKET_NAME_ACK: // 닉네임 등록 반응 
			{
				char szRecvMsg[256] = { 0, };
				int iRecvBytes = recv(sock, &szRecvMsg[iNumRecvByte], ph.len - iNumRecvByte, 0);

				do {
					int iRecvBytes = recv(sock, &szRecvMsg[iNumRecvByte], ph.len - iNumRecvByte, 0);

					iError = ErrorCheck(iRecvBytes);
					if (iError == WSAEWOULDBLOCK) continue;
					else if (iError != 1) break;

					iNumRecvByte += iRecvBytes;
				} while (ph.len > iNumRecvByte);

				int iResult = (int)szRecvMsg; // 일케하면되나?아님말고ㅋㅋ

				if (iResult == 1) printf("닉네임이 등록되었습니다.\n");
				else if (iResult == 2) printf("중복된 닉네임이 있습니다.\n");
				else if (iResult == 3) printf("닉네임에 사용 불가능한 문자가 있습니다.\n");
				else printf("닉네임 등록 오류. 원인불명ㅋ\n");
			}break;

			case PACKET_CHAR_MSG: // 서버에서 온 채팅? 메세지?
			{
				char szRecvMsg[256] = { 0, };

				do {
					int iRecvBytes = recv(sock, &szRecvMsg[iNumRecvByte], ph.len - iNumRecvByte, 0);

					iError = ErrorCheck(iRecvBytes);
					if (iError == WSAEWOULDBLOCK) continue;
					else if (iError != 1) break;

					iNumRecvByte += iRecvBytes;
				} while (ph.len > iNumRecvByte);

				if (iError != 1) break;
				printf("%s\n", szRecvMsg);

			}break;
			}
			if (iError != 1) break;
			iTotalRecvBytes = 0;
		}
	}
	printf("3초 후에 꺼집니다.\n\n");
	printf("3\n");
	Sleep(1000);
	printf("2\n");
	Sleep(1000);
	printf("1\n");
	Sleep(1000);

	CloseHandle(hClient);
	closesocket(sock);
	WSACleanup();
}