#include "K_Network.h"

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

    // ����Ʈ �� ����, ���̽�ũ(�񵿱�) ����Ʈ
    int iRet = WSAAsyncSelect(m_Sock, g_hWnd, NETWORK_MSG,
        FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    if (iRet == SOCKET_ERROR){ return false; }
    
    // client
    SOCKADDR_IN sa;//������+��Ʈ
    sa.sin_family = AF_INET;
    //error C4996: 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    sa.sin_port = htons(iPort);
    iRet = connect(m_Sock, (sockaddr*)&sa, sizeof(sa));

    // ���� �� �ּ��� �Ȱ���? ����̾ȳ�
    //if (iRet == SOCKET_ERROR)
    //{
    //    int iError = WSAGetLastError();
    //    printf("%d ", iError);
    //    return false;
    //}

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
    int iSendBytes = send(sock, msgSend, packet.ph.len, 0);

    if (iSendBytes == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            //WSAEWOULDBLOCK �ƴ϶�� ����!
            closesocket(sock);
            return -1;
        }
    }
    return 1;
}

void   K_Network::AddSend(SOCKET sock, const char* data, int iSize, short type)
{
    USER_PACKET packet;
    MakePacket(packet, data, iSize, type);
    m_SendPacketList.push_back(packet);
}


int K_Network::ErrorCheck(int iRecvBytes)
{
    if (iRecvBytes > 0) return 1;  // ���������� �ްų� ����

    else if (iRecvBytes == 0) // ���� ���� ���� 
    {
        printf("���� ���� ����\n");
        return 0;
    }

    else if (iRecvBytes == SOCKET_ERROR) // ���� ����, WSAEWOULDBLOCK�� �ƴ϶�� �� ����
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
            //WSAEWOULDBLOCK �ƴ϶�� ����!
            if (iError == WSAENOTSOCK)
            {
                printf("�������� �������ϴ�.\n");
                return iError;
            }

            else
            {
                printf("���� ����!\n");
                printf("%d Error\n", iError);
                return iError;
            }
        }
        return iError; // WSAEWOULDBLOCK ��ȯ
    }
}

void   K_Network::SendProcess()
{
    for (auto& packet : m_SendPacketList)
    {
        char* msgSend = (char*)&packet;
        int iSendBytes = send(m_Sock, msgSend, packet.ph.len, 0);
        int iError = ErrorCheck(iSendBytes);

        if (iError != 1 && iError != WSAEWOULDBLOCK)
        {
            closesocket(m_Sock);
            break;
        }
    }
    m_SendPacketList.clear();
}

void   K_Network::RecvProcess() 
{
    int iRecvPacketSize = PACKET_HEADER_SIZE;
    int iTotalRecvBytes = 0;
    int iBug = 0;

    while (iBug < 500)
    {
        //Sleep(1);
        iBug++;

        char szRecvMsg[256] = { 0, };
        int iRecvBytes = recv(m_Sock, szRecvMsg, PACKET_HEADER_SIZE - iTotalRecvBytes, 0);
        int iError = ErrorCheck(iRecvBytes);
        if (iError != 1)
        {
            if (iError == WSAEWOULDBLOCK) continue; // WSAEWOULDBLOCK ������ ���

            else // ������ ����/������������ �ƹ�ư ����
            {
                closesocket(m_Sock);
                return;
            }
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
                if (packet.ph.len == PACKET_HEADER_SIZE){ break; } // ������ ���� ����� �ִ� ��Ŷ�� ���(ex - PACKET_NAME_ACK)
                int iRecvBytes = recv(m_Sock, &packet.msg[iNumRecvByte], packet.ph.len - PACKET_HEADER_SIZE - iNumRecvByte, 0);
                iError = ErrorCheck(iRecvBytes);

                if (iError != 1) 
                {
                    if (iError == WSAEWOULDBLOCK) continue; // WSAEWOULDBLOCK ������ ���

                    else // ������ ����/������������ �ƹ�ư ����
                    {
                        closesocket(m_Sock);
                        return;
                    }
                }

                iNumRecvByte += iRecvBytes; 

            } while ((packet.ph.len - PACKET_HEADER_SIZE) > iNumRecvByte);

            m_RecvPacketList.push_back(packet);
            iTotalRecvBytes = 0;
            return; // �̰� �ʿ����� ����? ��� ������ ���ݾ�
        }
    }
}
