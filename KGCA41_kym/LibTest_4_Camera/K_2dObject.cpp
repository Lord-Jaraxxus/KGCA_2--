#include "K_2dObject.h"
#include "K_Input.h"

void K_2dObject::SetCamera(k_Vector2D vCameraPos)
{
	m_vCameraPos = vCameraPos;
}

void K_2dObject::SetRect(k_Vector4D rt)
{
	m_rtInit = rt;
	m_ptImageSize.x = m_pTexture->m_Desc.Width;
	m_ptImageSize.y = m_pTexture->m_Desc.Height;

	m_rtUV.x = rt.x / m_ptImageSize.x;
	m_rtUV.y = rt.y / m_ptImageSize.y;
	m_rtUV.z = rt.z / m_ptImageSize.x;
	m_rtUV.w = rt.w / m_ptImageSize.y;
	// 이미지에 대한 w,h 값이 필요하다 즉, 이미지의 크기가 필요하다
}

void K_2dObject::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;

	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;

	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	//m_vDrawPos.x = m_vDrawPos.x - m_vDrawSize.x / 2;
	//m_vDrawPos.y = m_vDrawPos.y + m_vDrawSize.y / 2;

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = m_rtInit.z / g_rtClient.right;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom;

	// 클라이언트 크기 바꾸면 찌그러지도록
	//m_vDrawSize.x = m_rtInit.z / m_ptImageSize.x;
	//m_vDrawSize.y = m_rtInit.w / m_ptImageSize.y;

	UpdateVertexBuffer();
}

void K_2dObject::UpdateVertexBuffer()
{
	m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
	m_VertexList[0].t = { m_rtUV.x, m_rtUV.y };

	m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.0f };
	m_VertexList[1].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y };

	m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[2].t = { m_rtUV.x, m_rtUV.y + m_rtUV.w };

	m_VertexList[3].p = m_VertexList[2].p;
	m_VertexList[3].t = m_VertexList[2].t;

	m_VertexList[4].p = m_VertexList[1].p;
	m_VertexList[4].t = m_VertexList[1].t;

	m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[5].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y + m_rtUV.w };
}

bool K_2dObject::Frame()
{
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

bool K_2dObject::Render()
{
	PreRender();
	if (m_pMaskTex != nullptr) 
	{
		ID3D11ShaderResourceView* srv = m_pMaskTex->GetSRV();
		m_pImmediateContext->PSSetShaderResources(1, 1, &srv);
	}
	PostRender();

	return true;
}

void K_2dObject::SetMask(K_Texture* pMaskTex)
{
	m_pMaskTex = pMaskTex;
}

