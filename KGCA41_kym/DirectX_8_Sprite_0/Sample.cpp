#include "Sample.h"
#include "K_TextureManager.h"

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
	m_vDrawSize.x = m_rtUV.z * 1.5f;
	m_vDrawSize.y = m_rtUV.w * 1.5f;

	m_vPos = pos;
	m_vDrawPos.x = (pos.x / g_rtClient.right)*2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	m_vDrawPos.x = m_vDrawPos.x - m_vDrawSize.x / 2;
	m_vDrawPos.y = m_vDrawPos.y + m_vDrawSize.y / 2;

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
	k_Vector2D vPos = m_vPos;
	if (I_Input.GetKey('W')) vPos.y += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('A')) vPos.x += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('S')) vPos.y += 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('D')) vPos.x += 1.0f * g_fSecondPerFrame * 100.0f;
	SetPosition(vPos);

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

bool K_2dObject::Render()
{
	PreRender();
	ID3D11ShaderResourceView* srv = m_pMaskTex->GetSRV();
	m_pImmediateContext->PSSetShaderResources(1, 1, &srv);
	PostRender();

	return true;
}

void K_2dObject::SetMask(K_Texture* pMaskTex)
{
	m_pMaskTex = pMaskTex;
}

bool K_Npc::Frame()
{
	k_Vector2D vPos = m_vPos;
	vPos.y += 0.2f * g_fSecondPerFrame * 100.0f;
	SetPosition(vPos);

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}


bool Sample::Init() 
{
	m_pMap = new K_BaseObject;
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"DefaultShape.txt");

	K_Texture* pMaskTex = I_Tex.Load(L"../../data/img/525_mask.bmp");
	m_pUser = new K_2dObject;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"MaskShape.txt");
	m_pObjectList.push_back(m_pUser);
	m_pUser->SetMask(pMaskTex);
	m_pUser->SetRect({ 0, 51, 113, 337 });
	m_pUser->SetPosition({g_rtClient.right/2.0f, g_rtClient.bottom/2.0f });

	for (int iNpc = 0; iNpc < 5; iNpc++) 
	{
		K_Npc* npc = new K_Npc;
		npc->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"MaskShape.txt");
		m_pNPCList.push_back(npc);
		npc->SetMask(pMaskTex);
		if(iNpc%2 == 0) npc->SetRect({ 120, 309, 132, 171 });
		else npc->SetRect({ 473, 206, 102, 109 });
		npc->SetPosition({ g_rtClient.right/6.0f + g_rtClient.right/6.0f*iNpc , g_rtClient.bottom / 4.0f });
	}

	return true; 
}

bool Sample::Frame()
{
	m_pMap->Frame();
	for (auto obj : m_pNPCList) { obj->Frame(); }
	for (auto obj : m_pObjectList) { obj->Frame(); }
	return true;
}

bool Sample::Render()
{
	m_pMap->Render();
	for (auto obj : m_pNPCList) { obj->Render(); }
	for (auto obj : m_pObjectList) { obj->Render(); }

	return true;
}

bool Sample::Release()
{
	m_pMap->Release();
	delete m_pMap;

	for (auto obj : m_pNPCList)
	{
		obj->Release();
		delete obj;
	}

	for (auto obj : m_pObjectList) 
	{ 
		obj->Release(); 
		delete obj;
	}

	return true; 
}


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow) 
//{
//    Sample demo;
//    demo.SetWindow(hInstance);
//    demo.Run();
//    return 1;
//}  

GAME_RUN(CreateObject, 1200, 900)
