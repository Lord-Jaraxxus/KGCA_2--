#include "Sample.h"

LRESULT Sample::MsgProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
		break;
	}
	// �޼��� ���� ó�� �Ұ� �ϰ� ��� ����.
	return  DefWindowProc(hWnd, message, wParam, lParam);
}

bool Sample::Init()
{
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{

	return true;
}

bool Sample::Release()
{
	return true;
}

bool Sample::Run()
{
	Init();
	while (m_bGameRun)
	{
		if (
			K_Window::Run() == true)
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

//int main() { return 0; };