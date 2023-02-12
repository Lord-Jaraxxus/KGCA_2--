#include "K_Texture.h"

bool K_Texture::Init()
{
    return true;
}

bool K_Texture::Frame()
{
    return true;
}

bool K_Texture::Render()
{
    return true;
}

bool K_Texture::Release()
{
    if (m_pTextureSRV) m_pTextureSRV->Release();
    if (m_pTexture) m_pTexture->Release();

    m_pTextureSRV = nullptr;
    m_pTexture = nullptr;

    return true;
}



HRESULT K_Texture::Load(ID3D11Device* d3dDevice,
                        ID3D11DeviceContext* pContext,
                        std::wstring filename)
{
    //"crusader.sprite.combat.png"
    HRESULT hr; 

    m_pd3dDevice = d3dDevice;
    m_pImmediateContext = pContext;

    hr = DirectX::CreateWICTextureFromFile(
            d3dDevice,
            pContext,
            filename.c_str(),
            (ID3D11Resource**)&m_pTexture,
            &m_pTextureSRV);

    m_pTexture->GetDesc(&m_Desc);
    return hr;
}