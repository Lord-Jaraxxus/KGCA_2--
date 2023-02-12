#include "K_DxState.h"

ID3D11SamplerState* K_DxState::g_pDefaultSS = nullptr;

bool K_DxState::SetState(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    // CreateSamplerState 매개변수들
    D3D11_SAMPLER_DESC sd; // 1. 샘플러 데스크
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // 최근점 필터링
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    //FLOAT MipLODBias;
    //UINT MaxAnisotropy;
    //D3D11_COMPARISON_FUNC ComparisonFunc;
    //FLOAT BorderColor[4];
    //FLOAT MinLOD;
    //FLOAT MaxLOD;

    //ID3D11SamplerState** ppSamplerState = &g_pDefaultSS; // 2. 샘플러 스테이트, 얘는 넣어서 반환받아오는 대충 그거임


    hr = pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSS);
    return true;
}

bool K_DxState::Release()
{
    if (g_pDefaultSS) g_pDefaultSS->Release();
    return true;
}
