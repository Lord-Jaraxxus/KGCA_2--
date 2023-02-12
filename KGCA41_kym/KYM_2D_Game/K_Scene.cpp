#include "K_Scene.h"

bool K_Scene::Init()
{
	m_pMap = new K_BackGround;
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../KymGameCore/DefaultShape.txt");
	m_pMap->SetRect({ 0, 0, (float)g_rtClient.right, (float)g_rtClient.bottom });
	m_pMap->SetPosition({ 0, 0 });

	K_Texture* pMaskTex = I_Tex.Load(L"../../data/img/525_mask.bmp");
	m_pUser = new K_Player;
	m_pUser->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"../KymGameCore/MaskShape.txt");
	m_pObjectList.push_back(m_pUser);
	m_pUser->SetMask(pMaskTex);
	m_pUser->SetRect({ 0, 51, 113, 337 });
	m_pUser->SetPosition({ g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f });

	for (int iNpc = 0; iNpc < 5; iNpc++)
	{
		K_Npc* npc = new K_Npc;
		npc->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/525.bmp", L"../KymGameCore/MaskShape.txt");
		m_pNPCList.push_back(npc);
		npc->SetMask(pMaskTex);
		//if(iNpc%2 == 0) npc->SetRect({ 120, 309, 132, 171 });
		//else npc->SetRect({ 473, 206, 102, 109 });
		npc->SetRect({ 120, 309, 132, 171 });
		npc->SetPosition({ g_rtClient.right / 6.0f + g_rtClient.right / 6.0f * iNpc , g_rtClient.bottom / 4.0f });
	}

	return true;
}

bool K_Scene::Frame()
{
	m_pMap->Frame();
	for (auto obj : m_pNPCList) { obj->Frame(); }
	for (auto obj : m_pObjectList) { obj->Frame(); }

	return true;
}

bool K_Scene::Render()
{
	m_pMap->Render();
	for (auto obj : m_pNPCList) { obj->Render(); }
	for (auto obj : m_pObjectList) { obj->Render(); }
	std::wstring MousePos = std::to_wstring(I_Input.m_ptPos.x);
	MousePos += L" ";
	MousePos += std::to_wstring(I_Input.m_ptPos.y);
	MousePos += L"\n";
	I_Write.Draw(1200, 450, MousePos, { 1,0,0,1 });

	return true;
}

bool K_Scene::Release()
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

bool K_Scene::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;

	return true;
}
