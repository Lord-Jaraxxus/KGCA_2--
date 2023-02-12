#include "Sample.h"
#include "K_TextureManager.h"

bool K_Map::Frame()
{
	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH) m_iStage++;

	float MovePerFrame = 0.5f * g_fSecondPerFrame;
	if (m_iStage > 0)
	{
		if (m_rtUV.x + MovePerFrame <= 0.1 * m_iStage && m_rtUV.x + m_rtUV.z + MovePerFrame <= 1.0f)
		{
			m_rtUV.x += MovePerFrame;
		}
	}

	//if (I_Input.GetKey('D') && m_rtUV.x + m_rtUV.z + MovePerFrame <= 1.0f) m_rtUV.x += MovePerFrame;
	//if (I_Input.GetKey('A') && m_rtUV.x - MovePerFrame >= 0.0f) m_rtUV.x -= MovePerFrame;

	//if (I_Input.GetKey('W') && m_rtUV.y - MovePerFrame >= 0.0f) m_rtUV.y -= MovePerFrame;
	//if (I_Input.GetKey('S') && m_rtUV.y + m_rtUV.w + MovePerFrame <= 1.0f) m_rtUV.y += MovePerFrame;

	UpdateVertexBuffer();
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_Map::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	//m_vDrawPos.x = m_vDrawPos.x - m_vDrawSize.x / 2;
	//m_vDrawPos.y = m_vDrawPos.y + m_vDrawSize.y / 2;

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = (m_rtInit.z / g_rtClient.right) * 2.0f;
	m_vDrawSize.y = (m_rtInit.w / g_rtClient.bottom) * 2.0f;

	// 클라이언트 크기 바꾸면 찌그러지도록
	//m_vDrawSize.x = m_rtInit.z / m_ptImageSize.x;
	//m_vDrawSize.y = m_rtInit.w / m_ptImageSize.y;

	UpdateVertexBuffer();
}


bool K_Npc::Frame()
{
	k_Vector2D vPos = m_vPos;
	vPos.y += 0.2f * g_fSecondPerFrame * 100.0f;
	SetPosition(vPos);
	if (I_Input.GetKey(VK_LBUTTON))
	{
		float mouseX = I_Input.m_ptPos.x;
		float mouseY = I_Input.m_ptPos.y;
		float mouseNdcX = (mouseX / g_rtClient.right) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
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
	if (I_Input.GetKey('W')) vPos.y += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('A')) vPos.x += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('S')) vPos.y += 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('D')) vPos.x += 1.0f * g_fSecondPerFrame * 100.0f;

	SetPosition(vPos);

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_Player::UpdateVertexBuffer()
{
	m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.5f };
	m_VertexList[0].t = { m_rtUV.x, m_rtUV.y };

	m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.5f };
	m_VertexList[1].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y };

	m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.5f };
	m_VertexList[2].t = { m_rtUV.x, m_rtUV.y + m_rtUV.w };

	m_VertexList[3].p = m_VertexList[2].p;
	m_VertexList[3].t = m_VertexList[2].t;

	m_VertexList[4].p = m_VertexList[1].p;
	m_VertexList[4].t = m_VertexList[1].t;

	m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.5f };
	m_VertexList[5].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y + m_rtUV.w };
	
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0), 0, 0);
}

bool Sample::Init() 
{
	m_pMap = new K_Map;
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../KymGameCore/DefaultShape.txt");
	m_pMap->SetRect({ 0, 0, (float)g_rtClient.right, (float)g_rtClient.bottom });
	m_pMap->SetPosition({ 0, 0 });

	K_Texture* pMaskTex = I_Tex.Load(L"../../data/img/525_mask.bmp");
	m_pUser = new K_Player;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"../KymGameCore/MaskShape.txt");
	m_pObjectList.push_back(m_pUser);
	m_pUser->SetMask(pMaskTex);
	m_pUser->SetRect({ 0, 51, 113, 337 });
	m_pUser->SetPosition({g_rtClient.right/2.0f, g_rtClient.bottom/2.0f });

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
	}

	I_Sound.Init();
	m_pBGM = I_Sound.Load(L"../../data/sound/hello_how_are_you.mp3");
	m_pEffect1 = I_Sound.Load(L"../../data/sound/Gun1.wav");
	m_pEffect2 = I_Sound.Load(L"../../data/sound/Gun2.wav");

	return true; 
}

bool Sample::Frame()
{
	m_pMap->Frame();
	for (auto obj : m_pNPCList) { obj->Frame(); }
	for (auto obj : m_pObjectList) { obj->Frame(); }

	if (I_Input.GetKey(VK_F1) == KEY_PUSH) 
		m_pBGM->Play(1);
	if (I_Input.GetKey(VK_F2) == KEY_PUSH) m_pBGM->Stop();
	if (m_pBGM->m_pChannel != nullptr)
	{
		if (I_Input.GetKey(VK_RBUTTON) == KEY_PUSH) m_pBGM->Pause();
		if (I_Input.GetKey(VK_F3) == KEY_PUSH) m_pBGM->Volume(true);
		if (I_Input.GetKey(VK_F4) == KEY_PUSH) m_pBGM->Volume(false);
	}
	m_pBGM->Frame();

	if (I_Input.GetKey(VK_F5) == KEY_PUSH) m_pEffect1->PlayEffect();
	if (I_Input.GetKey(VK_F6) == KEY_PUSH) m_pEffect2->PlayEffect();


	float fWhat = sin(g_fGameTimer) * 0.5f + 0.5f;
	K_Matrix scale, rotation, translation;
	K_Matrix projection;
	scale = scale.Scale(0.5f, 0.5f, 0.5f);
	rotation = rotation.RotationZ(g_fGameTimer);
	translation = translation.Translation(0.5f, 0.0f, 0.0f); 
	projection = projection.PerspectiveFovLH(0.0f, 10.0f, 90.0f, 16.0f/9.0f);

	for (int i = 0; i < m_pUser->m_InitVertexList.size(); i++)
	{
		k_Vector v = m_pUser->m_InitVertexList[i].p;
		//v = v * scale; // s * r * t 		
		//v = v * rotation;
		v = v * translation;
		v = v * projection;
		m_pUser->m_VertexList[i].p = v;
	}
	m_pImmediateContext->UpdateSubresource(m_pUser->m_pVertexBuffer, 0, nullptr, &m_pUser->m_VertexList.at(0), 0, 0);


	return true;
}

bool Sample::Render()
{
	m_pMap->Render();
	for (auto obj : m_pNPCList) { obj->Render(); }
	for (auto obj : m_pObjectList) { obj->Render(); }
	I_Write.Draw(0, 100, m_pBGM->m_szBuffer);

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

	I_Sound.Release();
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

GAME_RUN(CreateObject, 800, 900)
