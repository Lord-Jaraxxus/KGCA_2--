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
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // ���Ѹ��� �Լ��� �ѵ�, �ϴ� �̰ɷ�
    sa.sin_port = htons(10000);

    int iRet = bind(sock0, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) { return 1; }
    iRet = listen(sock0, SOMAXCONN);
    if (iRet == SOCKET_ERROR) { return 1; }
    printf("���� ����!\n");

    // ���ӵǸ� ��ȯ�ȴ�.
    SOCKADDR_IN clientaddr;
    int length = sizeof(clientaddr);
    SOCKET clientSock = accept(sock0, (sockaddr*)&clientaddr, &length);
    printf("Ŭ���̾�Ʈ ���� : IP:%s, PORT:%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    while(1)
    {
        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(clientSock, szRecvMsg, 256, 0);
        if (iRecvBytes == 0)
        {
            printf("Ŭ���̾�Ʈ�� ������ �����߽��ϴ�.\n");
            break;
        }
        else if (sizeof(szRecvMsg) != 0)
        {
            printf("��� : %s\n", szRecvMsg);
        }
        
        printf("�� : ");
        char szSendMsg[256] = { 0, };
        fgets(szSendMsg, 256, stdin);
        int iSendBytes = send(clientSock, szSendMsg, strlen(szRecvMsg), 0);
    }

    //printf("Ŭ���̾�Ʈ�� ������ �����߽��ϴ�.\n");
    Sleep(5000);

    closesocket(sock0);

    WSACleanup();
}
