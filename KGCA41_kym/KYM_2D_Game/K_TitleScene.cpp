#include "K_TitleScene.h"

bool K_TitleScene::Init()
{
	m_pBGM = I_Sound.Load(L"../../data/sound/audio/using/bgm/Title.ogg");

	K_BackGround* cloud = new K_BackGround;
	cloud->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/title.jpg", L"../KymGameCore/DefaultShape.txt");
	cloud->SetRect({ 0, 500, (float)g_rtClient.right, (float)g_rtClient.bottom });
	cloud->SetPosition({ 0, 0 });
	m_pBackGroundList.push_back(cloud);

	K_Texture* pTowerMaskTex = I_Tex.Load(L"../../data/img/title/title2.png");
	K_BackGround* tower = new K_BackGround;
	tower->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/title2.png", L"../KymGameCore/MaskShape_Png.txt");
	tower->SetMask(pTowerMaskTex);
	tower->SetRect({ 0, 860, 1920, 1118 });
	tower->SetPosition({ 0, 0 });
	m_pBackGroundList.push_back(tower);

	K_Texture* pCloudTexture = I_Tex.Load(L"../../data/img/title/title6.png");
	K_2dObject* pCloud1 = new K_2dObject;
	pCloud1->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/title6.png", L"../KymGameCore/MaskShape_Png.txt");
	pCloud1->SetMask(pCloudTexture);
	pCloud1->SetRect({ 0, 840, 910, 140 });
	pCloud1->SetPosition({ 100, 200 });
	m_pBackGroundList.push_back(pCloud1);

	K_2dObject* pCloud2 = new K_2dObject;
	pCloud2->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/title6.png", L"../KymGameCore/MaskShape_Png.txt");
	pCloud2->SetMask(pCloudTexture);
	pCloud2->SetRect({ 0, 840, 910, 140 });
	pCloud2->SetPosition({ 600, 700 });
	m_pBackGroundList.push_back(pCloud2);

	K_2dObject* pCloud3 = new K_2dObject;
	pCloud3->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/title6.png", L"../KymGameCore/MaskShape_Png.txt");
	pCloud3->SetMask(pCloudTexture);
	pCloud3->SetRect({ 0, 600, 1400, 200 });
	pCloud3->SetPosition({ 200, 500 });
	m_pBackGroundList.push_back(pCloud3);

	K_Texture* pButtonMask = I_Tex.Load(L"../../data/img/title/TitleButton.png");
	m_pStartButton = new K_Button;
	m_pStartButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/TitleButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pStartButton->SetMask(pButtonMask);
	m_pStartButton->SetRect({ 0, 0, 180, 50 });
	m_pStartButton->SetPosition({ 800, 450 });
	m_pBackGroundList.push_back(m_pStartButton);

	m_pSettingButton = new K_Button;
	m_pSettingButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/TitleButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pSettingButton->SetMask(pButtonMask);
	m_pSettingButton->SetRect({ 0, 50, 180, 52 });
	m_pSettingButton->SetPosition({ 800, 550 });
	m_pBackGroundList.push_back(m_pSettingButton);

	m_pEndButton = new K_Button;
	m_pEndButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/title/TitleButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pEndButton->SetMask(pButtonMask);
	m_pEndButton->SetRect({ 0, 102, 180, 52 });
	m_pEndButton->SetPosition({ 800, 650 });
	m_pBackGroundList.push_back(m_pEndButton);

	return true;
}

bool K_TitleScene::Frame()
{
	m_pBGM->Play(1);

	m_pBackGroundList[2]->m_vPos.x += 100.0f * g_fSecondPerFrame;
	if (m_pBackGroundList[2]->m_vPos.x > 1700.0f) { m_pBackGroundList[2]->m_vPos.x = -1000.0f; }
	m_pBackGroundList[2]->SetPosition(m_pBackGroundList[2]->m_vPos);

	m_pBackGroundList[3]->m_vPos.x -= 120.0f * g_fSecondPerFrame;
	if (m_pBackGroundList[3]->m_vPos.x < -1000.0f) { m_pBackGroundList[3]->m_vPos.x = 1700.0f; }
	m_pBackGroundList[3]->SetPosition(m_pBackGroundList[3]->m_vPos);

	m_pBackGroundList[4]->m_vPos.x -= 200.0f * g_fSecondPerFrame;
	if (m_pBackGroundList[4]->m_vPos.x < -1400.0f) { m_pBackGroundList[4]->m_vPos.x = 1700.0f; }
	m_pBackGroundList[4]->SetPosition(m_pBackGroundList[4]->m_vPos);

	for (auto obj : m_pBackGroundList) { obj->Frame(); }
	if (m_pStartButton->m_bButtonPushed == true) 
	{
		m_pStartButton->m_bButtonPushed = false;
		m_iSceneState = 1;
		m_pBGM->Stop();
	}
	if (m_pEndButton->m_bButtonPushed == true) m_iSceneState = 2;
	return true;
}

bool K_TitleScene::Render()
{
	 // for (auto obj : m_pBackGroundList) { obj->Render(); }	// 대체 왜 렌더링 순서가 거꾸로가 된거야???

	for (int i = m_pBackGroundList.size() -1; i >= 0; i--)
	{ 
		m_pBackGroundList[i]->Render();
	}

	//std::wstring MousePos = std::to_wstring(I_Input.m_ptPos.x);
	//MousePos += L" ";
	//MousePos += std::to_wstring(I_Input.m_ptPos.y);
	//MousePos += L"\n";
	//I_Write.Draw(1200, 450, MousePos, { 1,0,0,1 });

	return true;
}

bool K_TitleScene::Release()
{
	for (auto obj : m_pBackGroundList) { obj->Release(); }
	return true;
}
