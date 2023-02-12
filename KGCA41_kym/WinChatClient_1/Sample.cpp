#include "Sample.h"

LRESULT Sample::MsgProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	
	// 생성시, 버튼 리스트 에딧창 세개 만들고 번호 붙여주고 핸들 달아주고 뭐
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

	// 입력시, 안에 또 케이스문 달아서 버튼입력만 처리해줌
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1001:
		{
			WCHAR szBuffer[255] = L"";
			GetWindowText(m_hEdit, szBuffer, 255);
			SetWindowText(m_hEdit, L""); // 버튼 누르면 메시지 초기화되게끔, 이거 되나? 되네!
			OutputDebugString(szBuffer);

			if (m_bNameSend == false) // 맨 처음 접속시, 닉네임을 보냄
			{
				m_Net.SendMsg(m_Net.m_Sock,
					to_wm(szBuffer).c_str(),
					PACKET_NAME_REQ);
				m_bNameSend = true;
			}
			else					// 그 후에는 채팅임
			{
				m_Net.SendMsg(m_Net.m_Sock,
					to_wm(szBuffer).c_str(),
					PACKET_CHAR_MSG);
			}
		}break;
		}
	}break;

	case WM_DESTROY:
		PostQuitMessage(0); // 메세지큐에 직접 WM_QUIT
		break;
	}

	// 메세지 내가 처리 불가 니가 대신 해줘.
	return  DefWindowProc(hWnd, message, wParam, lParam);
}

bool Sample::Init()
{
	std::wstring fmt = L"IP[%s]:PORT[%d] %s";
	if (m_Net.NetStart("192.168.0.46", 10000))
	{
		Print(fmt.c_str(), L"192.168.0.46", 10000, L"접속 성공");
	}
	else
	{
		Print(fmt.c_str(), L"192.168.0.46", 10000, L"접속 실패");
	}

	return true;
}

bool Sample::Frame()
{
	m_Net.Frame();
	NetRun();

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
		if ( K_Window::Run() == true)
		{
			Frame();
			Render();
		}
		else
		{
			m_bGameRun = false;
		}
	}
	Release();

	return true;
}

// 패킷을 분석하는? 함수인듯?
void Sample::NetRun()
{
	for (auto& packet : m_Net.m_PacketList)
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
			Print(fmt.c_str(), L"이름을 입력하시오 : ");

		}break;

		case PACKET_USER_JOIN:
		{
			std::wstring fmt = L"%s%s";
			Print(fmt.c_str(), to_mw(packet.msg).c_str(), L"님이 입장하였습니다.");
		}break;

		case PACKET_NAME_ACK:
		{
			std::wstring fmt = L"%s%s";
			Print(fmt.c_str(), to_mw(packet.msg).c_str(), L"닉네임 사용 승인.");
		}break;

		}
	}
	m_Net.m_PacketList.clear();
}

// printf("%s %d %f",a,b,c); // 리스트 박스에 원하는 문자열을 출력하는 함수
void   Sample::Print(const WCHAR* fmt, ...)      // 이해가 안가... 몰랑.. 그냥쓰자..
{
	va_list arg;
	va_start(arg, fmt);
	WCHAR msg[256] = L"";
	_vsntprintf(msg, 256, fmt, arg);
	SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)msg);
	va_end(arg);
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