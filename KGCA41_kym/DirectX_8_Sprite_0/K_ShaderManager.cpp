#include "K_ShaderManager.h"

bool K_ShaderManager::Init()
{
    return true;
}

bool K_ShaderManager::Frame()
{
    return true;
}

bool K_ShaderManager::Render()
{
    return true;
}

bool K_ShaderManager::Release()
{
    for (auto data : m_List)
    {
        K_Shader* pData = data.second;
        if (pData) pData->Release();
        delete pData;
    }
    m_List.clear();
    return true;
}

K_ShaderManager::K_ShaderManager()
{
    m_iIndex = 0;
}

K_ShaderManager::~K_ShaderManager()
{
    Release();
}



void K_ShaderManager::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}

K_Shader* K_ShaderManager::Load(std::wstring name)
{
    HRESULT hr;

    //중복 제거
    auto iter = m_List.find(name);
    if (iter != m_List.end()) return iter->second;

    K_Shader* pNewData = new K_Shader;
    if (pNewData)
    {
        hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, name);
        if (SUCCEEDED(hr)) m_List.insert(std::make_pair(name, pNewData));
    }
    return pNewData;
}