 #include "K_Device.h"

bool K_Device::Init()
{
    HRESULT hr;
    if (FAILED(hr = CreateDevice())) return false;
    if (FAILED(hr = CreateDXGIDevice())) return false;
    if (FAILED(hr = CreateSwapChain())) return false;
    if (FAILED(hr = CreateRenderTargetView())) return false;
    if (FAILED(hr = CreateDepthStencilView())) return false;
    CreateViewport();

    return true;
}

bool K_Device::Frame()
{
    return true;
}

bool K_Device::PreRender()
{
    // 렌더 타겟 지정
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV); // 세번째가 DSV 자리

    // 렌더 타겟 초기화
    float color[4] = { 0.5f,0.5f,0.5f,1.0f };
    //float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);

    // 뎁스 스텐실 뷰 초기화. 2는 뎁스랑 스텐실 뭐 초기화할지(지금은 둘다), 3은 뎁스 뭘로 초기화, 4는 스텐실 뭘로 초기화
    m_pImmediateContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    return true;
}

bool K_Device::Render()
{
    PreRender();
    PostRender();
    return true;
}

bool K_Device::PostRender()
{
    m_pSwapChain->Present(0, 0);
    return true;
}

bool K_Device::Release()
{
    if (m_pd3dDevice) m_pd3dDevice->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pGIFactory) m_pGIFactory->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pRTV) m_pRTV->Release();
    return true;
}


// 1)디바이스 만들기
HRESULT K_Device::CreateDevice()
{
    HRESULT hr;

    // 바로밑에 얘가 그래픽카드임, nullptr로 놓으면 현재 설정된 그래픽카드를 쓰겠다는 뜼
    //IDXGIAdapter* pAdapter = nullptr; 
    //D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE; 
    //HMODULE Software = NULL;
    //UINT Flags = 0;
    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT FeatureLevels = 1;
    //UINT SDKVersion = D3D11_SDK_VERSION; // 묻지도 따지지도 말고 무조건 최신버전(11)
    D3D_FEATURE_LEVEL pFeatureLevel; // 대충 다이렉트 버전 몇쓸꺼? 그런느낌인듯

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT, // D2D를 쓰려면 플래그를 이걸로 해야한다 
        pFeatureLevels,
        FeatureLevels,
        D3D11_SDK_VERSION,
        &m_pd3dDevice,
        &pFeatureLevel,
        &m_pImmediateContext
    );

    return hr;
}

// 2) 팩토리 생성
HRESULT K_Device::CreateDXGIDevice()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory);
    return hr;
}

// 3) 스왑체인 생성
HRESULT K_Device::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    // 필수적인 요소들만 채움
    sd.BufferCount = 1; 
    // 백버퍼 몇개 만들래? : 하나.
    sd.BufferDesc.Width = g_rtClient.right;
    sd.BufferDesc.Height = g_rtClient.bottom; //크기는? : 클라이언트랑 똑같이.
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4바이트 픽셀로

    // 주사율
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼를 렌더 타겟용으로 만들겠다
    sd.OutputWindow = g_hWnd; // 요 윈도우에게 출력하겠따

    // 샘플링, 일단 디폴트값으로 지정
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    sd.Windowed = true; // 창모드로 띄울래, 전체화면으로 띄울래
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    //DXGI_MODE_DESC BufferDesc;
    //DXGI_SAMPLE_DESC SampleDesc;
    //DXGI_USAGE BufferUsage;
    //UINT BufferCount;
    //HWND OutputWindow;
    //BOOL Windowed;
    //DXGI_SWAP_EFFECT SwapEffect;
    //UINT Flags;

    return m_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
}

// 4) 렌더타겟뷰 생성
HRESULT K_Device::CreateRenderTargetView()
{
    HRESULT hr;
    ID3D11Texture2D* pBackBuffer = nullptr;
    // 스왑체인에서 0번 버퍼를 가져와서 만들어져 있는 백버퍼에 텍스쳐로 리턴해줘
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    // 나는 이 버퍼를 갖고 렌더타겟으로 만들어 쓰겠다
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRTV);
    // 이제 렌더타겟뷰(에?를?) 뿌리면 결과적으로 스왑체인의 0번버퍼가 렌더가 된다
    pBackBuffer->Release(); // 렌더타겟뷰를 만든 다음엔 반드시 릴리즈 해줘야댐

    return hr;
}

