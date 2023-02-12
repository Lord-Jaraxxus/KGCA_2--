#include "K_TextureManager.h"

bool K_TextureManager::Init()
{
    return true;
}

bool K_TextureManager::Frame()
{
    return true;
}

bool K_TextureManager::Render()
{
    return true;
}

bool K_TextureManager::Release()
{
    for (auto data : m_List)
    {
        K_Texture* pTexture = data.second;
        if (pTexture) pTexture->Release();
        delete pTexture;
    }
    m_List.clear();
    return true;
}

K_TextureManager::K_TextureManager()
{
    m_iIndex = 0;
}

K_TextureManager::~K_TextureManager()
{
    Release();
}



void K_TextureManager::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}

K_Texture* K_TextureManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end()) return iter->second;
    
    return nullptr;
}

K_Texture* K_TextureManager::Load(std::wstring name)
{
    HRESULT hr;

    //중복 제거
    auto data = Find(name);
    if (data != nullptr) return data;

    K_Texture* pNewData = new K_Texture;
    if (pNewData)
    {
        hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, name);
        if (SUCCEEDED(hr)) m_List.insert(std::make_pair(name, pNewData));
    }
    return pNewData;
}