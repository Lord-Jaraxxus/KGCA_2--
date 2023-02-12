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
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // 구한말때 함수긴 한데, 일단 이걸로
    sa.sin_port = htons(10000);

    int iRet = bind(sock0, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) { return 1; }
    iRet = listen(sock0, SOMAXCONN);
    if (iRet == SOCKET_ERROR) { return 1; }
    printf("서버 열림!\n");

    // 접속되면 반환된다.
    SOCKADDR_IN clientaddr;
    int length = sizeof(clientaddr);
    SOCKET clientSock = accept(sock0, (sockaddr*)&clientaddr, &length);
    printf("클라이언트 접속 : IP:%s, PORT:%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    while(1)
    {
        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(clientSock, szRecvMsg, 256, 0);
        if (iRecvBytes == 0)
        {
            printf("클라이언트가 접속을 종료했습니다.\n");
            break;
        }
        else if (sizeof(szRecvMsg) != 0)
        {
            printf("상대 : %s\n", szRecvMsg);
        }
        
        printf("나 : ");
        char szSendMsg[256] = { 0, };
        fgets(szSendMsg, 256, stdin);
        int iSendBytes = send(clientSock, szSendMsg, strlen(szRecvMsg), 0);
    }

    //printf("클라이언트가 접속을 종료했습니다.\n");
    Sleep(5000);

    closesocket(sock0);

    WSACleanup();
}