// 5) 뷰포트 설정
void K_Device::CreateViewport()
{
    m_ViewPort.Width = g_rtClient.right;
    m_ViewPort.Height = g_rtClient.bottom;
    //m_ViewPort.Width = m_rtWindow.right - m_rtWindow.left;
    //m_ViewPort.Height = m_rtWindow.top - m_rtWindow.bottom;

    m_ViewPort.TopLeftX = 0;
    m_ViewPort.TopLeftY = 0;


    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
}

HRESULT K_Device::CreateDepthStencilView()
{
    HRESULT hr;

    //D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    //m_pRTV->GetDesc(&rtvd);

    // 백버퍼의 크기를 가져와야 하는데 위에건 영양가가 없다, 그래서 스왑체인에서 가져와줌
    DXGI_SWAP_CHAIN_DESC scd;
    m_pSwapChain->GetDesc(&scd);

    // 1번 텍스처를 생성한다.
    ID3D11Texture2D*  pDSTexture;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = scd.BufferDesc.Width;
    td.Height = scd.BufferDesc.Height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R24G8_TYPELESS; // Depth가 24비트, Stencil이 8비트
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 바인드 플래그가 중요

    hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pDSTexture);


    // 2번 깊이스텐실 뷰로 생성한다.
    D3D11_DEPTH_STENCIL_VIEW_DESC dtvd;
    ZeroMemory(&dtvd, sizeof(dtvd));
    dtvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // UNORM은 정규화 안하겠다라는뜻ㅎ, 스텐실은 UINT형식으로? 라는느낌?
    dtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // 무조건 2차원
    hr = m_pd3dDevice->CreateDepthStencilView(pDSTexture, &dtvd, &m_pDSV);


    // 3번 뷰 적용
    //m_pImmediateContext->OMSetRenderTargets(1, m_pRTV, m_pDSV); -> 이건 그 어디더라 암튼 거기서 해줌

    // 4번 깊이스텐실 뷰 상태 객체 생성해서 적용
    
    return hr;
}


HRESULT K_Device::ResizeDevice(UINT width, UINT height)
{
    HRESULT hr;
    if (m_pd3dDevice == nullptr) return S_OK; // 윈도우를 생성할 때도 VM_SIZE 메세지가 호출되기 때문, 그때는 얘가 없응꼐
    g_rtClient.right = width;
    g_rtClient.bottom = height;

    // 현재 설정된 랜더타겟 해제 및 소멸
    if(FAILED(hr = DeleteDXResource())) return hr; // 렌더타겟이랑 연결된 놈들 먼저 지워주고
    m_pImmediateContext->OMSetRenderTargets(0, nullptr, NULL); 
    if (m_pRTV != nullptr) m_pRTV->Release();
    if (m_pDSV != nullptr) m_pDSV->Release();

    // 백버퍼의 크기를 조정
    DXGI_SWAP_CHAIN_DESC CurrentSD, AfterSD;
    m_pSwapChain->GetDesc(&CurrentSD);
    hr = m_pSwapChain->ResizeBuffers(CurrentSD.BufferCount, width, height, CurrentSD.BufferDesc.Format, 0);

    // 변경된 백버퍼의 크기를 얻는다, 근데 쌤 코드에서는 여기가 없네? 머임? 
    // 딱히 필요가 없었나? 아 위에 윈도우에서 VM_SIZE 검거했을때 이미 해줘서그릉가
    m_pSwapChain->GetDesc(&AfterSD);
    GetClientRect(m_hWnd, &g_rtClient);

    // 변경된 백 버퍼의 크기를 얻고 렌더타켓 뷰를 다시 생성 및 적용 -> CreateRenderTargetView() 한번하면 끝 ㅎ
    if (FAILED(hr = CreateRenderTargetView())) return hr;
    // 뎁스 스텐실 뷰도 RTV랑 똑같이 다시 생성 및 적용 해준다
    if (FAILED(hr = CreateDepthStencilView())) return hr;
    // 뷰포트 재 지정
    CreateViewport();
    // 연결된 놈들 다시 살려줌
    if (FAILED(hr = CreateDXResource())) return hr;
    
    return S_OK;
}

HRESULT K_Device::CreateDXResource()
{
    return S_OK;
}

HRESULT K_Device::DeleteDXResource()
{
    return S_OK;
}


