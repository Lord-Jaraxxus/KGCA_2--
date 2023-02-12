#include "Sample.h"


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow) 
//{
//    Sample demo;
//    demo.SetWindow(hInstance);
//    demo.Run();
//    return 1;
//}  

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
	if (I_Input.GetKey('W') == KEY_HOLD || I_Input.GetKey('W') == KEY_HOLD)
	{
		m_Write.Draw(500, 500, L"W", { 1,1,1,1 });
	}
	if (I_Input.GetKey('A') == KEY_HOLD || I_Input.GetKey('A') == KEY_HOLD)
	{
		m_Write.Draw(400, 600, L"A", { 1,1,1,1 });
	}
	if (I_Input.GetKey('S') == KEY_HOLD || I_Input.GetKey('S') == KEY_HOLD)
	{
		m_Write.Draw(500, 600, L"S", { 1,1,1,1 });
	}
	if (I_Input.GetKey('D') == KEY_HOLD || I_Input.GetKey('D') == KEY_HOLD)
	{
		m_Write.Draw(600, 600, L"D", { 1,1,1,1 });
	}
	return true;
}

bool Sample::Release()
{

	return true; 
}



GAME_RUN(CreateObject, 1024, 768)