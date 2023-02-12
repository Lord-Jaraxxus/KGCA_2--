#include "K_BaseObject.h"

static float frame_test = 0;
static bool test_flag = true;

namespace K_DX
{
    ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void* pDataAddress, UINT iNumVertex, UINT iVertexSize)
    {
        HRESULT hr;
        ID3D11Buffer* pVB = nullptr;
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = iVertexSize * iNumVertex; // 바이트 용량
        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = pDataAddress;
        hr = pd3dDevice->CreateBuffer(
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
            &pVB);
        return pVB;
    }

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

        //m_VertexList[0] = { {frame_test, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f} };
        //if (frame_test <= -1.0f) { test_flag = false; }
        //else if (frame_test >= 0.0f) { test_flag = true; }

        //if (test_flag == true) frame_test -= 0.001f;
        //else frame_test += 0.001f;

        // gpu update 함수
        //m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0) ,0 ,0);
        return true;
    }

    bool K_BaseObject::PreRender()
    {
        UINT stride = sizeof(PNCT_VERTEX); // 정점 1개의 바이트용량
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
        m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0); // 인덱스 버퍼 세팅
        m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer); // 콘스탄트 버퍼 세팅, 2D에선 필요없잖아

        m_pImmediateContext->IASetInputLayout(m_pVertexLayout); // 인풋-어셈블 스테이지 세팅
        m_pImmediateContext->VSSetShader(m_pShader->m_pVS, NULL, 0); // 버텍스 쉐이더 세팅, 뒤에 두개는 일단 사용하지 않을것
        m_pImmediateContext->PSSetShader(m_pShader->m_pPS, NULL, 0); // 픽셀 쉐이더 세팅, 얘는 반드시 해야함. 그러므로 픽셀 쉐이더를 만들러가보자..

        // 프리미티브는 점,선,면. 면(삼각형)이 디폴트
        //m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 디폴트 (삼각형)

        // PSSetShaderResources 함수에 필요한 매개변수들
        //UINT StartSlot = 0;
        //UINT NumViews = 1;
        //ID3D11ShaderResourceView* const* ppShaderResourceViews = &m_pTexture->m_pTextureSRV;

        //// 텍스처를 쉐이더리소스뷰에 담아? 쉐이더리소스뷰를 통해? 아무튼 그렇게 파이프라인에 넘겨줌
        //ID3D11ShaderResourceView* srv = m_pTexture->GetSRV();
        //m_pImmediateContext->PSSetShaderResources(0, 1, &srv);
        m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);

        return true;
    }

    bool K_BaseObject::Render()
    {
        PreRender();
        PostRender();

        return true;
    }

    bool K_BaseObject::PostRender()
    {
        if (m_pIndexBuffer == nullptr)
            m_pImmediateContext->Draw(m_VertexList.size(), 0);
        else
            m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
            //m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0); // 이러케 바꿔야하남..?

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
        std::wstring textureName,
        std::wstring shaderName)
    {
        SetDevice(pd3dDevice, pContext);
        m_szTextureName = textureName;
        m_szShaderName = shaderName;

        if (FAILED(CreateConstantBuffer())) return false;
        if (FAILED(CreateVertexBuffer())) return false;
        //if (FAILED(CreateIndexBuffer())) return false;
        if (CreateShader(shaderName) != true) return false;
        if (FAILED(CreateVertexLayout())) return false;
        if (LoadTexture(textureName) != true) return false;
        if (m_pTexture != nullptr) m_pTextureSRV = m_pTexture->m_pTextureSRV;

        return true;
    }

    void K_BaseObject::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
    {
        m_pd3dDevice = pd3dDevice;
        m_pImmediateContext = pContext;
    }

    void K_BaseObject::CreateVertexData()
    {
        if (m_VertexList.size() > 0)
        {
            m_InitVertexList = m_VertexList;
            return;
        }

        //m_VertexList.resize(4);
        //m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };
        //m_VertexList[1].p = { +1.0f, 1.0f,  0.0f };
        //m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };
        //m_VertexList[3].p = { 1.0f, -1.0f, 0.0f };

        //m_VertexList[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        //m_VertexList[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        //m_VertexList[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        //m_VertexList[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };

        //m_VertexList[0].t = { 0.0f, 0.0f };
        //m_VertexList[1].t = { 1.0f, 0.0f };
        //m_VertexList[2].t = { 0.0f, 1.0f };
        //m_VertexList[3].t = { 1.0f, 1.0f };

        m_VertexList.resize(6);
        m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };
        m_VertexList[1].p = { +1.0f, 1.0f,  0.0f };
        m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };
        m_VertexList[3].p = m_VertexList[2].p;
        m_VertexList[4].p = m_VertexList[1].p;
        m_VertexList[5].p = { 1.0f, -1.0f, 0.0f };

        m_VertexList[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[4].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[5].c = { 1.0f, 1.0f, 1.0f, 1.0f };

        m_VertexList[0].t = { 0.0f, 0.0f };
        m_VertexList[1].t = { 1.0f, 0.0f };
        m_VertexList[2].t = { 0.0f, 1.0f };
        m_VertexList[3].t = m_VertexList[2].t;
        m_VertexList[4].t = m_VertexList[1].t;
        m_VertexList[5].t = { 1.0f, 1.0f };

        m_InitVertexList = m_VertexList;
    }

    void K_BaseObject::CreateIndexData()
    {
        if (m_IndexList.size() > 0)return;

        // 정점버퍼에 인덱스
        m_IndexList.resize(6);
        m_IndexList[0] = 0;
        m_IndexList[1] = 1;
        m_IndexList[2] = 2;
        m_IndexList[3] = 2;
        m_IndexList[4] = 1;
        m_IndexList[5] = 3;

        m_dwFace = m_IndexList.size() / 3;
    }

    void K_BaseObject::CreateConstantData()
    {
        m_cbData.matWorld.Identity();
        m_cbData.matView.Identity();
        m_cbData.matProj.Identity();
        m_cbData.fTimer = 0.0f;
        m_cbData.matWorld.Transpose();
        m_cbData.matView.Transpose();
        m_cbData.matProj.Transpose();
    }

    HRESULT K_BaseObject::CreateVertexBuffer()
    {
        HRESULT hr;

        CreateVertexData();

        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size(); // 바이트 용량

        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼의 용도. 디폴트가 GPU, 앵간하면 디폴트 쓴다생각
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이 버퍼가 뭐냐

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_VertexList.at(0);   // 시스템 메모리, 얘는 씀

        hr = m_pd3dDevice->CreateBuffer
        (
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 채우는 CPU 메모리 주소
            &m_pVertexBuffer
        );

        return hr;
    }

    HRESULT K_BaseObject::CreateIndexBuffer()
    {
        HRESULT hr;

        CreateIndexData();

        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(DWORD) * m_IndexList.size(); // 바이트 용량
        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_IndexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
            &m_pIndexBuffer);
        return hr;
    }

    HRESULT K_BaseObject::CreateConstantBuffer()
    {
        HRESULT hr;
        CreateConstantData();
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER) * 1; // 바이트 용량
        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_cbData;
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
            &m_pConstantBuffer);
        return hr;
    }

    bool K_BaseObject::CreateShader(std::wstring filename)
    {

        m_pShader = I_Shader.Load(filename);
        if (m_pShader) 
        {
            m_pVS = m_pShader->m_pVS;
            m_pPS = m_pShader->m_pPS;
            m_pVSCode = m_pShader->m_pVSCode;
            m_pPSCode = m_pShader->m_pPSCode;
            return true;
        }

        return false;
    }

    HRESULT K_BaseObject::CreateVertexLayout()
    {
        HRESULT hr;
        // if (m_pVSCode == nullptr) return E_FAIL; // 어.. 선생님은 몬가 베이스오브젝트 멤버변수로 저게 있네

        // 정점 레이아웃은 정점 쉐이더랑 밀접한 관련이 있다
        // 정점 레이아웃 생성시 사전에 정점 쉐이더 생성이 필요하다
        D3D11_INPUT_ELEMENT_DESC ied[] = // 보통 이런식으로 열거형으로 많이 사용한다
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0,40,D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        //ied.SemanticName = "POSITION";
        //ied.SemanticIndex = 0;
        //ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        //ied.InputSlot = 0;
        //ied.AlignedByteOffset = 0; // 이게 쫌 중요한 것 같은데..? 
        //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        //ied.InstanceDataStepRate = 0;

        UINT NumElements = sizeof(ied) / sizeof(ied[0]); // 구성 원소들의 갯수..인듯?
        //void* pShaderBytecodeWithInputSignature = m_pShader->m_pVSCode->GetBufferPointer(); // 버텍스 쉐이더의 인자값이 뭐냐고
        //SIZE_T BytecodeLength = m_pShader->m_pVSCode->GetBufferSize(); // 버퍼 사이즈 (오브젝트 파일의 크기인듯?)
        //m_pVertexLayout;

        hr = m_pd3dDevice->CreateInputLayout(
            ied, // 배열의 주소니까 & 날려도됨
            NumElements,
            m_pShader->m_pVSCode->GetBufferPointer(),
            m_pShader->m_pVSCode->GetBufferSize(),
            &m_pVertexLayout);

        return hr;
    }

    void K_BaseObject::UpdateVertexBuffer()
    {
        m_pImmediateContext->UpdateSubresource(
            m_pVertexBuffer, 0, nullptr,
            &m_VertexList.at(0), 0, 0);
    }

    void K_BaseObject::UpdateConstantBuffer()
    {
        m_cbData.matWorld = m_matWorld.Transpose();
        m_cbData.matView = m_matView.Transpose();
        m_cbData.matProj = m_matProj.Transpose();

        m_pImmediateContext->UpdateSubresource(
            m_pConstantBuffer, 0, nullptr,
            &m_cbData, 0, 0);
    }

    void K_BaseObject::SetMatrix(K_Matrix* matWorld, K_Matrix* matView, K_Matrix* matProj)
    {
        if (matWorld != nullptr) m_matWorld = *matWorld;
        if (matView != nullptr) m_matView = *matView;
        if (matProj != nullptr) m_matProj = *matProj;

        UpdateConstantBuffer();
    }

    bool K_BaseObject::LoadTexture(std::wstring filename)
    {
        m_pTexture = I_Tex.Load(filename);
        if (m_pTexture != nullptr) return true;

        return false;
    }

}