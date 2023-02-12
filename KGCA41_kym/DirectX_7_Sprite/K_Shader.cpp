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
