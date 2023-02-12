#include "K_BaseObject.h"
#include "K_TextureManager.h"
#include "K_ShaderManager.h"

bool K_BaseObject::Init()
{
    return true;
}

bool K_BaseObject::Frame()
{
    //m_VertexList[0] = { {-1.0f, 1.0f, 0.0f}, };
    //m_VertexList[1] = { {+1.0f, 1.0f, 0.0f}, };
    //m_VertexList[2] = { {-1.0f, -1.0f, 0.0f}, };
    //m_VertexList[3] = { {-1.0f, -1.0f, 0.0f}, };
    //m_VertexList[4] = { {+1.0f, 1.0f, 0.0f}, };
    //m_VertexList[5] = { {+1.0f, -1.0f, 0.0f}, };
    // 몬가.. 몬가.. 모몬가임... 텍스쳐 좌표에 대한 공부가 좀 필요할 듯 하다

    //m_VertexList[0].t = { 1.0f, 0.0f };
    //m_VertexList[1].t = { 0.0f, 0.0f };
    //m_VertexList[4].t = { 0.0f, 0.0f }; 

    //m_VertexList[5].t = { 0.0f, 1.0f };
    //m_VertexList[2].t = { 1.0f, 1.0f };
    //m_VertexList[3].t = { 1.0f, 1.0f };
    // 텍스처 좌표를 뒤집어서 그림을 좌우반전 시킨거, 근데 대체 뭔지 아직 잘 모르겠따;

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
    m_pImmediateContext->VSSetShader(m_pShader->m_pVS, NULL, 0); // 버텍스 쉐이더 세팅, 뒤에 두개는 일단 사용하지 않을것
    m_pImmediateContext->PSSetShader(m_pShader->m_pPS, NULL, 0); // 픽셀 쉐이더 세팅, 얘는 반드시 해야함. 그러므로 픽셀 쉐이더를 만들러가보자..

    // 프리미티브는 점,선,면. 면(삼각형)이 디폴트
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 디폴트 (삼각형)

    // PSSetShaderResources 함수에 필요한 매개변수들
    UINT StartSlot = 0;
    UINT NumViews = 1;
    ID3D11ShaderResourceView* const* ppShaderResourceViews = &m_pTexture->m_pTextureSRV;

    // 텍스처를 쉐이더리소스뷰에 담아? 쉐이더리소스뷰를 통해? 아무튼 그렇게 파이프라인에 넘겨줌
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV);

    m_pImmediateContext->Draw(m_VertexList.size(), 0);

    return true;
}

bool K_BaseObject::Release()
{
    if (m_pTexture) m_pTexture->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pShader) m_pShader->Release();

    return true;
}

bool K_BaseObject::Create(ID3D11Device* pd3dDevice,
                          ID3D11DeviceContext* pContext,
                          const wchar_t* textureName,
                          const wchar_t* shaderName)
{
    SetDevice(pd3dDevice, pContext);

    if (FAILED(CreateVertexBuffer())) return false;
    if (CreateShader(shaderName) != true) return false;
    if (FAILED(CreateVertexLayout())) return false;
    if (LoadTexture(textureName) != true) return false;
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
    m_VertexList[0].p = { -0.5f, 0.5f, 0.0f };
    m_VertexList[0].c = { 1.0f, 0.0f, 0.0f, 0.0f };
    m_VertexList[0].t = { 0.0f, 0.0f };

    m_VertexList[1].p = { +0.5f, 0.5f,  0.0f };
    m_VertexList[1].c = { 1.0f, 0.0f, 0.0f, 0.0f };
    m_VertexList[1].t = { 1.0f, 0.0f };

    m_VertexList[2].p = { -0.5f, -0.5f, 0.0f };
    m_VertexList[2].c = { 1.0f, 0.0f, 0.0f, 0.0f };
    m_VertexList[2].t = { 0.0f, 1.0f };

    m_VertexList[3].p = m_VertexList[2].p;
    m_VertexList[3].c = m_VertexList[2].c;
    m_VertexList[3].t = m_VertexList[2].t;

    m_VertexList[4].p = m_VertexList[1].p;
    m_VertexList[4].c = m_VertexList[1].c;
    m_VertexList[4].t = m_VertexList[1].t;

    m_VertexList[5].p = { +0.5f, -0.5f, 0.0f };
    m_VertexList[5].c = { 1.0f, 0.0f, 0.0f, 0.0f };
    m_VertexList[5].t = { 1.0f, 1.0f };

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

bool K_BaseObject::CreateShader(std::wstring filename)
{
    
    m_pShader = I_Shader.Load(filename);
    if (m_pShader) return true;

    return false;
}

HRESULT K_BaseObject::CreateVertexLayout()
{
    HRESULT hr;

    // 정점 레이아웃은 정점 쉐이더랑 밀접한 관련이 있다
    // 정점 레이아웃 생성시 사전에 정점 쉐이더 생성이 필요하다
    D3D11_INPUT_ELEMENT_DESC ied[] = // 보통 이런식으로 열거형으로 많이 사용한다
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    //ied.SemanticName = "POSITION";
    //ied.SemanticIndex = 0;
    //ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //ied.InputSlot = 0;
    //ied.AlignedByteOffset = 0; // 이게 쫌 중요한 것 같은데..? 
    //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //ied.InstanceDataStepRate = 0;

    UINT NumElements = sizeof(ied) / sizeof(ied[0]); // 구성 원소들의 갯수..인듯?
    void* pShaderBytecodeWithInputSignature = m_pShader->m_pVSCode->GetBufferPointer(); // 버텍스 쉐이더의 인자값이 뭐냐고
    SIZE_T BytecodeLength = m_pShader->m_pVSCode->GetBufferSize(); // 버퍼 사이즈 (오브젝트 파일의 크기인듯?)
    //m_pVertexLayout;

    hr = m_pd3dDevice->CreateInputLayout(
        ied, // 배열의 주소니까 & 날려도됨
        NumElements,
        m_pShader->m_pVSCode->GetBufferPointer(),
        m_pShader->m_pVSCode->GetBufferSize(),
        &m_pVertexLayout);

    return hr;
}

bool K_BaseObject::LoadTexture(std::wstring filename)
{
   
    m_pTexture = I_Tex.Load(filename);
    if (m_pTexture != nullptr) return true;

    return false;
}
