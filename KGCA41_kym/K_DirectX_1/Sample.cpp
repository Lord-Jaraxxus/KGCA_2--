#include "Sample.h"

bool Sample::Init()
{
    if (FAILED(CreateVertexBuffer())) return false;
    if (FAILED(CreateShader())) return false;
    if (FAILED(CreateVertexLayout())) return false;
     
    return true;
}

bool Sample::Frame()
{
    return true;
}

bool Sample::Render()
{
    UINT stride = sizeof(SimpleVertex); // ���� 1���� ����Ʈ�뷮
    UINT offset = 0; // �������ۿ��� �������(����Ʈ)

    // ?�� ��������(StartSlot)�� ���ؽ����� ?��(NumBuffers)�� �ѱ�ž�, 
    // �� ������ �̷��� ������ �Ǿ��־�(stride), ��𼭺��� ������ �Ǵ���? ?������ ����(offset)
    m_pImmediateContext->IASetVertexBuffers(
        0, // StartSlot // , SLOT(�������͸�)
        1, // NumBuffers
        &m_pVertexBuffer, 
        &stride, 
        &offset
    ); // s�� ���� �͵��� �迭�� �ѱ� �� �ִ�
     
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout); // ��ǲ-����� �������� ����
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0); // ���ؽ� ���̴� ����, �ڿ� �ΰ��� �ϴ� ������� ������
    //m_pImmediateContext->HSSetShader(NULL, NULL, NULL); // ��
    //m_pImmediateContext->DSSetShader(NULL, NULL, NULL); // �� �� �ϴ� ���ص���, �Ǿ� ���� NULL�� �ּ� �н��Ҳ���
    //m_pImmediateContext->GSSetShader(NULL, NULL, NULL); // ��
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0); // �ȼ� ���̴� ����, ��� �ݵ�� �ؾ���. �׷��Ƿ� �ȼ� ���̴��� ���鷯������..
    m_pImmediateContext->Draw(3, 0);
    return true;
}

bool Sample::Release()
{   
    //if (m_pVertexBuffer) m_pVertexBuffer->Release();
    //if (m_pVertexLayout) m_pVertexLayout->Release();
    //if (m_pVS) m_pVS->Release();
    //if (m_pPS) m_pPS->Release();
    //if (m_pVSCode) m_pVSCode->Release();
    //if (m_pPSCode) m_pPSCode->Release(); 
    return true;
}

HRESULT Sample::CreateVertexBuffer()
{
    HRESULT hr;
    
    // NDC ��ǥ�� ����
    // x,y�� -1 ~ 1, z�� 0 ~ 1
    // v0       v1
    //
    // v2
    SimpleVertex vertices[] = 
    {
        -0.0f, 1.0f,  0.0f, //0.0f,0.0f,0.0f,0.0f,// v0
        +1.0f, 1.0f,  0.0f, //0.0f,0.0f,0.0f,0.0f,// v1
        -1.0f, -1.0f, 0.0f, //0.0f,0.0f,0.0f,0.0f,// v2
    };

    D3D11_BUFFER_DESC       bd; 
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = sizeof(SimpleVertex)*3; // ����Ʈ �뷮
    bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ������ �뵵. ����Ʈ�� GPU, �ް��ϸ� ����Ʈ ���ٻ���
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �� ���۰� ����
    //bd.CPUAccessFlags = 0; // cpu�� ���� ������ ����? �����Ӹ�
    //bd.MiscFlags = 0; // �⵿��� �÷���, �ϴ� 0
    //bd.StructureByteStride = 0; // ������ ���� ������ �����ϴ� 

    D3D11_SUBRESOURCE_DATA  sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertices;   // �ý��� �޸�, ��� ��
    //sd.SysMemPitch = 0;      // ����� ����
    //sd.SysMemSlicePitch = 0; // ���� ����

    //m_pVertexBuffer;

    hr = m_pd3dDevice->CreateBuffer
    (
        &bd, // ���� �Ҵ�
        &sd, // �ʱ� �Ҵ�� ���۸� ä��� CPU �޸� �ּ�
        &m_pVertexBuffer
    );

    return hr;
}

