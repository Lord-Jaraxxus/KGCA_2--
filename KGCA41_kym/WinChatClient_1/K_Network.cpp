#include "K_Network.h"

// 시작함수
DWORD WINAPI RecvThread(LPVOID lpThreadParameter)
{
    K_Network* net = (K_Network*)lpThreadParameter;
    SOCKET sock = net->m_Sock;
    int iRecvPacketSize = PACKET_HEADER_SIZE;
    int iTotalRecvBytes = 0;

    while (1)
    {
        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(net->m_Sock, szRecvMsg,
            PACKET_HEADER_SIZE - iTotalRecvBytes, 0);
        if (iRecvBytes == 0)
        {
            printf("서버 정상 종료\n");
            return true;
        }
        iTotalRecvBytes += iRecvBytes;
        if (iTotalRecvBytes == PACKET_HEADER_SIZE)
        {
            USER_PACKET packet;
            ZeroMemory(&packet, sizeof(USER_PACKET));
            memcpy(&packet.ph, szRecvMsg, PACKET_HEADER_SIZE);

            char* msg = (char*)&packet;
            int iNumRecvByte = 0;
            do {
                if (packet.ph.len == 0)
                {
                    break;
                }
                int iRecvBytes = recv(net->m_Sock,
                    &packet.msg[iNumRecvByte],
                    packet.ph.len - iNumRecvByte, 0);

                if (iRecvBytes == 0)
                {
                    //printf("서버 정상 종료\n");
                    break;
                }
                if (iRecvBytes == SOCKET_ERROR)
                {
                    int iError = WSAGetLastError();
                    if (iError != WSAEWOULDBLOCK)
                    {
                        //WSAEWOULDBLOCK 아니라면 오류!
                        closesocket(net->m_Sock);
                        //printf("서버 비정상 종료\n");
                        return 1;
                    }
                    continue;
                }
                iNumRecvByte += iRecvBytes;


            } while ((packet.ph.len) > iNumRecvByte);

            net->m_PacketList.push_back(packet);
            iTotalRecvBytes = 0;
        }
    }
    closesocket(sock);
};

bool K_Network::Init()
{
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
    // client
    SOCKADDR_IN sa;//목적지+포트
    sa.sin_family = AF_INET;
    //error C4996: 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    sa.sin_port = htons(iPort);
    int iRet = connect(m_Sock, (sockaddr*)&sa, sizeof(sa));
    if (iRet == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        printf("%d ", iError);
        return false;
    }

    // u_long iMode = TRUE;
    // ioctlsocket(m_Sock, FIONBIO, &iMode);

     //win api
    DWORD dwThreadID;
    m_hClientThread = CreateThread(0, 0, RecvThread,
        (LPVOID)this, 0, &dwThreadID);

    return true;
}

int K_Network::SendMsg(SOCKET sock, const char* msg, short type)
{
    USER_PACKET packet;
    ZeroMemory(&packet, sizeof(USER_PACKET));
    packet.ph.len = strlen(msg);
    packet.ph.type = type;
    memcpy(packet.msg, msg, strlen(msg));

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
