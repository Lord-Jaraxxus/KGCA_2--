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

	case WM_DESTROY:
		PostQuitMessage(0); // 메세지큐에 직접 WM_QUIT
		break;
	}

	// 메세지 내가 처리 불가 니가 대신 해줘.
	return  DefWindowProc(hWnd, message, wParam, lParam);
}

bool Sample::Init()
{
	m_Net.Init();
	using namespace std::placeholders;
	m_Net.m_Print.func = std::bind(&Sample::PRINT, this, _1);

	//m_Net.m_fnExecutePacket[PACKET_CHAR_MSG] = std::bind(&Sample::ChatMsg, this, _1);
	//m_Net.m_fnExecutePacket[PACKET_NAME_REQ] = std::bind(&Sample::NameReq, this, _1);

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
		Sleep(1);

		m_Net.RecvProcess();
		m_Net.PacketProcess();
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


// printf("%s %d %f",a,b,c); // 리스트 박스에 원하는 문자열을 출력하는 함수
void Sample::Print(const WCHAR* fmt, ...)      // 이해가 안가... 몰랑.. 그냥쓰자..
{
	va_list arg;
	va_start(arg, fmt);
	WCHAR msg[256] = L"";
	_vsntprintf(msg, 256, fmt, arg);
	SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)msg);
	va_end(arg);
}

void Sample::PRINT(std::list<std::wstring> myArguments)
{
	std::wstring buffer;
	for (auto data : myArguments)
	{
		buffer += data;
	}
	SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)buffer.c_str());
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