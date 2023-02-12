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
	m_object.Create(m_pd3dDevice, m_pImmediateContext, 
					L"../../data/img/wolfknight.sprite.attack_low.png",
					L"../../data/shader/DefaultShape.txt");
	return true; 
}

bool Sample::Frame()
{
	m_object.Frame();
	return true;
}

bool Sample::Render()
{
	m_object.Render();
	return true;
}

bool Sample::Release()
{
	m_object.Release();
	return true; 
}



GAME_RUN(CreateTextureObject, 1024, 768)