#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <list>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib") 


struct K_User
{
    SOCKET sock;
    SOCKADDR_IN address;
};
std::list<K_User> userlist;

int serverDownCode = 1;

// 시작함수
DWORD WINAPI ServerThread(LPVOID lpThreadParameter)
{
    while (1)
    {
        for (auto iterRecv = userlist.begin(); userlist.end() != iterRecv;)
        {
            char szRecvMsg[256] = { 0, };
            int iRecvBytes = recv(iterRecv->sock, szRecvMsg, 256, 0);

            if (iRecvBytes == 0)
            {
                printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                closesocket(iterRecv->sock);
                iterRecv = userlist.erase(iterRecv);
                continue;
            }
            if (iRecvBytes == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    //WSAEWOULDBLOCK 아니라면 오류!
                    printf("클라이언트 접속 비정상 종료 : IP:%s, PORT:%d\n",
                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                    printf("%d Recv Error\n", iError); 
                    closesocket(iterRecv->sock);
                    iterRecv = userlist.erase(iterRecv);
                    continue;
                }
            }

            if (iRecvBytes > 0) // 누군가 메세지를 보냈으면
            {
                printf("%s\n", szRecvMsg);

                for (auto iterSend = userlist.begin(); userlist.end() != iterSend;)
                {
                    int iSendBytes = send(iterSend->sock, szRecvMsg, strlen(szRecvMsg), 0);
                    if (iSendBytes == SOCKET_ERROR)
                    {
                        int iError = WSAGetLastError();
                        if (iError != WSAEWOULDBLOCK)
                        {
                            //WSAEWOULDBLOCK 아니라면 오류!
                            printf("클라이언트 접속 비정상 종료 : IP:%s, PORT:%d\n", inet_ntoa(iterSend->address.sin_addr), ntohs(iterSend->address.sin_port));
                            printf("%d Send Error\n", iError);
                            closesocket(iterSend->sock);
                            iterSend = userlist.erase(iterSend);
                            continue;
                        }
                    }
                    iterSend++;
                }
            }
            iterRecv++;

            if (strcmp(szRecvMsg, "서버펑") == 0)
            {
                serverDownCode = 0;
                return 1;
            }
        }
    }
};



int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 0;
    };

    SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // 구한말때 함수긴 한데, 일단 이걸로
    sa.sin_port = htons(10000);

    int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) { return 1; }
    iRet = listen(ListenSock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) { return 1; }
    printf("서버 열림!\n");

    DWORD dwThreadID;
    HANDLE hClient = CreateThread(0, 0, ServerThread, 0, 0, &dwThreadID);

    // 소켓을 논블록 소켓으로 변경
    u_long iMode = TRUE;
    ioctlsocket(ListenSock, FIONBIO, &iMode);

    while(serverDownCode)
    {
        // 접속되면 반환된다.
        SOCKADDR_IN clientaddr;
        int length = sizeof(clientaddr);
        SOCKET clientSock = accept(ListenSock, (sockaddr*)&clientaddr, &length); //여기선 블록소켓 상태라서 누가 올때까지 대기함
        if (clientSock == SOCKET_ERROR) // accept 에러
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK) 
            {
                printf("%d Accept Error\n", iError);
                break;
            }
            continue;
        }
        printf("클라이언트 접속 : IP:%s, PORT:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 소켓을 논블록 소켓으로 변경
        u_long iMode = TRUE;
        ioctlsocket(clientSock, FIONBIO, &iMode);

        K_User user;
        user.sock = clientSock;
        user.address = clientaddr;
        userlist.push_back(user);
    }

    for (auto iter = userlist.begin(); userlist.end() != iter; iter++)
    {
        closesocket(iter->sock);
    }

    printf("5초 후에 꺼집니다.\n\n");
    printf("5\n");
    Sleep(1000);
    printf("4\n");
    Sleep(1000);
    printf("3\n");
    Sleep(1000);
    printf("2\n");
    Sleep(1000);
    printf("1\n");
    Sleep(1000);

    closesocket(ListenSock);

    WSACleanup();
}
