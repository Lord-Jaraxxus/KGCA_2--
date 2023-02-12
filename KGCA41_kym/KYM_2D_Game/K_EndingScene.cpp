#include "K_EndingScene.h"

bool K_EndingScene::Init()
{
	m_pBGM = I_Sound.Load(L"../../data/sound/audio/using/bgm/Ending.mp3");

	m_pBackGround = new K_2dObject;
	m_pBackGround->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/ending/Ending.jpg", L"../KymGameCore/DefaultShape.txt");
	m_pBackGround->SetRect({ 0, 0, 728, 434 });
	m_pBackGround->SetPosition({ 0, 0 });
	m_pBackGround->SetDrawSize({2.0f, 2.0f});

	return true;
}

bool K_EndingScene::Frame()
{
	m_pBGM->Play(1);

	m_pBackGround->Frame();
	return true;
}

bool K_EndingScene::Render()
{
	m_pBackGround->Render();
	return true;
}

bool K_EndingScene::Release()
{
	m_pBackGround->Release();
	return true;
}
