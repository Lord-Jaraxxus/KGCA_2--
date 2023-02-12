#include "Sample.h"

bool Sample::Init()
{
    K_Device::Init();

    return true;
}

bool Sample::Frame()
{
    K_Device::Frame();
    return true;
}

bool Sample::Render()
{
    K_Device::Render();
    return true;
}

bool Sample::Release()
{   
    K_Device::Release();
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

GAME_RUN(CreateDevice_0, 800, 600)