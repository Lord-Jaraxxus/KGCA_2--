#include "K_DxState.h"

ID3D11SamplerState* K_DxState::g_pDefaultSS = nullptr;

bool K_DxState::SetState(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    // CreateSamplerState �Ű�������
    D3D11_SAMPLER_DESC sd; // 1. ���÷� ����ũ
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // �ֱ��� ���͸�
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    //FLOAT MipLODBias;
    //UINT MaxAnisotropy;
    //D3D11_COMPARISON_FUNC ComparisonFunc;
    //FLOAT BorderColor[4];
    //FLOAT MinLOD;
    //FLOAT MaxLOD;

    //ID3D11SamplerState** ppSamplerState = &g_pDefaultSS; // 2. ���÷� ������Ʈ, ��� �־ ��ȯ�޾ƿ��� ���� �װ���


    hr = pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSS);
    return true;
}

bool K_DxState::Release()
{
    if (g_pDefaultSS) g_pDefaultSS->Release();
    return true;
}
