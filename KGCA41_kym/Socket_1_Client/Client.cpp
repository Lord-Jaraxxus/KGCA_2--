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
    if (iRet == SOCKET_ERROR) 
    {
        int iError = WSAGetLastError();
        printf("%d Error\n", iError);
        std::cout << "3초 후에 꺼집니다.\n";
        Sleep(3000);
        closesocket(sock0);
        WSACleanup();
        return 1;
    }
    printf("서버 접속!\n");
    
    // 소켓을 논블록 소켓으로 변경
    u_long iMode = TRUE;
    ioctlsocket(sock0, FIONBIO, &iMode);

    int count = 0;
    while (count < 3)
    {
        count++;

        printf("나 : ");
        char szSendMsg[256] = { 0, };
        fgets(szSendMsg, 256, stdin);
        int iSendBytes = send(sock0, szSendMsg, strlen(szSendMsg), 0);
        if (iSendBytes == SOCKET_ERROR)
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK)
            {
                //WSAEWOULDBLOCK 아니라면 오류!
                printf("%d Error\n", iError);
                break;
            }
            continue;
        }


        //Sleep(1000);

        char szRecvMsg[256] = { 0, };
        int iRecvBytes = SOCKET_ERROR;
        //while (iRecvBytes == SOCKET_ERROR)
        {
            iRecvBytes = recv(sock0, szRecvMsg, 256, 0);
            if (iRecvBytes == SOCKET_ERROR)
            {
                int iError = WSAGetLastError();
                if (iError != WSAEWOULDBLOCK)
                {
                    //WSAEWOULDBLOCK 아니라면 오류!
                    printf("%d Error\n", iError);
                    break;
                }
                continue;
            }
        }
        printf("상대 : %s\n", szRecvMsg);

    }

    printf("3초 후에 꺼집니다.\n\n");
    printf("3\n");
    Sleep(1000);
    printf("2\n");
    Sleep(1000);
    printf("1\n");
    Sleep(1000);

    closesocket(sock0);

    WSACleanup();
}