HRESULT Sample::CreateShader()
{
    HRESULT hr;

    // ���� ���̴� ������ �ʿ�

    // D3DCompileFromFile�� �ʿ��� �Ű�������
    //LPCWSTR pFileName = L"VertexShader.txt"; // ���� �̸�
    //D3D_SHADER_MACRO* pDefines = NULL; // ��ũ�� �Ⱦ�����
    //ID3DInclude* pInclude = NULL;  // ��Ŭ���, �Ⱦ�����
    //LPCSTR pEntrypoint = "VS"; // c����� ��Ʈ������Ʈ�� main, �����쿡���� WinMain, ���� ��쿡�� "VS" ������ �ɵ�
    //LPCSTR pTarget = "vs_5_0"; // Ÿ��, Ÿ���� �����Ϸ��� ����Ѵ�. ���ؽ� ���̴� 5.0 �����Ϸ��� �������ض���¶�
    //UINT Flags1 = 0;
    //UINT Flags2 = 0;
    //ID3DBlob* m_pVSCode = nullptr; // �������� ���. ����� ����, ��������� �׳� �����ϵ�, �׳� ū �����Ͷ�� �����ϸ� �ȴ�
    ID3DBlob* pErrorCode = nullptr; // �����ڵ�, �� �־���Ѵ�! ���̴� ���Ͽ��� ���� �߸������ �̰ɷ� �˾Ƴ��� �ϱ� ����

    // m_pVSCode���� ���ؽ����̴��� �������� ����� �� ��, 
    // �������ϸ� ������Ʈ ������ ����µ� m_pVSCode�� ������Ʈ ������ ���� �ּ�, ������Ʈ ������ ��ü ũ�� �̷��� ��ȯ�޴´�

    hr = D3DCompileFromFile(
        L"VertexShader.txt",
        NULL,
        NULL,
        "main",
        "vs_5_0",
        0,
        0,
        &m_pVSCode,
        &pErrorCode);

    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            pErrorCode->Release();
        }
        return hr;
    }


    // ���ؽ� ���̴� ���� �Լ��� �ʿ��� �Ű�������
    //void* pShaderBytecode = m_pVSCode->GetBufferPointer();
    //SIZE_T BytecodeLength1 = m_pVSCode->GetBufferSize(); // ���̶� ���ļ� 1����, ������ ǥ�ÿ��̿�
    //ID3D11ClassLinkage* pClassLinkage = NULL; // �ɼ��̶� ������� ��������
    //ID3D11VertexShader** ppVertexShader = &m_pVS;

    // ���ؽ� ���̴� ����, �������̶� �����Ӥ�����������������������������
    hr = m_pd3dDevice->CreateVertexShader(
        m_pVSCode->GetBufferPointer(),
        m_pVSCode->GetBufferSize(),
        NULL,
        &m_pVS);

    // �ȼ� ���̴� ������ 

 // D3DCompileFromFile�� �ʿ��� �Ű�������, ������ ���� ���� �����ϱ� T �ٿ�����
    //LPCWSTR pFileNameT = L"PixelShader.txt"; // ���� �̸�
    //D3D_SHADER_MACRO* pDefinesT = NULL; // ��ũ�� �Ⱦ�����
    //ID3DInclude* pIncludeT = NULL;  // ��Ŭ���, �Ⱦ�����
    //LPCSTR pEntrypointT = "VS"; // c����� ��Ʈ������Ʈ�� main, �����쿡���� WinMain, ���� ��쿡�� "PS" ������ �ɵ�
    //LPCSTR pTargetT = "vs_5_0"; // Ÿ��, Ÿ���� �����Ϸ��� ����Ѵ�. ���ؽ� ���̴� 5.0 �����Ϸ��� �������ض���¶�
    //UINT Flags1T = 0;
    //UINT Flags2T = 0;
    //ID3DBlob* m_pPSCode = nullptr; // �������� ���. ����� ����, ��������� �׳� �����ϵ�, �׳� ū �����Ͷ�� �����ϸ� �ȴ�
    //ID3DBlob* pErrorCodeT = nullptr; // �����ڵ�, �� �־���Ѵ�! ���̴� ���Ͽ��� ���� �߸������ �̰ɷ� �˾Ƴ��� �ϱ� ����

    // m_pVSCode���� ���ؽ����̴��� �������� ����� �� ��, 
    // �������ϸ� ������Ʈ ������ ����µ� m_pVSCode�� ������Ʈ ������ ���� �ּ�, ������Ʈ ������ ��ü ũ�� �̷��� ��ȯ�޴´�

    hr = D3DCompileFromFile(
        L"PixelShader.txt",
        NULL,
        NULL,
        "PSMain",
        "ps_5_0",
        0,
        0,
        &m_pPSCode,
        &pErrorCode);

    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            pErrorCode->Release();
        }
        return hr;
    }

    // �Ƚ� ���̴� ���� �Լ��� �ʿ��� �Ű�������, ���⵵ ������ �Ⱦ��ϱ� T �ٿ���
    //void* pShaderBytecodeT = m_pPSCode->GetBufferPointer();
    //SIZE_T BytecodeLengthT = m_pPSCode->GetBufferSize(); // ���̶� ���ļ� 1����, ������ ǥ�ÿ��̿�
    //ID3D11ClassLinkage* pClassLinkageT = NULL; // �ɼ��̶� ������� ��������
    //ID3D11VertexShader** ppVertexShaderT = &m_pVS;

    // �ȼ� ���̴� ����
    hr = m_pd3dDevice->CreatePixelShader(
        m_pPSCode->GetBufferPointer(),
        m_pPSCode->GetBufferSize(),
        NULL,
        &m_pPS);
    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            pErrorCode->Release();
        }
        return hr;
    }
    return hr;
}

HRESULT Sample::CreateVertexLayout()
{
    HRESULT hr;

    // ���� ���̾ƿ��� ���� ���̴��� ������ ������ �ִ�
    // ���� ���̾ƿ� ������ ������ ���� ���̴� ������ �ʿ��ϴ�
    D3D11_INPUT_ELEMENT_DESC ied[] = // ���� �̷������� ���������� ���� ����Ѵ�
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 9, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    //ied.SemanticName = "POSITION";
    //ied.SemanticIndex = 0;
    //ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //ied.InputSlot = 0;
    //ied.AlignedByteOffset = 0; // �̰� �� �߿��� �� ������..? 
    //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //ied.InstanceDataStepRate = 0;

    UINT NumElements = sizeof(ied) / sizeof(ied[0]); // ���� ���ҵ��� ����..�ε�?
    void* pShaderBytecodeWithInputSignature = m_pVSCode->GetBufferPointer(); // ���ؽ� ���̴��� ���ڰ��� ���İ�
    SIZE_T BytecodeLength = m_pVSCode->GetBufferSize(); // ���� ������ (������Ʈ ������ ũ���ε�?)
    //m_pVertexLayout;
     
    hr = m_pd3dDevice->CreateInputLayout(
        ied, // �迭�� �ּҴϱ� & ��������
        NumElements,
        m_pVSCode->GetBufferPointer(),
        m_pVSCode->GetBufferSize(),
        &m_pVertexLayout); 

    return hr;
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

GAME_RUN(CreateDevice_0, 1024, 768)