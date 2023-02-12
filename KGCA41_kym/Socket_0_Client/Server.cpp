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

// �����Լ�
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
                printf("Ŭ���̾�Ʈ ���� ���� : IP:%s, PORT:%d\n",
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
                    //WSAEWOULDBLOCK �ƴ϶�� ����!
                    printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n",
                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                    printf("%d Recv Error\n", iError); 
                    closesocket(iterRecv->sock);
                    iterRecv = userlist.erase(iterRecv);
                    continue;
                }
            }

            if (iRecvBytes > 0) // ������ �޼����� ��������
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
                            //WSAEWOULDBLOCK �ƴ϶�� ����!
                            printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n", inet_ntoa(iterSend->address.sin_addr), ntohs(iterSend->address.sin_port));
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

            if (strcmp(szRecvMsg, "������") == 0)
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
    sa.sin_addr.s_addr = htonl(INADDR_ANY); // ���Ѹ��� �Լ��� �ѵ�, �ϴ� �̰ɷ�
    sa.sin_port = htons(10000);

    int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR) { return 1; }
    iRet = listen(ListenSock, SOMAXCONN);
    if (iRet == SOCKET_ERROR) { return 1; }
    printf("���� ����!\n");

    DWORD dwThreadID;
    HANDLE hClient = CreateThread(0, 0, ServerThread, 0, 0, &dwThreadID);

    // ������ ���� �������� ����
    u_long iMode = TRUE;
    ioctlsocket(ListenSock, FIONBIO, &iMode);

    while(serverDownCode)
    {
        // ���ӵǸ� ��ȯ�ȴ�.
        SOCKADDR_IN clientaddr;
        int length = sizeof(clientaddr);
        SOCKET clientSock = accept(ListenSock, (sockaddr*)&clientaddr, &length); //���⼱ ��ϼ��� ���¶� ���� �ö����� �����
        if (clientSock == SOCKET_ERROR) // accept ����
        {
            int iError = WSAGetLastError();
            if (iError != WSAEWOULDBLOCK) 
            {
                printf("%d Accept Error\n", iError);
                break;
            }
            continue;
        }
        printf("Ŭ���̾�Ʈ ���� : IP:%s, PORT:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ���� �������� ����
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

    printf("5�� �Ŀ� �����ϴ�.\n\n");
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
