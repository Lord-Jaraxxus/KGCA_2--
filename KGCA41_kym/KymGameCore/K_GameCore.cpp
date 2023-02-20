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

    // ���� Ÿ�� ����, ���� ���� Ÿ��+a �� RT�� ��������� �����ص�
    m_RT.m_pOldRTV = m_pRTV;
    m_RT.m_pOldDSV = m_pDSV;
    m_RT.m_OldViewport = m_ViewPort;

    if (m_RT.Begin()) // ����Ÿ���̶� ����Ʈ�� RT�� ���� �ؽ�ó�� �ٱ���
    {
        if (Render() != true) return false;
        m_RT.End(); // �ٲ� ����Ÿ���̶� ����Ʈ�� �������� �ǵ���
    }

    if (m_RT.m_pSRV) { m_BG.m_pTextureSRV = m_RT.m_pSRV; } // �׷��� �ؽ�ó�� m_BG�� �ؽ�ó(map.jpg)�� �ٲ㳢����

    K_GameCorePostRender();

	return true;
}

bool K_GameCore::K_GameCorePostRender()
{
    K_DxState::g_pCurrentRS = K_DxState::g_pDefaultRSSolid;
    m_pImmediateContext->RSSetState(K_DxState::g_pCurrentRS);
    K_DxState::g_pCurrentBS = K_DxState::g_pAlphaBlendDisable;
    m_pImmediateContext->OMSetBlendState(K_DxState::g_pCurrentBS, 0, -1);

    m_BG.SetMatrix(nullptr, nullptr, nullptr); // ���� ��⸦ �ϴ� ���̾�
    m_BG.Render(); // ��� ���� ����ۿ� �ٷ� �׸���?��?

    // ����� ��.. ��� ��������� ������� �ѵ� �ϴ� �� �ؽ�ó���� ������ �׷���
    // ������� ���Ԥ��� ������ �����Ѹ��ϱ� �������� �������� ����? ������ ���⼭ ���������?
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
    if (I_Write.m_pBackBuffer == nullptr) return S_OK; // ������ ������ ȣ��Ǵ� VM_SIZE ���

    I_Write.Set(m_pSwapChain); // ����� ����ü���� �Կ��� ����Ÿ���� �ٽ� �ͱ۾���, ����� m_pTextColor��
    if (FAILED(hr = I_Write.CreateDXResource())) return hr; // ??�׷� ��� �ʿ䰡 ���ڴµ�??
    return S_OK;
}

HRESULT K_GameCore::DeleteDXResource()
{
    HRESULT hr;
    if (I_Write.m_pBackBuffer == nullptr) return S_OK; // ������ ������ ȣ��Ǵ� VM_SIZE ���

    if(FAILED(hr = I_Write.DeleteDXResource())) return hr;
    return S_OK;
}

bool K_GameCore::Run()
{
    if (K_GameCoreInit() == false) return false; // �� Init�� �ؿ� ���ò��� ���Եɲ��̴�?
    //K_Window::Init();

    m_bGameRun = true;

    MSG msg = { 0, };

    // �⺻ �޽��� �����Դϴ�:
    while (m_bGameRun)
    {
        // �����찡 ��� �޼����� ó���ߴٸ�, ������ ������ ������ �̻��� ������ �÷��׸� false�� �ٲ� ������ �����ض�
        if (K_Window::Run() == true) { if (!K_GameCoreFrame() || !K_GameCoreRender()) m_bGameRun = false; }
        else { m_bGameRun = false; } // �̰� ������ �ݱ�â�� ������ ���
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
