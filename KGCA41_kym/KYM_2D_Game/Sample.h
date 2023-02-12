#pragma once
#pragma comment(lib, "KymGameCore_2D_d.lib")

#include "K_Scene.h"
#include "K_TitleScene.h"
#include "K_MapScene.h"
#include "K_BattleScene.h"
#include "K_EliteBattleScene.h"
#include "K_CardRewardScene.h"
#include "K_DeckViewScene.h"
#include "K_DeathScene.h"
#include "K_EndingScene.h"

#include "K_Deck.h"

#include "K_SoundManager.h"

class Sample : public K_GameCore
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	K_Scene*			m_pCurrentScene;
	K_TitleScene*		m_pTitleScene;
	K_MapScene*			m_pMapScene;
	K_BattleScene*		m_pBattleScene;
	K_EliteBattleScene* m_pEliteBattleScene;
	K_CardRewardScene*	m_pCardRewardScene;
	K_DeckViewScene*	m_pDeckViewScene;
	K_EndingScene*		m_pEndingScene;
	K_Scene*			m_pTestScene;

	K_Deck*				m_pDeck;

public:
	void			SceneChange();

public:
	int				m_iPlayerMaxHP;
	int				m_iPlayerCurrentHP;
}; 

