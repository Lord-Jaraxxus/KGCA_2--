#include "Sample.h"

bool Sample::Init()
{
    K_Device::Init();

     //"crusader.sprite.combat.png"
    HRESULT hr = 
        DirectX::CreateWICTextureFromFile(
        m_pD3dDevice, 
        m_pImmediateContext,
        L"../../data/crusader.sprite.combat.png", 
        &m_pResource,
        &m_pTextureSRV);

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
    if (m_pResource) m_pResource->Release();
    if (m_pTextureSRV) m_pTextureSRV->Release();
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