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
    // ���� Ÿ�� ����
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV); // ����°�� DSV �ڸ�

    // ���� Ÿ�� �ʱ�ȭ
    float color[4] = { 0.5f,0.5f,0.5f,1.0f };
    //float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRTV, color);

    // ���� ���ٽ� �� �ʱ�ȭ. 2�� ������ ���ٽ� �� �ʱ�ȭ����(������ �Ѵ�), 3�� ���� ���� �ʱ�ȭ, 4�� ���ٽ� ���� �ʱ�ȭ
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


// 1)����̽� �����
HRESULT K_Device::CreateDevice()
{
    HRESULT hr;

    // �ٷιؿ� �갡 �׷���ī����, nullptr�� ������ ���� ������ �׷���ī�带 ���ڴٴ� ��
    //IDXGIAdapter* pAdapter = nullptr; 
    //D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE; 
    //HMODULE Software = NULL;
    //UINT Flags = 0;
    D3D_FEATURE_LEVEL pFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT FeatureLevels = 1;
    //UINT SDKVersion = D3D11_SDK_VERSION; // ������ �������� ���� ������ �ֽŹ���(11)
    D3D_FEATURE_LEVEL pFeatureLevel; // ���� ���̷�Ʈ ���� ���? �׷������ε�

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT, // D2D�� ������ �÷��׸� �̰ɷ� �ؾ��Ѵ� 
        pFeatureLevels,
        FeatureLevels,
        D3D11_SDK_VERSION,
        &m_pd3dDevice,
        &pFeatureLevel,
        &m_pImmediateContext
    );

    return hr;
}

// 2) ���丮 ����
HRESULT K_Device::CreateDXGIDevice()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory);
    return hr;
}

// 3) ����ü�� ����
HRESULT K_Device::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    // �ʼ����� ��ҵ鸸 ä��
    sd.BufferCount = 1; 
    // ����� � ���鷡? : �ϳ�.
    sd.BufferDesc.Width = g_rtClient.right;
    sd.BufferDesc.Height = g_rtClient.bottom; //ũ���? : Ŭ���̾�Ʈ�� �Ȱ���.
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4����Ʈ �ȼ���

    // �ֻ���
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۸� ���� Ÿ�ٿ����� ����ڴ�
    sd.OutputWindow = g_hWnd; // �� �����쿡�� ����ϰڵ�

    // ���ø�, �ϴ� ����Ʈ������ ����
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    sd.Windowed = true; // â���� ��﷡, ��üȭ������ ��﷡
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

// 4) ����Ÿ�ٺ� ����
HRESULT K_Device::CreateRenderTargetView()
{
    HRESULT hr;
    ID3D11Texture2D* pBackBuffer = nullptr;
    // ����ü�ο��� 0�� ���۸� �����ͼ� ������� �ִ� ����ۿ� �ؽ��ķ� ��������
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    // ���� �� ���۸� ���� ����Ÿ������ ����� ���ڴ�
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRTV);
    // ���� ����Ÿ�ٺ�(��?��?) �Ѹ��� ��������� ����ü���� 0�����۰� ������ �ȴ�
    pBackBuffer->Release(); // ����Ÿ�ٺ並 ���� ������ �ݵ�� ������ ����ߴ�

    return hr;
}

// 5) ����Ʈ ����
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

    // ������� ũ�⸦ �����;� �ϴµ� ������ ���簡�� ����, �׷��� ����ü�ο��� ��������
    DXGI_SWAP_CHAIN_DESC scd;
    m_pSwapChain->GetDesc(&scd);

    // 1�� �ؽ�ó�� �����Ѵ�.
    ID3D11Texture2D*  pDSTexture;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = scd.BufferDesc.Width;
    td.Height = scd.BufferDesc.Height;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R24G8_TYPELESS; // Depth�� 24��Ʈ, Stencil�� 8��Ʈ
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL; // ���ε� �÷��װ� �߿�

    hr = m_pd3dDevice->CreateTexture2D(&td, NULL, &pDSTexture);


    // 2�� ���̽��ٽ� ��� �����Ѵ�.
    D3D11_DEPTH_STENCIL_VIEW_DESC dtvd;
    ZeroMemory(&dtvd, sizeof(dtvd));
    dtvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // UNORM�� ����ȭ ���ϰڴٶ�¶椾, ���ٽ��� UINT��������? ��´���?
    dtvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ������ 2����
    hr = m_pd3dDevice->CreateDepthStencilView(pDSTexture, &dtvd, &m_pDSV);


    // 3�� �� ����
    //m_pImmediateContext->OMSetRenderTargets(1, m_pRTV, m_pDSV); -> �̰� �� ������ ��ư �ű⼭ ����

    // 4�� ���̽��ٽ� �� ���� ��ü �����ؼ� ����
    
    return hr;
}


HRESULT K_Device::ResizeDevice(UINT width, UINT height)
{
    HRESULT hr;
    if (m_pd3dDevice == nullptr) return S_OK; // �����츦 ������ ���� VM_SIZE �޼����� ȣ��Ǳ� ����, �׶��� �갡 ������
    g_rtClient.right = width;
    g_rtClient.bottom = height;

    // ���� ������ ����Ÿ�� ���� �� �Ҹ�
    if(FAILED(hr = DeleteDXResource())) return hr; // ����Ÿ���̶� ����� ��� ���� �����ְ�
    m_pImmediateContext->OMSetRenderTargets(0, nullptr, NULL); 
    if (m_pRTV != nullptr) m_pRTV->Release();
    if (m_pDSV != nullptr) m_pDSV->Release();

    // ������� ũ�⸦ ����
    DXGI_SWAP_CHAIN_DESC CurrentSD, AfterSD;
    m_pSwapChain->GetDesc(&CurrentSD);
    hr = m_pSwapChain->ResizeBuffers(CurrentSD.BufferCount, width, height, CurrentSD.BufferDesc.Format, 0);

    // ����� ������� ũ�⸦ ��´�, �ٵ� �� �ڵ忡���� ���Ⱑ ����? ����? 
    // ���� �ʿ䰡 ������? �� ���� �����쿡�� VM_SIZE �˰������� �̹� ���༭�׸���
    m_pSwapChain->GetDesc(&AfterSD);
    GetClientRect(m_hWnd, &g_rtClient);

    // ����� �� ������ ũ�⸦ ��� ����Ÿ�� �並 �ٽ� ���� �� ���� -> CreateRenderTargetView() �ѹ��ϸ� �� ��
    if (FAILED(hr = CreateRenderTargetView())) return hr;
    // ���� ���ٽ� �䵵 RTV�� �Ȱ��� �ٽ� ���� �� ���� ���ش�
    if (FAILED(hr = CreateDepthStencilView())) return hr;
    // ����Ʈ �� ����
    CreateViewport();
    // ����� ��� �ٽ� �����
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


