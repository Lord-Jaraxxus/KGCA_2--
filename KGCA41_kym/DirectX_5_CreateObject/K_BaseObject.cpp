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
    // ��.. ��.. �����... �ؽ��� ��ǥ�� ���� ���ΰ� �� �ʿ��� �� �ϴ�

    //m_VertexList[0].t = { 1.0f, 0.0f };
    //m_VertexList[1].t = { 0.0f, 0.0f };
    //m_VertexList[4].t = { 0.0f, 0.0f }; 

    //m_VertexList[5].t = { 0.0f, 1.0f };
    //m_VertexList[2].t = { 1.0f, 1.0f };
    //m_VertexList[3].t = { 1.0f, 1.0f };
    // �ؽ�ó ��ǥ�� ����� �׸��� �¿���� ��Ų��, �ٵ� ��ü ���� ���� �� �𸣰ڵ�;

    // gpu update �Լ�
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0) ,0 ,0);
    return true;
}

bool K_BaseObject::Render()
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
    m_pImmediateContext->VSSetShader(m_pShader->m_pVS, NULL, 0); // ���ؽ� ���̴� ����, �ڿ� �ΰ��� �ϴ� ������� ������
    m_pImmediateContext->PSSetShader(m_pShader->m_pPS, NULL, 0); // �ȼ� ���̴� ����, ��� �ݵ�� �ؾ���. �׷��Ƿ� �ȼ� ���̴��� ���鷯������..

    // ������Ƽ��� ��,��,��. ��(�ﰢ��)�� ����Ʈ
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ����Ʈ (�ﰢ��)

    // PSSetShaderResources �Լ��� �ʿ��� �Ű�������
    UINT StartSlot = 0;
    UINT NumViews = 1;
    ID3D11ShaderResourceView* const* ppShaderResourceViews = &m_pTexture->m_pTextureSRV;

    // �ؽ�ó�� ���̴����ҽ��信 ���? ���̴����ҽ��並 ����? �ƹ�ư �׷��� ���������ο� �Ѱ���
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

    // NDC ��ǥ�� ����
    // x,y�� -1 ~ 1, z�� 0 ~ 1
    // v0       v1,v4
    //
    // v2,v3    v5
    // 
    // �ݵ�� �ð����(�ո�)���� �����Ѵ�.
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
    bd.ByteWidth = sizeof(SimpleVertex) * NumVertex; // ����Ʈ �뷮
    bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ������ �뵵. ����Ʈ�� GPU, �ް��ϸ� ����Ʈ ���ٻ���
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �� ���۰� ����
    //bd.CPUAccessFlags = 0; // cpu�� ���� ������ ����? �����Ӹ�
    //bd.MiscFlags = 0; // �⵿��� �÷���, �ϴ� 0
    //bd.StructureByteStride = 0; // ������ ���� ������ �����ϴ� 

    D3D11_SUBRESOURCE_DATA  sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = &m_VertexList.at(0);   // �ý��� �޸�, ��� ��
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

bool K_BaseObject::CreateShader(std::wstring filename)
{
    
    m_pShader = I_Shader.Load(filename);
    if (m_pShader) return true;

    return false;
}

HRESULT K_BaseObject::CreateVertexLayout()
{
    HRESULT hr;

    // ���� ���̾ƿ��� ���� ���̴��� ������ ������ �ִ�
    // ���� ���̾ƿ� ������ ������ ���� ���̴� ������ �ʿ��ϴ�
    D3D11_INPUT_ELEMENT_DESC ied[] = // ���� �̷������� ���������� ���� ����Ѵ�
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    //ied.SemanticName = "POSITION";
    //ied.SemanticIndex = 0;
    //ied.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    //ied.InputSlot = 0;
    //ied.AlignedByteOffset = 0; // �̰� �� �߿��� �� ������..? 
    //ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    //ied.InstanceDataStepRate = 0;

    UINT NumElements = sizeof(ied) / sizeof(ied[0]); // ���� ���ҵ��� ����..�ε�?
    void* pShaderBytecodeWithInputSignature = m_pShader->m_pVSCode->GetBufferPointer(); // ���ؽ� ���̴��� ���ڰ��� ���İ�
    SIZE_T BytecodeLength = m_pShader->m_pVSCode->GetBufferSize(); // ���� ������ (������Ʈ ������ ũ���ε�?)
    //m_pVertexLayout;

    hr = m_pd3dDevice->CreateInputLayout(
        ied, // �迭�� �ּҴϱ� & ��������
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
