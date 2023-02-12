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
        bd.ByteWidth = iVertexSize * iNumVertex; // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = pDataAddress;
        hr = pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
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
        // ��.. ��.. �����... �ؽ��� ��ǥ�� ���� ���ΰ� �� �ʿ��� �� �ϴ�

        //m_VertexList[0].t = { 1.0f, 0.0f };
        //m_VertexList[1].t = { 0.0f, 0.0f };
        //m_VertexList[4].t = { 0.0f, 0.0f }; 

        //m_VertexList[5].t = { 0.0f, 1.0f };
        //m_VertexList[2].t = { 1.0f, 1.0f };
        //m_VertexList[3].t = { 1.0f, 1.0f };
        // �ؽ�ó ��ǥ�� ����� �׸��� �¿���� ��Ų��, �ٵ� ��ü ���� ���� �� �𸣰ڵ�;

        //m_VertexList[0] = { {frame_test, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f} };
        //if (frame_test <= -1.0f) { test_flag = false; }
        //else if (frame_test >= 0.0f) { test_flag = true; }

        //if (test_flag == true) frame_test -= 0.001f;
        //else frame_test += 0.001f;

        // gpu update �Լ�
        //m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0) ,0 ,0);
        return true;
    }

    bool K_BaseObject::PreRender()
    {
        UINT stride = sizeof(PNCT_VERTEX); // ���� 1���� ����Ʈ�뷮
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
        m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0); // �ε��� ���� ����
        m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer); // �ܽ�źƮ ���� ����, 2D���� �ʿ���ݾ�

        m_pImmediateContext->IASetInputLayout(m_pVertexLayout); // ��ǲ-����� �������� ����
        m_pImmediateContext->VSSetShader(m_pShader->m_pVS, NULL, 0); // ���ؽ� ���̴� ����, �ڿ� �ΰ��� �ϴ� ������� ������
        m_pImmediateContext->PSSetShader(m_pShader->m_pPS, NULL, 0); // �ȼ� ���̴� ����, ��� �ݵ�� �ؾ���. �׷��Ƿ� �ȼ� ���̴��� ���鷯������..

        // ������Ƽ��� ��,��,��. ��(�ﰢ��)�� ����Ʈ
        //m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ����Ʈ (�ﰢ��)

        // PSSetShaderResources �Լ��� �ʿ��� �Ű�������
        //UINT StartSlot = 0;
        //UINT NumViews = 1;
        //ID3D11ShaderResourceView* const* ppShaderResourceViews = &m_pTexture->m_pTextureSRV;

        //// �ؽ�ó�� ���̴����ҽ��信 ���? ���̴����ҽ��並 ����? �ƹ�ư �׷��� ���������ο� �Ѱ���
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
            //m_pImmediateContext->DrawIndexed(m_dwFace * 3, 0, 0); // �̷��� �ٲ���ϳ�..?

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

        // �������ۿ� �ε���
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
        bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size(); // ����Ʈ �뷮

        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ������ �뵵. ����Ʈ�� GPU, �ް��ϸ� ����Ʈ ���ٻ���
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �� ���۰� ����

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_VertexList.at(0);   // �ý��� �޸�, ��� ��

        hr = m_pd3dDevice->CreateBuffer
        (
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ä��� CPU �޸� �ּ�
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
        bd.ByteWidth = sizeof(DWORD) * m_IndexList.size(); // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_IndexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
            &m_pIndexBuffer);
        return hr;
    }

    HRESULT K_BaseObject::CreateConstantBuffer()
    {
        HRESULT hr;
        CreateConstantData();
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER) * 1; // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_cbData;
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
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
        // if (m_pVSCode == nullptr) return E_FAIL; // ��.. �������� �� ���̽�������Ʈ ��������� ���� �ֳ�

        // ���� ���̾ƿ��� ���� ���̴��� ������ ������ �ִ�
        // ���� ���̾ƿ� ������ ������ ���� ���̴� ������ �ʿ��ϴ�
        D3D11_INPUT_ELEMENT_DESC ied[] = // ���� �̷������� ���������� ���� ����Ѵ�
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
        //ied.AlignedByteOffset = 0; // �̰� �� �߿��� �� ������..? 
        //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        //ied.InstanceDataStepRate = 0;

        UINT NumElements = sizeof(ied) / sizeof(ied[0]); // ���� ���ҵ��� ����..�ε�?
        //void* pShaderBytecodeWithInputSignature = m_pShader->m_pVSCode->GetBufferPointer(); // ���ؽ� ���̴��� ���ڰ��� ���İ�
        //SIZE_T BytecodeLength = m_pShader->m_pVSCode->GetBufferSize(); // ���� ������ (������Ʈ ������ ũ���ε�?)
        //m_pVertexLayout;

        hr = m_pd3dDevice->CreateInputLayout(
            ied, // �迭�� �ּҴϱ� & ��������
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