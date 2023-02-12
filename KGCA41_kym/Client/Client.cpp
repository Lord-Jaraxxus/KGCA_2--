#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        return 0;
    };

    SOCKET sock0 = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("192.168.0.46"); // 구한말때 함수긴 한데, 일단 이걸로
    sa.sin_port = htons(10000);

    int iRet = connect(sock0, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) { return 1; }
    printf("서버 접속!\n");

    while (1) 
    {
        printf("나 : ");
        char szSendMsg[256] = { 0, };
        fgets(szSendMsg, 256, stdin);
        int iSendBytes = send(sock0, szSendMsg, strlen(szSendMsg), 0);

        //Sleep(1000);

        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(sock0, szRecvMsg, 256, 0);
        printf("상대 : %s\n", szRecvMsg);
    }
        
    Sleep(3000);

    closesocket(sock0);

    WSACleanup();
}
