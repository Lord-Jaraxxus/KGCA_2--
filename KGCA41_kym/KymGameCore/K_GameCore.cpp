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
    I_Shader.Load(L"../../data/shader/DefaultShape.txt");
    I_Tex.SetDevice(m_pd3dDevice, m_pImmediateContext);
    K_DxState::SetState(m_pd3dDevice);

    std::wstring shaderfilename = L"../../data/shader/DefaultObject.txt";
    m_BG.Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject_Orgin.txt");
    m_RT.Create(m_pd3dDevice, m_pImmediateContext); //, 256, 256);

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
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
    m_pImmediateContext->RSSetState(K_DxState::g_pCurrentRS);
    m_pImmediateContext->OMSetDepthStencilState(K_DxState::g_pCurrentDSS, 0x01);
    m_pImmediateContext->OMSetBlendState(K_DxState::g_pCurrentBS, 0, -1);

    return true;
}
  
bool K_GameCore::K_GameCoreRender()
{
    K_GameCorePreRender();

    // 렌더 타겟 저장, 원래 렌더 타겟+a 를 RT의 멤버변수에 저장해둠
    m_RT.m_pOldRTV = m_pRTV;
    m_RT.m_pOldDSV = m_pDSV;
    m_RT.m_OldViewport = m_ViewPort;

    if (m_RT.Begin()) // 렌더타겟이랑 뷰포트를 RT가 가진 텍스처로 바까줌
    {
        if (Render() != true) return false;
        m_RT.End(); // 바꾼 렌더타겟이랑 뷰포트를 원래껄로 되돌림
    }

    if (m_RT.m_pSRV) { m_BG.m_pTextureSRV = m_RT.m_pSRV; } // 그려진 텍스처를 m_BG의 텍스처(map.jpg)랑 바꿔끼워줌

    K_GameCorePostRender();

	return true;
}

bool K_GameCore::K_GameCorePostRender()
{
    K_DxState::g_pCurrentRS = K_DxState::g_pDefaultRSSolid;
    m_pImmediateContext->RSSetState(K_DxState::g_pCurrentRS);
    K_DxState::g_pCurrentBS = K_DxState::g_pAlphaBlendDisable;
    m_pImmediateContext->OMSetBlendState(K_DxState::g_pCurrentBS, 0, -1);

    m_BG.SetMatrix(nullptr, nullptr, nullptr); // 여긴 얘기를 하다 마셨어
    m_BG.Render(); // 얘는 이제 백버퍼에 바로 그리는?듯?

    // 얘들은 뭐.. 사실 어따넣으나 상관없긴 한데 일단 저 텍스처랑은 별개로 그려줌
    // 상관없긴 개뿔ㅋㅋ 위에서 먼저뿌리니까 가려져서 보이지도 않쥬? 무조건 여기서 해줘야하쥬?
    if (I_Timer.Render() != true) return false;
    //if (I_Input.Render() != true) return false;
    //if (I_Write.Render() != true) return false;
    //I_Write.Draw(0, 0, I_Timer.m_szTimer, { 1,0,0,1 });
    I_Write.Draw(600, 0, I_Input.m_szCursorPos, { 1,0,0,1 });

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

HRESULT K_GameCore::CreateDXResource()
{
    HRESULT hr;
    if (I_Write.m_pBackBuffer == nullptr) return S_OK; // 윈도우 켜질때 호출되는 VM_SIZE 대비

    I_Write.Set(m_pSwapChain); // 변경된 스왑체인을 먹여서 렌더타겟을 다시 맹글어줌, 겸사겸사 m_pTextColor도
    if (FAILED(hr = I_Write.CreateDXResource())) return hr; // ??그럼 얘는 필요가 없겠는데??
    return S_OK;
}

HRESULT K_GameCore::DeleteDXResource()
{
    HRESULT hr;
    if (I_Write.m_pBackBuffer == nullptr) return S_OK; // 윈도우 켜질때 호출되는 VM_SIZE 대비

    if(FAILED(hr = I_Write.DeleteDXResource())) return hr;
    return S_OK;
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

bool K_GameCore::ToolRun()
{
     if (!K_GameCoreFrame() || !K_GameCoreRender())
        {
            m_bGameRun = false;
            return false;
        }

     return true;
}
