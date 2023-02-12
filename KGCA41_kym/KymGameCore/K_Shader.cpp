#include "K_Shader.h"

bool K_Shader::Init()
{
	return true;
}

bool K_Shader::Frame()
{
	return true;
}

bool K_Shader::Render()
{
	return true;
}

bool K_Shader::Release()
{
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    if (m_pVSCode) m_pVSCode->Release();
    if (m_pPSCode) m_pPSCode->Release();

    m_pVS = nullptr;
    m_pPS = nullptr;
    m_pVSCode = nullptr;
    m_pPSCode = nullptr;

	return true;
}

HRESULT K_Shader::Load( ID3D11Device* d3dDevice, 
						ID3D11DeviceContext* pContext, 
						std::wstring filename)
{
    HRESULT hr;

    m_pd3dDevice = d3dDevice;
    m_pImmediateContext = pContext;

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
        filename.c_str(),
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
        filename.c_str(),
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
