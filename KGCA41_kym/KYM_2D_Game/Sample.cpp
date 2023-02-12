#include "Sample.h"

bool Sample::Init() 
{
	m_iPlayerMaxHP = 30;
	m_iPlayerCurrentHP = m_iPlayerMaxHP;

	m_pDeck = new K_Deck;

	I_Sound.Init();

	m_pTitleScene = new K_TitleScene;
	m_pTitleScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pTitleScene->Init();

	m_pMapScene = new K_MapScene;
	m_pMapScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pMapScene->Init();

	m_pBattleScene = new K_BattleScene;
	m_pBattleScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pBattleScene->Init();
	m_pBattleScene->SetDeck(m_pDeck);

	m_pEliteBattleScene = new K_EliteBattleScene;
	m_pEliteBattleScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pEliteBattleScene->Init();
	m_pEliteBattleScene->SetDeck(m_pDeck);

	m_pCardRewardScene = new K_CardRewardScene;
	m_pCardRewardScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pCardRewardScene->Init();
	m_pCardRewardScene->m_pDeck = m_pDeck;

	m_pDeckViewScene = new K_DeckViewScene;
	m_pDeckViewScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pDeckViewScene->Init();
	m_pDeckViewScene->m_pDeck = m_pDeck;

	m_pEndingScene = new K_EndingScene;
	m_pEndingScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pEndingScene->Init();

	m_pTestScene = new K_Scene;
	m_pTestScene->SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_pTestScene->Init();

	m_pCurrentScene = m_pTitleScene;
	return true; 
}

bool Sample::Frame()
{
	m_pCurrentScene->Frame();
	return true;
}

bool Sample::Render()
{
	m_pCurrentScene->Render();
	SceneChange();

	return true;
}

bool Sample::Release()
{
	m_pTitleScene->Release();
	delete m_pTitleScene;
	m_pMapScene->Release();
	delete m_pMapScene;
	m_pBattleScene->Release();
	delete m_pBattleScene;
	m_pCardRewardScene->Release();
	delete m_pCardRewardScene;

	return true; 
}

void Sample::SceneChange()
{
	if (m_pCurrentScene == m_pTitleScene && m_pCurrentScene->m_iSceneState == 1) // 타이틀->맵
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pMapScene;
		m_pMapScene->m_iPlayerCurrentHP = m_iPlayerCurrentHP;
		m_pMapScene->m_iPlayerMaxHP = m_iPlayerMaxHP;
	}

	if (m_pCurrentScene == m_pMapScene && m_pCurrentScene->m_iSceneState == 1) // 맵->전투
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pBattleScene;
		m_pBattleScene->SetPlayerHP(m_iPlayerCurrentHP, m_iPlayerMaxHP);
	}

	if (m_pCurrentScene == m_pMapScene && m_pCurrentScene->m_iSceneState == 2) // 맵->엘리트전투
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pEliteBattleScene;
		m_pEliteBattleScene->SetPlayerHP(m_iPlayerCurrentHP, m_iPlayerMaxHP);
	}

	if (m_pCurrentScene == m_pMapScene && m_pCurrentScene->m_iSceneState == 3) // 맵->덱
	{
		m_pCurrentScene->m_iSceneState = 0;

		m_pCurrentScene = m_pDeckViewScene;
		m_pDeckViewScene->m_iDeckViewState = 0;
	}

	if (m_pCurrentScene == m_pDeckViewScene && m_pCurrentScene->m_iSceneState == 1) // 덱->맵
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pMapScene;
	}

	if (m_pCurrentScene == m_pBattleScene && m_pCurrentScene->m_iSceneState == 1) // 전투->카드 보상
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_iPlayerCurrentHP = m_pBattleScene->m_iPlayerCurrentHP;
		m_pCurrentScene = m_pCardRewardScene;
	}

	if (m_pCurrentScene == m_pBattleScene && m_pCurrentScene->m_iSceneState == 2) // 전투->남은 덱
	{
		m_pCurrentScene->m_iSceneState = 0;

		m_pCurrentScene = m_pDeckViewScene;
		m_pDeckViewScene->m_iDeckViewState = 1;
	}

	if (m_pCurrentScene == m_pBattleScene && m_pCurrentScene->m_iSceneState == 3) // 전투->버린 덱
	{
		m_pCurrentScene->m_iSceneState = 0;

		m_pCurrentScene = m_pDeckViewScene;
		m_pDeckViewScene->m_iDeckViewState = 2;
	}

	if (m_pCurrentScene == m_pBattleScene && m_pCurrentScene->m_iSceneState == 4) // 전투->패배화면
	{
		m_pCurrentScene->m_iSceneState = 0;
		//m_pCurrentScene = m_pDefeatScene;
	}

	if (m_pCurrentScene == m_pDeckViewScene && m_pCurrentScene->m_iSceneState == 2) // 남은 카드 or 버린 카드 -> 전투
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pBattleScene;
	}

	if (m_pCurrentScene == m_pCardRewardScene && m_pCurrentScene->m_iSceneState == 1) // 카드 보상->맵
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pMapScene->m_iPlayerCurrentHP = m_iPlayerCurrentHP;
		m_pCurrentScene = m_pMapScene;
		for (auto obj : m_pDeck->m_DeckList) { m_pDeck->m_DiscardList.push_back(obj); }
		m_pDeck->Shuffle();
	}

	if (m_pCurrentScene == m_pEliteBattleScene && m_pCurrentScene->m_iSceneState == 1) // 엘리트전투->엔딩
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_iPlayerCurrentHP = m_pBattleScene->m_iPlayerCurrentHP;
		m_pCurrentScene = m_pEndingScene;
	}

	if (m_pCurrentScene == m_pEliteBattleScene && m_pCurrentScene->m_iSceneState == 2) // 엘리트전투->남은 덱
	{
		m_pCurrentScene->m_iSceneState = 0;

		m_pCurrentScene = m_pDeckViewScene;
		m_pDeckViewScene->m_iDeckViewState = 3;
	}

	if (m_pCurrentScene == m_pEliteBattleScene && m_pCurrentScene->m_iSceneState == 3) // 엘리트전투->버린 덱
	{
		m_pCurrentScene->m_iSceneState = 0;

		m_pCurrentScene = m_pDeckViewScene;
		m_pDeckViewScene->m_iDeckViewState = 4;
	}

	if (m_pCurrentScene == m_pDeckViewScene && m_pCurrentScene->m_iSceneState == 3) // 남은 카드 or 버린 카드 -> 엘리트전투
	{
		m_pCurrentScene->m_iSceneState = 0;
		m_pCurrentScene = m_pEliteBattleScene;
	}

	if (m_pCurrentScene == m_pEliteBattleScene && m_pCurrentScene->m_iSceneState == 4) // 엘리트전투->패배화면
	{
		m_pCurrentScene->m_iSceneState = 0;
		//m_pCurrentScene = m_pDefeatScene;
	}
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

GAME_RUN(2D Card Game, 1600, 900)
