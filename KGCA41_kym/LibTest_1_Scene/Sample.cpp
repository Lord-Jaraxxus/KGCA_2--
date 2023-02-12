#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	m_pScene = new K_Scene;
	m_pScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pScene->Init();
	return true; 
}

bool Sample::Frame()
{
	m_pScene->Frame();
	return true;
}

bool Sample::Render()
{
	m_pScene->Render();
	return true;
}

bool Sample::Release()
{
	m_pScene->Release();
	delete m_pScene;
	return true; 
}


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

GAME_RUN(CreateObject, 800, 900)
