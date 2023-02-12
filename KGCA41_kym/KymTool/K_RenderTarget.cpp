#include "pch.h"

#include "K_RenderTarget.h"

HRESULT K_RenderTarget::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, FLOAT fWidth, FLOAT fHeight)
{
    HRESULT hr;

    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pImmediateContext;

    // 뷰포트 설정, 근데 얘 뷰포트 어디서 쓰냐? 아 바로밑에 Begin에서 쓰네
    m_Viewport.Width = fWidth;
    m_Viewport.Height = fHeight;
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;

    // 텍스처 데스크 설정, 바로밑에서 씀
    m_TexDesc.Width = (UINT)fWidth;
    m_TexDesc.Height = (UINT)fHeight;
    m_TexDesc.MipLevels = 1;
    m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_TexDesc.SampleDesc.Count = 1;
    m_TexDesc.SampleDesc.Quality = 0;
    m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
    m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    m_TexDesc.CPUAccessFlags = 0;
    m_TexDesc.MiscFlags = 0;
    m_TexDesc.ArraySize = 1;

    // 텍스처 생성
    if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, &m_pTexture))) return hr;
    // 위에서 만든 텍스처를 쉐이더 리소스 뷰로 설정 (원래 파일에서 텍스처 긁어오면 요 SRV에 정보가 담겼음)
    if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pSRV))) return hr;
    // 위에서 만든 텍스처를 렌더 타겟 뷰로 설정
    if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture, NULL, &m_pRenderTargetView))) return hr;



    // Depth Buffer
    ID3D11Texture2D* pDSTexture = nullptr;
    D3D11_TEXTURE2D_DESC DescDepth;
    DescDepth.Width = fWidth;
    DescDepth.Height = fHeight;
    DescDepth.MipLevels = 1;
    DescDepth.ArraySize = 1;
    DescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
    DescDepth.SampleDesc.Count = 1;
    DescDepth.SampleDesc.Quality = 0;
    DescDepth.Usage = D3D11_USAGE_DEFAULT;
    // 백 버퍼 깊이 및 스텐실 버퍼 생성
    DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DescDepth.CPUAccessFlags = 0;
    DescDepth.MiscFlags = 0;
    if (FAILED(hr = pd3dDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture))) return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if (FAILED(hr = pd3dDevice->CreateDepthStencilView(pDSTexture, &dsvDesc, &m_pDepthStencilView))) return hr;
 
    return hr;
}

bool K_RenderTarget::Begin()
{
    /*UINT iNumViewport = 1;
    pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);
    pContext->RSGetViewports(&iNumViewport, m_vpOld);*/

    // 렌더 타겟을 얘가 갖고있는 텍스처로 바꿔줌
    ID3D11RenderTargetView* pNullRTV = NULL;
    m_pImmediateContext->OMSetRenderTargets(1, &pNullRTV, NULL);
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // 클리어도 함 해주고, 근데 난 어차피 Device PreRender에서 해주는데? 뭐 여러번 지우지 
    const FLOAT color[] = { 1, 1, 1, 1 };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

    // 뷰포트도 얘껄로 바꿔줌, 그럼 디폴트로 냅두면 1024x1024 되나? 아 해상도는 알아서 맞춰주던가 나중에 256 넣어보지 뭐
    m_pImmediateContext->RSSetViewports(1, &m_Viewport);

    return true;
}

void K_RenderTarget::End()
{
    // 렌더타겟이랑 뷰포트 갖고있
    m_pImmediateContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
    m_pImmediateContext->RSSetViewports(1, &m_OldViewport);
}

bool K_RenderTarget::Release()
{
    // 여기서 할거 없나? 흐므
    return true;
}
