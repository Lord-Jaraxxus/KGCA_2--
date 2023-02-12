#include "Sample.h"

LRESULT Sample::MsgProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	// ������, ��ư ����Ʈ ����â ���� ����� ��ȣ �ٿ��ְ� �ڵ� �޾��ְ� ��
	case WM_CREATE:
	{
		m_hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			500, 10, 200, 25, hWnd, (HMENU)1000, m_hInstance, NULL);

		m_hSendBtn = CreateWindow(L"button", L"Send", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			700, 10, 50, 25, hWnd, (HMENU)1001, m_hInstance, NULL);

		m_hListbox = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL,
			0, 0, 500, 600, hWnd, (HMENU)1002, m_hInstance, NULL);

		std::wstring fmt = L"%s";
		Print(fmt.c_str(), L"Chatting Start");
	}break;

	// ��Ʈ��ũ �޼����� ������ ��. �񵿱� ����Ʈ �κκκ�
	case NETWORK_MSG:
	{
		if (WSAGETSELECTERROR(lParam) != 0)
		{
			PostQuitMessage(0);
			break;
		}
		WORD dwSelect = WSAGETSELECTEVENT(lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:
		{
			m_Net.m_bConnect = true;
		}break;
		case FD_READ:
		{
			m_Net.RecvProcess();
		} break;
		case FD_WRITE:
		{
			//m_Net.SendPrecess();
		}break;
		case FD_CLOSE:
		{
			m_Net.m_bConnect = false;
		}break;
		}
	}break;

	// �Է½�, �ȿ� �� ���̽��� �޾Ƽ� ��ư�Է¸� ó������
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1001:
		{
			WCHAR szBuffer[255] = L"";
			GetWindowText(m_hEdit, szBuffer, 255);
			SetWindowText(m_hEdit, L""); // ��ư ������ �޽��� �ʱ�ȭ�ǰԲ�, �̰� �ǳ�? �ǳ�!
			OutputDebugString(szBuffer);

			if (m_bNameSend == false) // �� ó�� ���ӽ�, �г����� ����
			{
				std::string msg = to_wm(szBuffer);
				m_Net.AddSend(m_Net.m_Sock, msg.c_str(), msg.size() + PACKET_HEADER_SIZE, PACKET_NAME_REQ);
				m_bNameSend = true;
			}
			else					// �� �Ŀ��� ä����
			{
				std::string msg = to_wm(szBuffer);
				m_Net.AddSend(m_Net.m_Sock, msg.c_str(), msg.size() + PACKET_HEADER_SIZE, PACKET_CHAR_MSG);
			}
		}break;
		}
	}break;

	case WM_DESTROY:
		PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
		break;
	}

	// �޼��� ���� ó�� �Ұ� �ϰ� ��� ����.
	return  DefWindowProc(hWnd, message, wParam, lParam);
}

bool Sample::Init()
{
	m_fnExecutePacket[PACKET_NICKNAME_REQ] = std::bind(&Sample::ChatNameReq, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_CHAR_MSG] = std::bind(&Sample::ChatMsg, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_USER_JOIN] = std::bind(&Sample::UserJoin, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_NAME_ACK] = std::bind(&Sample::NameAck, this, std::placeholders::_1);

	std::wstring fmt = L"IP[%s]:PORT[%d] %s";
	if (m_Net.NetStart("192.168.0.46", 10000))
	{
		Print(fmt.c_str(), L"192.168.0.46", 10000, L"���� ����");
	}
	else
	{
		Print(fmt.c_str(), L"192.168.0.46", 10000, L"���� ����");
	}

	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	m_Net.Render();

	return true;
}

bool Sample::Release()
{
	m_Net.Release();

	return true;
}

bool Sample::Run()
{
	Init();
	while (m_bGameRun)
	{
		//Sleep(0);

		PacketProcess();
		if ( K_Window::Run() == true)
		{
			Frame();
			Render();
		}
		else
		{
			m_bGameRun = false;
		}
		m_Net.SendProcess();
	}
	Release();

	return true;
}

// ��Ŷ�� �м��ϴ�? �Լ��ε�? ->: �𹰤���
void Sample::NetRun()
{
	for (auto& packet : m_Net.m_RecvPacketList)
	{
		switch (packet.ph.type)
		{
		case PACKET_CHAR_MSG:
		{
			std::wstring fmt = L"%s";
			Print(fmt.c_str(), to_mw(packet.msg).c_str());
		}break;

		case PACKET_NICKNAME_REQ:
		{
			std::wstring fmt = L"%s";
			Print(fmt.c_str(), L"�̸��� �Է��Ͻÿ� : ");

		}break;

		case PACKET_USER_JOIN:
		{
			std::wstring fmt = L"%s%s";
			Print(fmt.c_str(), to_mw(packet.msg).c_str(), L"���� �����Ͽ����ϴ�.");
		}break;

		case PACKET_NAME_ACK:
		{
			std::wstring fmt = L"%s%s";
			Print(fmt.c_str(), to_mw(packet.msg).c_str(), L"�г��� ��� ����.");
		}break;

		}
	}
	m_Net.m_RecvPacketList.clear();
}

// printf("%s %d %f",a,b,c); // ����Ʈ �ڽ��� ���ϴ� ���ڿ��� ����ϴ� �Լ�
void Sample::Print(const WCHAR* fmt, ...)      // ���ذ� �Ȱ�... ����.. �׳ɾ���..
{
	va_list arg;
	va_start(arg, fmt);
	WCHAR msg[256] = L"";
	_vsntprintf(msg, 256, fmt, arg);
	SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)msg);
	va_end(arg);
}

// m_RecvPacketList�� ���� ��Ŷ�� �ݹ� �ý������� ó��
void Sample::PacketProcess()
{
	for (auto& packet : m_Net.m_RecvPacketList)
	{
		Sample::FunIter iter = m_fnExecutePacket.find(packet.ph.type);
		if (iter != m_fnExecutePacket.end())
		{
			Sample::CallFuction call = iter->second;
			call(packet);
		}
	}
	m_Net.m_RecvPacketList.clear();
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	Sample demo;
	demo.SetWindow(hInstance, L"WinChat", 800, 900);
	demo.Run();
	return 1;
}