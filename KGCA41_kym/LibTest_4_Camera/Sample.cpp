#include "Sample.h"
#include "K_TextureManager.h"

k_Vector2D g_vCameraPos;

bool K_Map::Frame()
{

	//if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH) m_iStage++;

	//float MovePerFrame = 0.5f * g_fSecondPerFrame;
	//if (m_iStage > 0)
	//{
	//	if (m_rtUV.x + MovePerFrame <= 0.1 * m_iStage && m_rtUV.x + m_rtUV.z + MovePerFrame <= 1.0f)
	//	{
	//		m_rtUV.x += MovePerFrame;
	//	}
	//}

	//if (I_Input.GetKey('D') && m_rtUV.x + m_rtUV.z + MovePerFrame <= 1.0f) m_rtUV.x += MovePerFrame;
	//if (I_Input.GetKey('A') && m_rtUV.x - MovePerFrame >= 0.0f) m_rtUV.x -= MovePerFrame;

	//if (I_Input.GetKey('W') && m_rtUV.y - MovePerFrame >= 0.0f) m_rtUV.y -= MovePerFrame;
	//if (I_Input.GetKey('S') && m_rtUV.y + m_rtUV.w + MovePerFrame <= 1.0f) m_rtUV.y += MovePerFrame;

	//if (I_Input.GetKey(VK_LBUTTON)) m_fZoom += 1.0f * g_fSecondPerFrame * 1.0f;
	//if (I_Input.GetKey(VK_RBUTTON)) m_fZoom -= 1.0f * g_fSecondPerFrame * 1.0f;

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		if (m_iZoomFlag == 0) m_iZoomFlag = 1; 
	}
	if (m_iZoomFlag == 1) 
	{
		if (m_fZoom > 2.1f) m_iZoomFlag = 2;
		else m_fZoom += 2.0f * g_fSecondPerFrame * 1.0f;
	}
	else if (m_iZoomFlag == 2) 
	{
		if (m_fZoom < 2.0f)
		{
			m_fZoom = 2.0f;
			m_iZoomFlag = 0;
		}
		else m_fZoom -= 2.0f * g_fSecondPerFrame * 1.0f;
	}

	SetCamera(g_vCameraPos);
	SetPosition(m_vPos);
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_Map::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;

	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;

	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	m_vDrawSize.x = 2.0f * (m_rtInit.z / g_rtClient.right) * m_fZoom;
	m_vDrawSize.y = 2.0f * (m_rtInit.w / g_rtClient.bottom) * m_fZoom;

	//m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	//m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	//m_vDrawPos.x = m_vDrawPos.x - m_vDrawSize.x / 2;
	//m_vDrawPos.y = m_vDrawPos.y + m_vDrawSize.y / 2;

	// Ŭ���̾�Ʈ ũ�� �ٲ㵵 ���� �̻ڰ� ����, �ػ󵵰� ���̴� ���� �׻� ������ �ȼ���ŭ ��Ƹ���
	//m_vDrawSize.x = (m_rtInit.z / g_rtClient.right) * 2.0f;
	//m_vDrawSize.y = (m_rtInit.w / g_rtClient.bottom) * 2.0f;

	// Ŭ���̾�Ʈ ũ�� �ٲٸ� ��׷�������
	//m_vDrawSize.x = m_rtInit.z / m_ptImageSize.x;
	//m_vDrawSize.y = m_rtInit.w / m_ptImageSize.y;

	UpdateVertexBuffer();
}


