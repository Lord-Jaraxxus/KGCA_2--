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

	case WM_DESTROY:
		PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
		break;
	}

	// �޼��� ���� ó�� �Ұ� �ϰ� ��� ����.
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