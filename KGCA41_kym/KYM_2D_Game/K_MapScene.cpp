#include "K_MapScene.h"

bool K_MapScene::Init()
{
	m_pBGM = I_Sound.Load(L"../../data/sound/audio/using/bgm/Map.mp3");
	m_pSelectSound = I_Sound.Load(L"../../data/sound/audio/using/MapSelect.ogg");
	m_pViewDeckSound = I_Sound.Load(L"../../data/sound/audio/using/DeckView.ogg");

	m_vCameraPos = { 160,50 };


	// 맵
	K_Texture* pMapTopMask = I_Tex.Load(L"../../data/img/map/mapTop(3).jpg");
	K_Map* MapTop = new K_Map;
	MapTop->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/mapTop(3).jpg", L"../../data/shader/MaskShape.txt");
	MapTop->SetMask(pMapTopMask);
	MapTop->SetCamera(m_vCameraPos);
	MapTop->SetRect({ 0, 0, (float)MapTop->m_pTexture->m_Desc.Width, (float)MapTop->m_pTexture->m_Desc.Height });
	MapTop->SetPosition({ 0, 0 });
	m_pBackGroundList.push_back(MapTop);

	K_Texture* pMapMidMask = I_Tex.Load(L"../../data/img/map/mapMid.png");
	K_Map* MapMid = new K_Map;
	MapMid->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/mapMid.png", L"../KymGameCore/MaskShape_Png.txt");
	MapMid->SetMask(pMapMidMask);
	MapMid->SetCamera(m_vCameraPos);
	MapMid->SetRect({ 0, 0, (float)MapMid->m_pTexture->m_Desc.Width, (float)MapMid->m_pTexture->m_Desc.Height });
	MapMid->SetPosition({ 0, 1080 });
	m_pBackGroundList.push_back(MapMid);

	K_Texture* pMapBottomMask = I_Tex.Load(L"../../data/img/map/mapBot.png");
	K_Map* MapBottom = new K_Map;
	MapBottom->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/mapBot.png", L"../KymGameCore/MaskShape_Png.txt");
	MapBottom->SetMask(pMapBottomMask);
	MapBottom->SetCamera(m_vCameraPos);
	MapBottom->SetRect({ 0, 0, (float)MapBottom->m_pTexture->m_Desc.Width, (float)MapBottom->m_pTexture->m_Desc.Height });
	MapBottom->SetPosition({ 0, 2060 });
	m_pBackGroundList.push_back(MapBottom);


	// 전투
	K_Texture* pMonsterMask = I_Tex.Load(L"../../data/img/map/monster.png");
	K_MapObject* pMonster1 = new K_MapObject;
	pMonster1->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/monster.png", L"../../data/shader/MaskShape_Png_Black.txt");
	pMonster1->SetMask(pMonsterMask);
	pMonster1->SetCamera(m_vCameraPos);
	pMonster1->SetRect({ 0, 0, (float)pMonster1->m_pTexture->m_Desc.Width, (float)pMonster1->m_pTexture->m_Desc.Height });
	pMonster1->SetPosition({ 600, 855 });
	pMonster1->m_iStage = 0;
	m_pMapObjectList.push_back(pMonster1);

	K_MapObject* pMonster2 = new K_MapObject;
	pMonster2->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/monster.png", L"../../data/shader/MaskShape_Png_Black.txt");
	pMonster2->SetMask(pMonsterMask);
	pMonster2->SetCamera(m_vCameraPos);
	pMonster2->SetRect({ 0, 0, (float)pMonster2->m_pTexture->m_Desc.Width, (float)pMonster2->m_pTexture->m_Desc.Height });
	pMonster2->SetPosition({ 910, 750 });
	pMonster2->m_iStage = 0;
	m_pMapObjectList.push_back(pMonster2);

	K_MapObject* pMonster3 = new K_MapObject;
	pMonster3->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/monster.png", L"../../data/shader/MaskShape_Png_Black.txt");
	pMonster3->SetMask(pMonsterMask);
	pMonster3->SetCamera(m_vCameraPos);
	pMonster3->SetRect({ 0, 0, (float)pMonster3->m_pTexture->m_Desc.Width, (float)pMonster3->m_pTexture->m_Desc.Height });
	pMonster3->SetPosition({ 1350, 860 });
	pMonster3->m_iStage = 0;
	m_pMapObjectList.push_back(pMonster3);

	K_Texture* pEliteMask = I_Tex.Load(L"../../data/img/map/elite.png");
	K_MapObject* pElite1 = new K_MapObject;
	pElite1->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/elite.png", L"../../data/shader/MaskShape_Png_Black.txt");
	pElite1->SetMask(pEliteMask);
	pElite1->SetCamera(m_vCameraPos);
	pElite1->SetRect({ 0, 0, (float)pElite1->m_pTexture->m_Desc.Width, (float)pElite1->m_pTexture->m_Desc.Height });
	pElite1->SetPosition({ 755, 310 });
	pElite1->m_iStage = 1;
	m_pMapObjectList.push_back(pElite1);

	K_MapObject* pElite2 = new K_MapObject;
	pElite2->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map/elite.png", L"../../data/shader/MaskShape_Png_Black.txt");
	pElite2->SetMask(pEliteMask);
	pElite2->SetCamera(m_vCameraPos);
	pElite2->SetRect({ 0, 0, (float)pElite2->m_pTexture->m_Desc.Width, (float)pElite2->m_pTexture->m_Desc.Height });
	pElite2->SetPosition({ 1130, 390 });
	pElite2->m_iStage = 1;
	m_pMapObjectList.push_back(pElite2);


	// 덱 버튼
	K_Texture* pDeckViewButtonTexture = I_Tex.Load(L"../../data/img/card/DeckButton.png");
	m_pDeckViewButton = new K_Button;
	m_pDeckViewButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/DeckButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pDeckViewButton->SetMask(pDeckViewButtonTexture);
	m_pDeckViewButton->SetRect({ 0, 0, (float)m_pDeckViewButton->m_pTexture->m_Desc.Width, (float)m_pDeckViewButton->m_pTexture->m_Desc.Height });
	m_pDeckViewButton->m_fSize = 2.0f;
	m_pDeckViewButton->SetPosition({ 1530.0f, 50.0f });

	return true;
}

