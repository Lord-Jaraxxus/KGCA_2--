#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <list>
#include <WinSock2.h>
#include "CallistoProtocol.h"

#pragma comment(lib, "ws2_32.lib") 

int serverDownCode = 1;

struct K_User
{
    SOCKET sock;
    SOCKADDR_IN address;
    char m_szName[9] = { 0, };
    char szRecvMsg[255] = { 0, };
    int iTotalRecvBytes = 0;
};
std::list<K_User> userlist;


int ErrorCheck(int iRecvBytes)
{
    if (iRecvBytes > 0) return 1;  // ���������� �ްų� ����

    else if (iRecvBytes == 0) // ���� ���� 
    {
        //printf("Ŭ���̾�Ʈ ���� ����\n");
        return 0;
    }

    else if (iRecvBytes == SOCKET_ERROR) // ���� ����, WSAEWOULDBLOCK�� �ƴ϶�� �� ����
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            //WSAEWOULDBLOCK �ƴ϶�� ����!
            return iError;
        }
        return iError; // WSAEWOULDBLOCK ��ȯ
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
    int iSendBytes = send(sock, msgSend, PACKET_HEADER_SIZE + packet.ph.len, 0);    // ��� �ѹ��� �Ⱥ��������� �ִٴµ�, �װ� �� ���߿�..
    // �ƴ� �ٵ� ���⼭ ����� �ȶ߳�? �����س����� ��Ͱ�����;;
    if (iSendBytes == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            //WSAEWOULDBLOCK �ƴ϶�� ����!
            closesocket(sock);
            return iError;
        }
    }
    return 1;
}

// �����Լ�
DWORD WINAPI ServerThread(LPVOID lpThreadParameter)
{
    while (1)
    {
        Sleep(1);

        for (auto iterRecv = userlist.begin(); userlist.end() != iterRecv;)
        {
            int iError = 0;
            char szRecvMsg[256] = { 0, };
            int iRecvBytes = recv(iterRecv->sock, szRecvMsg, PACKET_HEADER_SIZE, 0);

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
                iError = WSAGetLastError();
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

            if (iRecvBytes == PACKET_HEADER_SIZE) // ����� �о�Դٸ�
            {
                PACKET_HEADER ph;
                ZeroMemory(&ph, sizeof(PACKET_HEADER));
                memcpy(&ph, szRecvMsg, PACKET_HEADER_SIZE);

                int iNumRecvByte = 0;

                switch (ph.type)
                {
                case PACKET_CHAR_MSG: // Ŭ�󿡼� �� ä��
                {
                    char szRecvMsg[256] = { 0, };
         
                    int iRecvBytes;
                    do {
                        iRecvBytes = recv(iterRecv->sock, &szRecvMsg[iNumRecvByte], ph.len - iNumRecvByte, 0);

                        if (iRecvBytes == 0)
                        {
                            printf("Ŭ���̾�Ʈ ���� ���� : IP:%s, PORT:%d\n",
                                inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                            closesocket(iterRecv->sock);
                            iterRecv = userlist.erase(iterRecv);
                            break;
                        }
                        if (iRecvBytes == SOCKET_ERROR)
                        {
                            iError = WSAGetLastError();
                            if (iError != WSAEWOULDBLOCK)
                            {
                                //WSAEWOULDBLOCK �ƴ϶�� ����!
                                printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n",
                                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                                printf("%d Recv Error\n", iError);
                                closesocket(iterRecv->sock);
                                iterRecv = userlist.erase(iterRecv);
                                break;
                            }
                        }
                        iNumRecvByte += iRecvBytes;

                        //Sleep(0);
                    } while (ph.len > iNumRecvByte);

                    if (iRecvBytes == SOCKET_ERROR && iError != WSAEWOULDBLOCK) break;

                    else printf("[%s] : %s\n", iterRecv->m_szName, szRecvMsg);
                    std::string pMsg = "[";
                    pMsg += iterRecv->m_szName;
                    pMsg += "] : ";
                    pMsg += szRecvMsg;
                    ZeroMemory(szRecvMsg, sizeof(pMsg));
                    memcpy(szRecvMsg, pMsg.c_str(), pMsg.size());

                    for (auto iterSend = userlist.begin(); userlist.end() != iterSend;)
                    {
                        //if (iterSend != iterRecv) 
                        {
                            int iError = SendPacket(iterSend->sock, szRecvMsg, PACKET_CHAR_MSG);
                            if (iError != 1)
                            {
                                printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n", inet_ntoa(iterSend->address.sin_addr), ntohs(iterSend->address.sin_port));
                                printf("%d Send Error\n", iError);
                                closesocket(iterSend->sock);
                                iterSend = userlist.erase(iterSend);
                                continue;
                            }
                        }
                        iterSend++;
                    }
                }break;

                case PACKET_NAME_REQ: 
                {
                    int iRecvBytes;

                    do {
                        iRecvBytes = recv(iterRecv->sock, &iterRecv->m_szName[iNumRecvByte], ph.len - iNumRecvByte, 0);

                        if (iRecvBytes == 0)
                        {
                            printf("Ŭ���̾�Ʈ ���� ���� : IP:%s, PORT:%d\n",
                                inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                            closesocket(iterRecv->sock);
                            iterRecv = userlist.erase(iterRecv);
                            break;
                        }
                        if (iRecvBytes == SOCKET_ERROR)
                        {
                            iError = WSAGetLastError();
                            if (iError != WSAEWOULDBLOCK)
                            {
                                //WSAEWOULDBLOCK �ƴ϶�� ����!
                                printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n",
                                    inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                                printf("%d Recv Error\n", iError);
                                closesocket(iterRecv->sock);
                                iterRecv = userlist.erase(iterRecv);
                                break;
                            }
                        }
                        iNumRecvByte += iRecvBytes;

                        //Sleep(0);
                    } while (ph.len > iNumRecvByte);

                    if (iRecvBytes == SOCKET_ERROR && iError != WSAEWOULDBLOCK) break;
                    
                    printf("�Է¹��� �г��� : [%s]\n", iterRecv->m_szName);

                    for (auto iterSend = userlist.begin(); userlist.end() != iterSend;)
                    {
                        if (iterSend == iterRecv) 
                        {
                            iError = SendPacket(iterRecv->sock, iterRecv->m_szName, PACKET_NAME_ACK);
                            if (iError != 1)
                            {
                                printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n", inet_ntoa(iterRecv->address.sin_addr), ntohs(iterRecv->address.sin_port));
                                printf("%d Send Error\n", iError);
                                closesocket(iterRecv->sock);
                            }
                        }

                        else
                        {
                            int iError = SendPacket(iterSend->sock, iterRecv->m_szName, PACKET_USER_JOIN);
                            if (iError != 1)
                            {
                                printf("Ŭ���̾�Ʈ ���� ������ ���� : IP:%s, PORT:%d\n", inet_ntoa(iterSend->address.sin_addr), ntohs(iterSend->address.sin_port));
                                printf("%d Send Error\n", iError);
                                closesocket(iterSend->sock);
                                iterSend = userlist.erase(iterSend);
                                continue;
                            }
                        }
                        iterSend++;
                    }

                }break;

                }
            }
            iterRecv++;
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

    while (1)
    {
        Sleep(1);

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

        SendPacket(clientSock, nullptr, PACKET_NICKNAME_REQ); // ���⼱ ����üũ�� ���°Ͱ�����.. �Ƹ���
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