bool K_Npc::Frame()
{
	k_Vector2D vPos = m_vPos;
	vPos.y += 0.2f * g_fSecondPerFrame * 100.0f;
	SetCamera(g_vCameraPos);
	SetPosition(vPos);
	if (I_Input.GetKey(VK_LBUTTON))
	{
		float mouseX = I_Input.m_ptPos.x;
		float mouseY = I_Input.m_ptPos.y;
		float mouseNdcX = (mouseX / g_rtClient.right) * 2.0f - 1.0f; // ���콺 x��ǥ�� NDC��ǥ��� ��ȯ
		float mouseNdcY = -((mouseY / g_rtClient.bottom) * 2.0f - 1.0f);

		if (m_VertexList[0].p.x <= mouseNdcX && m_VertexList[5].p.x >= mouseNdcX &&
			m_VertexList[0].p.y >= mouseNdcY && m_VertexList[5].p.y <= mouseNdcY)
		{
			SetRect({ 473, 206, 102, 109 });
			SetPosition(vPos);
		}
	}
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

bool K_Player::Frame()
{
	k_Vector2D vPos = m_vPos;
	//if (I_Input.GetKey('W')) vPos.y += -1.0f * g_fSecondPerFrame * 100.0f;
	//if (I_Input.GetKey('A')) vPos.x += -1.0f * g_fSecondPerFrame * 100.0f;
	//if (I_Input.GetKey('S')) vPos.y += 1.0f * g_fSecondPerFrame * 100.0f;
	//if (I_Input.GetKey('D')) vPos.x += 1.0f * g_fSecondPerFrame * 100.0f;
	SetCamera(g_vCameraPos);
	k_Vector2D vPlayerPos = g_vCameraPos;
	vPlayerPos.x += g_rtClient.right / 2.0f;
	vPlayerPos.y += g_rtClient.bottom / 2.0f;
	SetPosition(vPlayerPos);

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

bool Sample::Init() 
{
	g_vCameraPos = { 0,0 };
	m_pMap = new K_Map;
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../KymGameCore/DefaultShape.txt");
	m_pMap->SetRect({ 0, 0, (float)m_pMap->m_pTexture->m_Desc.Width, (float)m_pMap->m_pTexture->m_Desc.Height });
	m_pMap->SetPosition({ 0, 0 });
	m_pMap->SetCamera(g_vCameraPos);

	K_Texture* pMaskTex = I_Tex.Load(L"../../data/img/525_mask.bmp");
	m_pUser = new K_Player;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"../KymGameCore/MaskShape.txt");
	m_pObjectList.push_back(m_pUser);
	m_pUser->SetMask(pMaskTex);
	m_pUser->SetRect({ 0, 51, 113, 337 });
	m_pUser->SetPosition({g_rtClient.right/2.0f, g_rtClient.bottom/2.0f });
	m_pUser->SetCamera(g_vCameraPos);

	for (int iNpc = 0; iNpc < 5; iNpc++) 
	{
		K_Npc* npc = new K_Npc;
		npc->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"../KymGameCore/MaskShape.txt");
		m_pNPCList.push_back(npc);
		npc->SetMask(pMaskTex);
		//if(iNpc%2 == 0) npc->SetRect({ 120, 309, 132, 171 });
		//else npc->SetRect({ 473, 206, 102, 109 });
		npc->SetRect({ 120, 309, 132, 171 });
		npc->SetPosition({ g_rtClient.right/6.0f + g_rtClient.right/6.0f*iNpc , g_rtClient.bottom / 4.0f });
		npc->SetCamera(g_vCameraPos);
	}

	return true; 
}

bool Sample::Frame()
{
	if (I_Input.GetKey('A')) 
		g_vCameraPos.x -= 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('D')) g_vCameraPos.x += 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('W')) g_vCameraPos.y -= 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('S')) g_vCameraPos.y += 1.0f * g_fSecondPerFrame * 100.0f;

	m_pMap->Frame();
	for (auto obj : m_pNPCList) { obj->Frame(); }
	for (auto obj : m_pObjectList) { obj->Frame(); }
	return true;
}

bool Sample::Render()
{
	D3D11_VIEWPORT vp;
	vp.Width = 1600;
	vp.Height = 900;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);

	m_pMap->Render();
	for (auto obj : m_pNPCList) { obj->Render(); }
	for (auto obj : m_pObjectList) { obj->Render(); }

	vp.Width = 320;
	vp.Height = 180;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &vp);

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

GAME_RUN(CreateObject, 1600, 900)
