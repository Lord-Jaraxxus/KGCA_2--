#include "K_GameCore.h"

bool K_GameCore::Init()
{
	return true;
}

bool K_GameCore::Frame()
{
	return true;
}

bool K_GameCore::Render()
{
	return true;
}

bool K_GameCore::Release()
{
	return true;
}

bool K_GameCore::K_GameCoreInit()
{
    HRESULT hr;
    if(K_Device::Init() != true) return false; 
    if (I_Timer.Init() != true) return false;
    if (I_Input.Init() != true) return false;
    if (I_Write.Init() != true) return false;
    I_Write.Set(m_pSwapChain);

    I_Shader.SetDevice(m_pd3dDevice, m_pImmediateContext);
    I_Shader.Load(L"DefaultShape.txt");
    I_Tex.SetDevice(m_pd3dDevice, m_pImmediateContext);
    K_DxState::SetState(m_pd3dDevice);
    if (Init() != true) return false;
	return true; 
}

bool K_GameCore::K_GameCoreFrame()
{
    if (K_Device::Frame() != true) return false;
    if (I_Timer.Frame() != true) return false;
    if (I_Input.Frame() != true) return false;
    if (I_Write.Frame() != true) return false;
    if (Frame() != true) return false;
	return true;
}

bool K_GameCore::K_GameCorePreRender()
{
    K_Device::PreRender();
    m_pImmediateContext->PSSetSamplers(0, 1, &K_DxState::g_pDefaultSS);
    return true;
}
  
bool K_GameCore::K_GameCoreRender()
{
    K_GameCorePreRender();

    if (Render() != true) return false;
    if (I_Timer.Render() != true) return false;
    if (I_Input.Render() != true) return false; 
    //if (I_Write.Render() != true) return false;
    I_Write.Draw(0, 0, I_Timer.m_szTimer, { 1,0,0,1 });
    I_Write.Draw(100, 400, I_Timer.m_szTimer, { 1,1,1,0.8 });
    K_GameCorePostRender();
	return true;
}

bool K_GameCore::K_GameCorePostRender()
{
    K_Device::PostRender();
    return true;
}

bool K_GameCore::K_GameCoreRelease()
{
    K_DxState::Release();
    if (Release() != true) return false;
    if (K_Device::Release() != true) return false;
    if (I_Timer.Release() != true) return false;
    if (I_Input.Release() != true) return false;
    if (I_Write.Release() != true) return false;
	return true;
}

bool K_GameCore::Run()
{
    if (K_GameCoreInit() == false) return false; // 이 Init은 밑에 샘플껏이 돌게될껐이다?
    //K_Window::Init();

    m_bGameRun = true;

    MSG msg = { 0, };

    // 기본 메시지 루프입니다:
    while (m_bGameRun)
    {
        // 윈도우가 모든 메세지를 처리했다면, 프레임 렌더를 돌리고 이상이 있으면 플래그를 false로 바꿔 게임을 종료해라
        if (K_Window::Run() == true) { if (!K_GameCoreFrame() || !K_GameCoreRender()) m_bGameRun = false; }
        else { m_bGameRun = false; } // 이건 윈도우 닫기창을 눌렀을 경우
    }

    K_GameCoreRelease();
}