bool K_MapScene::Frame()
{
	m_pBGM->Play(1);

	if (I_Input.GetKey('W') == KEY_HOLD) m_vCameraPos.y -= 500.0f * g_fSecondPerFrame;
	if (I_Input.GetKey('S') == KEY_HOLD) m_vCameraPos.y += 500.0f * g_fSecondPerFrame;

	k_Vector2D vMousePosMove;
	vMousePosMove.x = I_Input.m_ptPos.x - m_vMousePos.x;
	vMousePosMove.y = I_Input.m_ptPos.y - m_vMousePos.y;
	m_vMousePos.x = I_Input.m_ptPos.x;
	m_vMousePos.y = I_Input.m_ptPos.y;

	if (m_bSceneStart == true) 
	{ 
		vMousePosMove = { 0,0 }; 
		m_bSceneStart = false;
	}

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH) m_bLButtonState = true;
	if (I_Input.GetKey(VK_LBUTTON) == KEY_UP) m_bLButtonState = false;
	if (m_bLButtonState == true) m_vCameraPos.y += vMousePosMove.y * -1.0f;

	for (auto obj : m_pBackGroundList) 
	{ 
		obj->SetCamera(m_vCameraPos);
		obj->SetPosition(obj->m_vPos);
		obj->Frame(); 
	}

	for (auto obj : m_pMapObjectList)
	{
		if (obj->m_iStage < m_iStage) { obj->m_bStageDone = true; }
		obj->SetCamera(m_vCameraPos);
		obj->SetPosition(obj->m_vPos);
		obj->Frame();
	}

	m_pDeckViewButton->Frame();

	return true;
}

bool K_MapScene::Render()
{
	//for (auto obj : m_pBackGroundList) { obj->Render(); }	
	//for (auto obj : m_pMapObjectList) { obj->Render(); }

	for (int i = m_pMapObjectList.size() - 1; i >= 0; i--) { m_pMapObjectList[i]->Render(); }
	for (int i = m_pBackGroundList.size() - 1; i >= 0; i--){ m_pBackGroundList[i]->Render(); }

	for (auto obj : m_pMapObjectList) 
	{
		if (obj->m_bButtonPushed && obj->m_iStage == m_iStage)
		{
			m_bSceneStart = true;
			if(m_iStage == 0) m_iSceneState = 1; // 1스테이지일때->일반전투
			else if (m_iStage == 1) m_iSceneState = 2; // 2스테이지일때->엘리트전투

			m_iStage++;
			m_pBGM->Stop();
			m_pSelectSound->PlayEffect();
		}
	}

	std::wstring PlayerHP = L"HP : ";
	PlayerHP += std::to_wstring(m_iPlayerCurrentHP);
	PlayerHP += L"/";
	PlayerHP += std::to_wstring(m_iPlayerMaxHP);
	I_Write.Draw(10, 10, PlayerHP, { 1,0,0,1 });

	m_pDeckViewButton->Render();
	if (m_pDeckViewButton->m_bButtonPushed == true) 
	{
		m_pDeckViewButton->m_bButtonPushed = false;
		m_bSceneStart = true;

		m_iSceneState = 3;
		m_pViewDeckSound->PlayEffect();
	}

	return true;
}

bool K_MapScene::Release()
{
	for (auto obj : m_pBackGroundList) { obj->Release(); }
	for (auto obj : m_pMapObjectList) { obj->Release(); }
	m_pDeckViewButton->Release();

	return true;
}
