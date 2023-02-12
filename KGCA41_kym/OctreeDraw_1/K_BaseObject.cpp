#include "K_BaseObject.h"

bool K_BaseObject::Init()
{
    if (FAILED(CreateVertexBuffer())) return false;
    if (FAILED(CreateShader())) return false;
    if (FAILED(CreateVertexLayout())) return false;

    return true;
}

bool K_BaseObject::Frame()
{
    
    // gpu update 함수
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0) ,0 ,0);
    return true;
}

bool K_BaseObject::Render()
{
    UINT stride = sizeof(SimpleVertex); // 정점 1개의 바이트용량
    UINT offset = 0; // 정점버퍼에서 출발지점(바이트)

    // ?번 레지스터(StartSlot)에 버텍스버퍼 ?개(NumBuffers)를 넘길거야, 
    // 그 정점은 이러케 구조가 되어있어(stride), 어디서부터 빼내면 되느냐? ?번부터 빼내(offset)
    m_pImmediateContext->IASetVertexBuffers(
        0, // StartSlot // , SLOT(레지스터리)
        1, // NumBuffers
        &m_pVertexBuffer,
        &stride,
        &offset
    ); // s가 붙은 것들은 배열로 넘길 수 있다

    m_pImmediateContext->IASetInputLayout(m_pVertexLayout); // 인풋-어셈블 스테이지 세팅
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0); // 버텍스 쉐이더 세팅, 뒤에 두개는 일단 사용하지 않을것
    //m_pImmediateContext->HSSetShader(NULL, NULL, NULL); // 얘
    //m_pImmediateContext->DSSetShader(NULL, NULL, NULL); // 내 은 일단 안해도됨, 맨앞 값도 NULL로 둬서 패스할꺼임
    //m_pImmediateContext->GSSetShader(NULL, NULL, NULL); // 셋
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0); // 픽셀 쉐이더 세팅, 얘는 반드시 해야함. 그러므로 픽셀 쉐이더를 만들러가보자..

    // 프리미티브는 점,선,면. 면(삼각형)이 디폴트
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 디폴트 (삼각형)
    //m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); // 점
    //m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // 선

    //m_pImmediateContext->Draw(2, 0);
    //m_pImmediateContext->Draw(2, 1); // 선 세개 긋기
    //m_pImmediateContext->Draw(2, 2);
    m_pImmediateContext->Draw(m_VertexList.size(), 0);
    return true;
}

bool K_BaseObject::Release()
{
    //if (m_pVertexBuffer) m_pVertexBuffer->Release();
    //if (m_pVertexLayout) m_pVertexLayout->Release();
    //if (m_pVS) m_pVS->Release();
    //if (m_pPS) m_pPS->Release();
    //if (m_pVSCode) m_pVSCode->Release();
    //if (m_pPSCode) m_pPSCode->Release(); 
    return true;
}

void K_BaseObject::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}

HRESULT K_BaseObject::CreateVertexBuffer()
{
    HRESULT hr;

    // NDC 좌표계 공간
    // x,y는 -1 ~ 1, z는 0 ~ 1
    // v0       v1,v4
    //
    // v2,v3    v5
    // 
    // 반드시 시계방향(앞면)으로 구성한다.
    m_VertexList.resize(6); 
    m_VertexList[0] = { {-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f} };
    m_VertexList[1] = { {+0.5f, 0.5f,  0.0f}, {0.0f, 1.0f, 0.0f, 0.0f} };
    m_VertexList[2] = { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f} };
    m_VertexList[3] = { {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 0.0f} };
    m_VertexList[4] = { {+0.5f, 0.5f,  0.0f}, {1.0f, 1.0f, 0.0f, 0.0f} };
    m_VertexList[5] = { {+0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 0.0f} };

    UINT NumVertex = m_VertexList.size();
    D3D11_BUFFER_DESC       bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = sizeof(SimpleVertex) * NumVertex; // 바이트 용량
    bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼의 용도. 디폴트가 GPU, 앵간하면 디폴트 쓴다생각
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이 버퍼가 뭐냐
    //bd.CPUAccessFlags = 0; // cpu가 접근 가능해 안해? 안해임마
    //bd.MiscFlags = 0; // 잡동사니 플래그, 일단 0
    //bd.StructureByteStride = 0; // 지금은 당장 쓸일이 없읍니다 

    D3D11_SUBRESOURCE_DATA  sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = &m_VertexList.at(0);   // 시스템 메모리, 얘는 씀
    //sd.SysMemPitch = 0;      // 얘들은 지금
    //sd.SysMemSlicePitch = 0; // 쓸모가 없다

    //m_pVertexBuffer;

    hr = m_pd3dDevice->CreateBuffer
    (
        &bd, // 버퍼 할당
        &sd, // 초기 할당된 버퍼를 채우는 CPU 메모리 주소
        &m_pVertexBuffer
    );

    return hr;
}

HRESULT K_BaseObject::CreateShader()
{
    HRESULT hr;

    // 정점 쉐이더 컴파일 필요

    // D3DCompileFromFile에 필요한 매개변수들
    //LPCWSTR pFileName = L"VertexShader.txt"; // 파일 이름
    //D3D_SHADER_MACRO* pDefines = NULL; // 매크로 안쓸꺼임
    //ID3DInclude* pInclude = NULL;  // 인클루드, 안쓸꺼임
    //LPCSTR pEntrypoint = "VS"; // c언어의 엔트리포인트는 main, 윈도우에서는 WinMain, 지금 경우에는 "VS" 넣으면 될듯
    //LPCSTR pTarget = "vs_5_0"; // 타겟, 타겟은 컴파일러를 얘기한다. 버텍스 쉐이더 5.0 컴파일러로 컴파일해라라라는뜻
    //UINT Flags1 = 0;
    //UINT Flags2 = 0;
    //ID3DBlob* m_pVSCode = nullptr; // 컴파일한 결과. 블랍이 뭐냐, 결과적으로 그냥 데이턴데, 그냥 큰 데이터라고 생각하면 된다
    ID3DBlob* pErrorCode = nullptr; // 에러코드, 꼭 있어야한다! 쉐이더 파일에서 뭐가 잘못됬는지 이걸로 알아내야 하기 때문

    // m_pVSCode에는 버텍스쉐이더가 컴파일한 결과가 들어갈 것, 
    // 컴파일하면 오브젝트 파일이 생기는데 m_pVSCode는 오브젝트 파일의 시작 주소, 오브젝트 파일의 전체 크기 이렇게 반환받는다

    hr = D3DCompileFromFile(
        L"VertexShader.txt",
        NULL,
        NULL,
        "VS",
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


    // 버텍스 쉐이더 생성 함수에 필요한 매개변수들
    //void* pShaderBytecode = m_pVSCode->GetBufferPointer();
    //SIZE_T BytecodeLength1 = m_pVSCode->GetBufferSize(); // 밑이랑 겹쳐서 1붙임, 어차피 표시용이여
    //ID3D11ClassLinkage* pClassLinkage = NULL; // 옵션이라 사용하지 않을것임
    //ID3D11VertexShader** ppVertexShader = &m_pVS;

    // 버텍스 쉐이더 생성, 컴파일이랑 별개임ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
    hr = m_pd3dDevice->CreateVertexShader(
        m_pVSCode->GetBufferPointer(),
        m_pVSCode->GetBufferSize(),
        NULL,
        &m_pVS);

    // 픽셀 쉐이더 컴파일 

 // D3DCompileFromFile에 필요한 매개변수들, 어차피 직접 쓰진 않으니까 T 붙여놓음
    //LPCWSTR pFileNameT = L"PixelShader.txt"; // 파일 이름
    //D3D_SHADER_MACRO* pDefinesT = NULL; // 매크로 안쓸꺼임
    //ID3DInclude* pIncludeT = NULL;  // 인클루드, 안쓸꺼임
    //LPCSTR pEntrypointT = "VS"; // c언어의 엔트리포인트는 main, 윈도우에서는 WinMain, 지금 경우에는 "PS" 넣으면 될듯
    //LPCSTR pTargetT = "vs_5_0"; // 타겟, 타겟은 컴파일러를 얘기한다. 버텍스 쉐이더 5.0 컴파일러로 컴파일해라라라는뜻
    //UINT Flags1T = 0;
    //UINT Flags2T = 0;
    //ID3DBlob* m_pPSCode = nullptr; // 컴파일한 결과. 블랍이 뭐냐, 결과적으로 그냥 데이턴데, 그냥 큰 데이터라고 생각하면 된다
    //ID3DBlob* pErrorCodeT = nullptr; // 에러코드, 꼭 있어야한다! 쉐이더 파일에서 뭐가 잘못됬는지 이걸로 알아내야 하기 때문

    // m_pVSCode에는 버텍스쉐이더가 컴파일한 결과가 들어갈 것, 
    // 컴파일하면 오브젝트 파일이 생기는데 m_pVSCode는 오브젝트 파일의 시작 주소, 오브젝트 파일의 전체 크기 이렇게 반환받는다

    hr = D3DCompileFromFile(
        L"PixelShader.txt",
        NULL,
        NULL,
        "PS",
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

    // 픽쉘 쉐이더 생성 함수에 필요한 매개변수들, 여기도 어차피 안쓰니까 T 붙여줌
    //void* pShaderBytecodeT = m_pPSCode->GetBufferPointer();
    //SIZE_T BytecodeLengthT = m_pPSCode->GetBufferSize(); // 밑이랑 겹쳐서 1붙임, 어차피 표시용이여
    //ID3D11ClassLinkage* pClassLinkageT = NULL; // 옵션이라 사용하지 않을것임
    //ID3D11VertexShader** ppVertexShaderT = &m_pVS;

    // 픽셀 쉐이더 생성
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

HRESULT K_BaseObject::CreateVertexLayout()
{
    HRESULT hr;

    // 정점 레이아웃은 정점 쉐이더랑 밀접한 관련이 있다
    // 정점 레이아웃 생성시 사전에 정점 쉐이더 생성이 필요하다
    D3D11_INPUT_ELEMENT_DESC ied[] = // 보통 이런식으로 열거형으로 많이 사용한다
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    //ied.SemanticName = "POSITION";
    //ied.SemanticIndex = 0;
    //ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //ied.InputSlot = 0;
    //ied.AlignedByteOffset = 0; // 이게 쫌 중요한 것 같은데..? 
    //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //ied.InstanceDataStepRate = 0;

    UINT NumElements = sizeof(ied) / sizeof(ied[0]); // 구성 원소들의 갯수..인듯?
    void* pShaderBytecodeWithInputSignature = m_pVSCode->GetBufferPointer(); // 버텍스 쉐이더의 인자값이 뭐냐고
    SIZE_T BytecodeLength = m_pVSCode->GetBufferSize(); // 버퍼 사이즈 (오브젝트 파일의 크기인듯?)
    //m_pVertexLayout;

    hr = m_pd3dDevice->CreateInputLayout(
        ied, // 배열의 주소니까 & 날려도됨
        NumElements,
        m_pVSCode->GetBufferPointer(),
        m_pVSCode->GetBufferSize(),
        &m_pVertexLayout);

    return hr;
}