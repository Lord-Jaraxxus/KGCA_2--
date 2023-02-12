#include "K_BattleScene.h"

bool K_BattleScene::Init()
{
	m_pBGM = I_Sound.Load(L"../../data/sound/audio/using/bgm/Battle.ogg");

	K_Sound* pSound1 = I_Sound.Load(L"../../data/sound/audio/using/Attack_Strike.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Attack_Strike", pSound1));
	K_Sound* pSound2 = I_Sound.Load(L"../../data/sound/audio/using/Attack_PommelStrike.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Attack_PommelStrike", pSound2));
	K_Sound* pSound3 = I_Sound.Load(L"../../data/sound/audio/using/Attack_IronWave.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Attack_IronWave", pSound3));
	K_Sound* pSound4 = I_Sound.Load(L"../../data/sound/audio/using/Attack_Bludgeon.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Attack_Bludgeon", pSound4));
	K_Sound* pSound5 = I_Sound.Load(L"../../data/sound/audio/using/Shield.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Shield", pSound5));
	K_Sound* pSound6 = I_Sound.Load(L"../../data/sound/audio/using/Block.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Block", pSound6));
	K_Sound* pSound7 = I_Sound.Load(L"../../data/sound/audio/using/Heal.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Heal", pSound7));
	K_Sound* pSound8 = I_Sound.Load(L"../../data/sound/audio/using/CardUsed.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"CardUsed", pSound8));
	K_Sound* pSound9 = I_Sound.Load(L"../../data/sound/audio/using/EnemyTurn.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"EnemyTurn", pSound9));
	K_Sound* pSound10 = I_Sound.Load(L"../../data/sound/audio/using/EnemyDefeat.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"EnemyDefeat", pSound10));
	K_Sound* pSound11 = I_Sound.Load(L"../../data/sound/audio/using/PlayerTurn.ogg"); 
	m_pEffectSoundList.insert(std::make_pair(L"PlayerTurn", pSound11));
	K_Sound* pSound12 = I_Sound.Load(L"../../data/sound/audio/using/TurnEnd.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"TurnEnd", pSound12));
	K_Sound* pSound13 = I_Sound.Load(L"../../data/sound/audio/using/DeckView.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"DeckView", pSound13));
	K_Sound* pSound14 = I_Sound.Load(L"../../data/sound/audio/using/Hit.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"Hit", pSound14));
	K_Sound* pSound15 = I_Sound.Load(L"../../data/sound/audio/using/CardReward.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"CardReward", pSound15));
	K_Sound* pSound16 = I_Sound.Load(L"../../data/sound/audio/using/EnemyDefeat.ogg");
	m_pEffectSoundList.insert(std::make_pair(L"EnemyDefeat", pSound16));
	
	m_iEnemyMaxHP = 15;
	m_iEnemyCurrentHP = m_iEnemyMaxHP;

	// 배경
	m_pBackGround = new K_BackGround;
	m_pBackGround->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/background.jpg", L"../KymGameCore/DefaultShape.txt");
	m_pBackGround->SetRect({ 0, 0, 1920, 1140 });
	m_pBackGround->SetPosition({ 0, 0 });


	// 플레이어 캐릭터
	K_Texture* pPlayerTexture1 = I_Tex.Load(L"../../data/img/battle/Player_Stand.png");
	K_Texture* pPlayerTexture2 = I_Tex.Load(L"../../data/img/battle/Player_Shield.png");
	K_Texture* pPlayerTexture3 = I_Tex.Load(L"../../data/img/battle/Player_Attack1.png");
	K_Texture* pPlayerTexture4 = I_Tex.Load(L"../../data/img/battle/Player_Attack2.png");
	K_Texture* pPlayerTexture5 = I_Tex.Load(L"../../data/img/battle/Player_Attack3.png");
	K_Texture* pPlayerTexture6 = I_Tex.Load(L"../../data/img/battle/Player_Hit.png");
	K_Texture* pPlayerTexture7 = I_Tex.Load(L"../../data/img/battle/Player_Block.png");
	K_Texture* pPlayerTexture8 = I_Tex.Load(L"../../data/img/battle/Player_Retreat.png");

	m_pPlayer = new K_Character;
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture1);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture2);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture3);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture4);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture5);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture6);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture7);
	m_pPlayer->m_pCharacterTextureList.push_back(pPlayerTexture8);

	m_pPlayer->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/Player_Stand.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pPlayer->SetMask(pPlayerTexture1);
	m_pPlayer->SetRect({ 0, 0, (float)m_pPlayer->m_pTexture->m_Desc.Width, (float)m_pPlayer->m_pTexture->m_Desc.Height });
	m_pPlayer->SetPosition({ 200, 700 });
	m_pPlayer->m_vPosOrgin = {200, 700};
	m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom});
	m_pPlayer->SetDrawPos();


	// 적 캐릭터
	K_Texture* pEnemyTexture = I_Tex.Load(L"../../data/img/enemy/GremlinLeader.png");
	K_Texture* pEnemyDefeatTexture = I_Tex.Load(L"../../data/img/enemy/GremlinLeaderDefeat.png");
	K_Texture* pEnemyHitTexture = I_Tex.Load(L"../../data/img/enemy/GremlinLeaderHit.png");

	m_pEnemy = new K_Character;
	m_pEnemy->m_pCharacterTextureList.push_back(pEnemyTexture);
	m_pEnemy->m_pCharacterTextureList.push_back(pEnemyDefeatTexture);
	m_pEnemy->m_pCharacterTextureList.push_back(pEnemyHitTexture);

	m_pEnemy->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/enemy/GremlinLeader.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pEnemy->SetMask(pEnemyTexture);
	m_pEnemy->SetRect({ 0, 0, (float)m_pEnemy->m_pTexture->m_Desc.Width, (float)m_pEnemy->m_pTexture->m_Desc.Height });
	m_pEnemy->SetPosition({ 1200, 700 });
	m_pEnemy->m_vPosOrgin = { 1200, 700 };
	m_pEnemy->SetDrawPos();


	// 턴 종료 버튼
	K_Texture* pTurnEndButtonMask = I_Tex.Load(L"../../data/img/battle/TurnEndButton.png");
	m_pTurnEndButton = new K_Button;
	m_pTurnEndButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/TurnEndButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pTurnEndButton->SetMask(pTurnEndButtonMask);
	m_pTurnEndButton->SetRect({ 0, 0, 180, 70 });
	m_pTurnEndButton->SetPosition({ 800, 300 });


	// 카드
	K_Texture* pTextureCard0 = I_Tex.Load(L"../../data/img/card/0_Strike.png");
	K_Texture* pTextureCard1 = I_Tex.Load(L"../../data/img/card/1_Defend.png");
	K_Texture* pTextureCard2 = I_Tex.Load(L"../../data/img/card/2_PommelStrike.png");
	K_Texture* pTextureCard3 = I_Tex.Load(L"../../data/img/card/3_ShrugItOff.png");
	K_Texture* pTextureCard4 = I_Tex.Load(L"../../data/img/card/4_Hemokinesis.png");
	K_Texture* pTextureCard5 = I_Tex.Load(L"../../data/img/card/5_Bludgeon.png");
	K_Texture* pTextureCard6 = I_Tex.Load(L"../../data/img/card/6_IronWave.png");
	m_pCardTextureList.push_back(pTextureCard0);
	m_pCardTextureList.push_back(pTextureCard1);
	m_pCardTextureList.push_back(pTextureCard2);
	m_pCardTextureList.push_back(pTextureCard3);
	m_pCardTextureList.push_back(pTextureCard4);
	m_pCardTextureList.push_back(pTextureCard5);
	m_pCardTextureList.push_back(pTextureCard6);

	K_Texture* pCardMask = I_Tex.Load(L"../../data/img/card/0_Strike.png");
	K_Card* Card1 = new K_Card;
	Card1->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/0_Strike.png", L"../KymGameCore/MaskShape_Png.txt");
	Card1->SetMask(pCardMask);
	Card1->SetRect({ 0, 0, (float)Card1->m_pTexture->m_Desc.Width, (float)Card1->m_pTexture->m_Desc.Height });
	Card1->SetPosition({ 500, 700 });
	Card1->m_vPosOrigin = {500, 700};
	Card1->m_iCardNum = 0;
	m_pCardList.push_back(Card1);

	K_Card* Card2 = new K_Card;
	Card2->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/0_Strike.png", L"../KymGameCore/MaskShape_Png.txt");
	Card2->SetMask(pCardMask);
	Card2->SetRect({ 0, 0, (float)Card2->m_pTexture->m_Desc.Width, (float)Card2->m_pTexture->m_Desc.Height });
	Card2->SetPosition({ 700, 700 });
	Card2->m_vPosOrigin = { 700, 700 };
	Card2->m_iCardNum = 0;
	m_pCardList.push_back(Card2);

	K_Card* Card3 = new K_Card;
	Card3->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/0_Strike.png", L"../KymGameCore/MaskShape_Png.txt");
	Card3->SetMask(pCardMask);
	Card3->SetRect({ 0, 0, (float)Card3->m_pTexture->m_Desc.Width, (float)Card3->m_pTexture->m_Desc.Height });
	Card3->SetPosition({ 900, 700 });
	Card3->m_vPosOrigin = { 900, 700 };
	Card3->m_iCardNum = 0;
	m_pCardList.push_back(Card3);

	K_Card* Card4 = new K_Card;
	Card4->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/0_Strike.png", L"../KymGameCore/MaskShape_Png.txt");
	Card4->SetMask(pCardMask);
	Card4->SetRect({ 0, 0, (float)Card4->m_pTexture->m_Desc.Width, (float)Card4->m_pTexture->m_Desc.Height });
	Card4->SetPosition({ 1100, 700 });
	Card4->m_vPosOrigin = { 1100, 700 };
	Card4->m_iCardNum = 0;
	m_pCardList.push_back(Card4);


	// 적 의도
	K_Texture* pTextureEnemyIntent1 = I_Tex.Load(L"../../data/img/intent/attack_intent_1.png");
	K_Texture* pTextureEnemyIntent2 = I_Tex.Load(L"../../data/img/intent/attack_intent_2.png");
	K_Texture* pTextureEnemyIntent3 = I_Tex.Load(L"../../data/img/intent/attack_intent_3.png");
	K_Texture* pTextureEnemyIntent4 = I_Tex.Load(L"../../data/img/intent/attack_intent_4.png");
	K_Texture* pTextureEnemyIntent5 = I_Tex.Load(L"../../data/img/intent/attack_intent_5.png");
	K_Texture* pTextureEnemyIntent6 = I_Tex.Load(L"../../data/img/intent/attack_intent_6.png");
	K_Texture* pTextureEnemyIntent7 = I_Tex.Load(L"../../data/img/intent/attack_intent_7.png");
	K_Texture* pTextureEnemyIntent8 = I_Tex.Load(L"../../data/img/intent/heal_intent_1.png");

	m_pEnemyIntent = new K_EnemyIntent;
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent1);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent2);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent3);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent4);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent5);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent6);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent7);
	m_pEnemyIntent->m_pEnemyIntentTextureList.push_back(pTextureEnemyIntent8);

	m_pEnemyIntent->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/intent/attack_intent_3.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pEnemyIntent->SetMask(pTextureEnemyIntent3);
	m_pEnemyIntent->SetRect({ 0, 0, (float)m_pEnemyIntent->m_pTexture->m_Desc.Width, (float)m_pEnemyIntent->m_pTexture->m_Desc.Height });
	m_pEnemyIntent->SetPosition({ 1220, 300 });


	// 이펙트, 플레이어 몬스터 순
	K_Texture* pTextureEffect1 = I_Tex.Load(L"../../data/img/battle/vfx.png");
	K_Texture* pTextureEffect2 = I_Tex.Load(L"../../data/img/battle/shield.png");
	K_Texture* pTextureEffect3 = I_Tex.Load(L"../../data/img/battle/heal_effect.png");

	m_pPlayerEffect = new K_EnemyIntent;
	m_pPlayerEffect->m_pEnemyIntentTextureList.push_back(pTextureEffect1);
	m_pPlayerEffect->m_pEnemyIntentTextureList.push_back(pTextureEffect2);

	m_pPlayerEffect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/vfx.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pPlayerEffect->SetMask(pTextureEffect1);
	m_pPlayerEffect->SetRect({ 1270, 290, 220, 170 });
	m_pPlayerEffect->SetPosition({ 200, 400 });

	m_pEnemyEffect = new K_EnemyIntent;
	m_pEnemyEffect->m_pEnemyIntentTextureList.push_back(pTextureEffect1);
	m_pEnemyEffect->m_pEnemyIntentTextureList.push_back(pTextureEffect2);
	m_pEnemyEffect->m_pEnemyIntentTextureList.push_back(pTextureEffect3);

	m_pEnemyEffect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/vfx.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pEnemyEffect->SetMask(pTextureEffect1);
	m_pEnemyEffect->SetRect({ 1270, 290, 220, 170 });
	m_pEnemyEffect->SetPosition({ 1200, 500 });


	// 남은 카드, 버린 카드 버튼
	K_Texture* pRemainViewButtonTexture = I_Tex.Load(L"../../data/img/card/DeckButton.png");
	m_pRemainViewButton = new K_Button;
	m_pRemainViewButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/DeckButton.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pRemainViewButton->SetMask(pRemainViewButtonTexture);
	m_pRemainViewButton->SetRect({ 0, 0, (float)m_pRemainViewButton->m_pTexture->m_Desc.Width, (float)m_pRemainViewButton->m_pTexture->m_Desc.Height });
	m_pRemainViewButton->m_fSize = 2.0f;
	m_pRemainViewButton->SetPosition({ 100.0f, 50.0f });

	K_Texture* pDiscardViewButtonTexture = I_Tex.Load(L"../../data/img/card/Discard.png");
	m_pDiscardViewButton = new K_Button;
	m_pDiscardViewButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/Discard.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pDiscardViewButton->SetMask(pDiscardViewButtonTexture);
	m_pDiscardViewButton->SetRect({ 0, 0, (float)m_pDiscardViewButton->m_pTexture->m_Desc.Width, (float)m_pDiscardViewButton->m_pTexture->m_Desc.Height });
	m_pDiscardViewButton->m_fSize = 2.0f;
	m_pDiscardViewButton->SetPosition({ 1500.0f, 50.0f });


	// 마나, 방어도 이미지
	K_Texture* pManaTexture = I_Tex.Load(L"../../data/img/battle/Mana.png");
	m_pManaImage = new K_2dObject;
	m_pManaImage->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/Mana.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pManaImage->SetMask(pManaTexture);
	m_pManaImage->SetRect({ 0, 0, (float)m_pManaImage->m_pTexture->m_Desc.Width, (float)m_pManaImage->m_pTexture->m_Desc.Height });
	m_pManaImage->SetPosition({ 220.0f, 300.0f });
	m_pManaImage->SetDrawSize({0.09f, 0.16f});

	K_Texture* pShieldTetxture = I_Tex.Load(L"../../data/img/battle/block.png");
	m_pShieldImage = new K_2dObject;
	m_pShieldImage->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/battle/block.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pShieldImage->SetMask(pShieldTetxture);
	m_pShieldImage->SetRect({ 0, 0, (float)m_pShieldImage->m_pTexture->m_Desc.Width, (float)m_pShieldImage->m_pTexture->m_Desc.Height });
	m_pShieldImage->SetPosition({ 220.0f, 740.0f });
	return true;


	return true;
} 

bool K_BattleScene::Frame()
{
	m_pBGM->Play(1);

	m_pBackGround->Frame();
	m_pPlayer->Frame();
	CharacterActionProcessing();
	m_pEnemy->Frame();
	m_pTurnEndButton->Frame();
	m_pPlayerEffect->Frame(); 
	m_pEnemyEffect->Frame();
	EffectProcessing();
	CameraShake();
	m_pRemainViewButton->Frame();
	m_pDiscardViewButton->Frame();
	m_pManaImage->Frame();
	m_pShieldImage->Frame();

	for (auto obj : m_pCardList) { obj->Frame(); }

	if (m_pTurnEndButton->m_bButtonPushed == true)
	{
		m_pEffectSoundList.find(L"TurnEnd")->second->PlayEffect();
		m_pDeck->TurnEnd();
		m_iTurn++;
		m_pTurnEndButton->m_bButtonPushed = false;
		for (auto obj : m_pCardList) 
		{
			obj->m_bUsed = true;
			obj->m_bAvailable = false;
		}
	}

	Combat(m_iTurn);

	m_pEnemyIntent->Frame();

	return true;
}

bool K_BattleScene::Render()
{
	m_pBackGround->Render();
	m_pPlayer->Render();
	m_pEnemy->Render();
	if (m_bEnemyTurn == false) { m_pTurnEndButton->Render(); }
	m_pEnemyIntent->Render();
	if (m_pPlayerEffect->m_bEffectOn == true) { m_pPlayerEffect->Render(); }
	if (m_pEnemyEffect->m_bEffectOn == true) { m_pEnemyEffect->Render(); }
	m_pRemainViewButton->Render();
	m_pDiscardViewButton->Render();
	m_pManaImage->Render();
	m_pShieldImage->Render();

	for (auto obj : m_pCardList) 
	{ 
		if (obj->m_bUsed == false) { obj->Render(); }
	}

	std::wstring PlayerHP = std::to_wstring(m_iPlayerCurrentHP);
	PlayerHP += L"/";
	PlayerHP += std::to_wstring(m_iPlayerMaxHP);
	I_Write.Draw(250, 700, PlayerHP, { 1,0,0,1 });

	std::wstring PlayerShield =  std::to_wstring(m_iPlayerShield);
	I_Write.Draw(280, 750, PlayerShield, { 0.8f, 0.8f, 0.8f, 1 });

	std::wstring EnemyHP = std::to_wstring(m_iEnemyCurrentHP);
	EnemyHP += L"/";
	EnemyHP += std::to_wstring(m_iEnemyMaxHP);
	I_Write.Draw(1300, 700, EnemyHP, { 1,0,0,1 });

	std::wstring EnemyIntentPower = std::to_wstring(m_iEnemyIntentPower);
	I_Write.Draw(1320, 340, EnemyIntentPower, { 1,1,1,1 });

	std::wstring Mana = std::to_wstring(m_iCurrentMana);
	Mana += L"/";
	Mana += std::to_wstring(m_iMaxMana);
	I_Write.Draw(300, 315, Mana, { 1,0.4,0.2,1 });

	if (m_bNoMana == true) 
	{
		m_fWarningTimer += g_fSecondPerFrame;
		std::wstring ManaWarning = L"마나가 부족합니다!";
		I_Write.Draw(660, 200, ManaWarning, { 1,0,0.5,1 });

		if (m_fWarningTimer > 1.0f) 
		{
			m_bNoMana = false;
			m_fWarningTimer = 0.0f;
		}
	}

	return true;
}

bool K_BattleScene::Release()
{
	m_pBackGround->Release();
	m_pPlayer->Release();
	m_pEnemy->Release();
	m_pTurnEndButton->Release();
	m_pEnemyIntent->Release();
	m_pPlayerEffect->Release();
	m_pEnemyEffect->Release();
	m_pRemainViewButton->Release();
	m_pDiscardViewButton->Release();
	m_pManaImage->Release();
	m_pShieldImage->Release();
	for (auto obj : m_pCardList) { obj->Release(); }

	return true;
}





void K_BattleScene::SetDeck(K_Deck* deck)
{
	m_pDeck = deck;
}

void K_BattleScene::SetPlayerHP(int currentHP, int maxHP)
{
	m_iPlayerCurrentHP = currentHP;
	m_iPlayerMaxHP = maxHP;
}

void K_BattleScene::Combat(int turn)
{
	if (m_bTurnStart == true) 
	{
		EnemyTurnProcessing();
		if(m_bEnemyTurn == false)
		{
			for (auto obj : m_pCardList)
			{
				obj->m_bUsed = false;
				obj->m_bAvailable = true;
			}

			m_pDeck->Draw(4);
			m_bTurnStart = false;
			CardSetting();
			m_iCurrentMana = m_iMaxMana;
			m_iPlayerShield = 0;
		}
	}

	if (turn % 2 == 0) 
	{
		for (int i = 0; i < m_pCardList.size(); i++)
		{ 
			if (m_pCardList[i]->m_bUsed == true && m_pCardList[i]->m_bAvailable == true)
			{
				if (m_iCurrentMana < m_pCardList[i]->m_iCardMana)
				{
					m_pCardList[i]->m_bUsed = false;
					m_bNoMana = true;
				}
				else 
				{
					m_iCurrentMana -= m_pCardList[i]->m_iCardMana;
					m_pCardList[i]->m_bAvailable = false;
					m_pDeck->m_DiscardList.push_back(m_pCardList[i]->m_iCardNum);
					m_pDeck->m_HandList[i] = 99;
					CardUsed(m_pCardList[i]->m_iCardNum);
					m_pEffectSoundList.find(L"CardUsed")->second->PlayEffect();
				}
			}
		}
		if (m_iTurn % 3 == 0 && m_bEnemyTurn == false)
		{
			m_iEnemyIntentPower = 7;
			m_pEnemyIntent->m_pTexture = m_pEnemyIntent->m_pEnemyIntentTextureList[2];
			m_pEnemyIntent->SetMask(m_pEnemyIntent->m_pEnemyIntentTextureList[2]);
		}
		else if (m_iTurn % 3 == 1 && m_bEnemyTurn == false)
		{
			m_iEnemyIntentPower = 13;
			m_pEnemyIntent->m_pTexture = m_pEnemyIntent->m_pEnemyIntentTextureList[5];
			m_pEnemyIntent->SetMask(m_pEnemyIntent->m_pEnemyIntentTextureList[5]);
		}
		else if (m_iTurn % 3 == 2 && m_bEnemyTurn == false)
		{
			m_iEnemyIntentPower = 5;
			m_pEnemyIntent->m_pTexture = m_pEnemyIntent->m_pEnemyIntentTextureList[7];
			m_pEnemyIntent->SetMask(m_pEnemyIntent->m_pEnemyIntentTextureList[7]);
		}
	}
	else 
	{
		EnemyTurn(m_iTurn);
		m_bEnemyTurn = true;
		m_bTurnStart = true;
	}

	if (m_iEnemyCurrentHP <= 0) // 적 체력이 0 이하로 떨어졌을 때
	{
		EnemyDefeat();
		m_iEnemyCurrentHP = 0;

		if (m_fEnemyTurnTimer > 3.0f)
		{
			m_iSceneState = 1;

			m_pBGM->Stop();
			m_pEffectSoundList.find(L"CardReward")->second->PlayEffect();

			m_iEnemyCurrentHP = m_iEnemyMaxHP;
			m_iCurrentMana = m_iMaxMana;

			m_pDeck->m_RemainingCardList.clear();
			m_pDeck->m_HandList.clear();
			m_pDeck->m_DiscardList.clear();
		}
	}

	if (m_pRemainViewButton->m_bButtonPushed == true) 
	{
		m_iSceneState = 2;
		m_pEffectSoundList.find(L"DeckView")->second->PlayEffect();
		m_pRemainViewButton->m_bButtonPushed = false;
	}

	if (m_pDiscardViewButton->m_bButtonPushed == true)
	{
		m_iSceneState = 3;
		m_pEffectSoundList.find(L"DeckView")->second->PlayEffect();
		m_pDiscardViewButton->m_bButtonPushed = false;
	}

}

void K_BattleScene::CardUsed(int card_num)
{
	switch (card_num)
	{
	case Strike:
		m_iEnemyCurrentHP -= 6;

		m_iPlayerState = 1;

		m_pEnemyEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[0];
		m_pEnemyEffect->SetMask(m_pEnemyEffect->m_pTexture);
		m_pEnemyEffect->SetRect({ 1270, 290, 220, 170 });
		m_pEnemyEffect->SetPosition({ 1200, 500 });
		break;

	case Defend:
		m_iPlayerShield += 5;

		m_iPlayerState = 2;

		m_pPlayerEffect->m_bEffectOn = true;
		m_pPlayerEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[1];
		m_pPlayerEffect->SetMask(m_pPlayerEffect->m_pTexture); 
		m_pPlayerEffect->SetRect({ 0, 0, 200, 200 });
		m_pPlayerEffect->SetPosition({ 200, 440 });
		break;

	case PommelStrike:
		m_iEnemyCurrentHP -= 9;

		m_iPlayerState = 3;

		m_pEnemyEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[0];
		m_pEnemyEffect->SetMask(m_pEnemyEffect->m_pTexture);
		m_pEnemyEffect->SetRect({ 1270, 290, 220, 170 });
		m_pEnemyEffect->SetPosition({ 1200, 500 });

		m_pDeck->Draw(1);
		for (int i = 0; i < 4; i++)
		{
			if (m_pCardList[i]->m_bAvailable == false) 
			{
				m_pCardList[i]->m_iCardNum = m_pDeck->m_HandList.back();
				m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pCardList[i]->m_iCardNum];
				if (m_pCardList[i]->m_iCardNum == Bludgeon) { m_pCardList[i]->m_iCardMana = 3; }
				m_pCardList[i]->m_bUsed = false;
				m_pCardList[i]->m_bAvailable = true;
				m_pCardList[i]->m_vPos = m_pCardList[i]->m_vPosOrigin;
				m_pCardList[i]->m_iDragFlag = 0;
				break;
			}
		}
		break;

	case ShrugItOff: // ㅅㅂ 왜 얘만쓰면 카드가 복사가 되냐고...
		m_iPlayerShield += 8;

		m_iPlayerState = 2;

		m_pPlayerEffect->m_bEffectOn = true;
		m_pPlayerEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[1];
		m_pPlayerEffect->SetMask(m_pPlayerEffect->m_pTexture);
		m_pPlayerEffect->SetRect({ 0, 0, 200, 200 });
		m_pPlayerEffect->SetPosition({ 200, 440 });

		m_pDeck->Draw(1);
		for (int i = 0; i < 4; i++)
		{
			if (m_pCardList[i]->m_bAvailable == false)
			{
				m_pCardList[i]->m_iCardNum = m_pDeck->m_HandList.back();
				m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pCardList[i]->m_iCardNum];
				if (m_pCardList[i]->m_iCardNum == Bludgeon) { m_pCardList[i]->m_iCardMana = 3; }
				m_pCardList[i]->m_bUsed = false;
				m_pCardList[i]->m_bAvailable = true;
				m_pCardList[i]->m_vPos = m_pCardList[i]->m_vPosOrigin;
				m_pCardList[i]->m_iDragFlag = 0;
				break;
			}
		}
		break;

	case IronWave:
		m_iEnemyCurrentHP -= 5;
		m_iPlayerShield += 5;
		m_iPlayerState = 5;

		m_pEnemyEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[0];
		m_pEnemyEffect->SetMask(m_pEnemyEffect->m_pTexture);
		m_pEnemyEffect->SetRect({ 1270, 290, 220, 170 });
		m_pEnemyEffect->SetPosition({ 1200, 500 });
		break;

	case Bludgeon:
		m_iEnemyCurrentHP -= 32;

		m_iPlayerState = 4;

		m_pEnemyEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[0]; // 이펙트 다른걸로 (짱쎄보이는거) 바꿀만할지도?
		m_pEnemyEffect->SetMask(m_pEnemyEffect->m_pTexture);
		m_pEnemyEffect->SetRect({ 1270, 290, 220, 170 });
		m_pEnemyEffect->SetPosition({ 1200, 500 });
		break;
	}
}

void K_BattleScene::EnemyTurn(int turn)
{
	m_iTurn++;

	switch (turn % 3)
	{
	case 0:
		m_iEnemyCurrentHP += 5;
		if (m_iEnemyCurrentHP > m_iEnemyMaxHP) m_iEnemyCurrentHP = m_iEnemyMaxHP;

		m_iEnemyStae = 2;

		m_pEnemyEffect->m_pTexture = m_pEnemyEffect->m_pEnemyIntentTextureList[2]; 
		m_pEnemyEffect->SetMask(m_pEnemyEffect->m_pTexture);
		m_pEnemyEffect->SetRect({ 0, 0, 128, 128 });
		m_pEnemyEffect->SetPosition({ 1250, 500 });
		break;

	case 1:
		m_iPlayerShield -= 7;

		m_iEnemyStae = 1;

		if (m_iPlayerShield < 0)
		{
			m_iPlayerCurrentHP += m_iPlayerShield;
			m_iPlayerShield = 0;
		}


		m_pPlayerEffect->m_pTexture = m_pPlayerEffect->m_pEnemyIntentTextureList[0];
		m_pPlayerEffect->SetMask(m_pPlayerEffect->m_pTexture);
		m_pPlayerEffect->SetRect({ 1270, 290, 220, 170 });
		m_pPlayerEffect->SetPosition({ 200, 400 });
		break;

	case 2:
		m_iPlayerShield -= 13;

		m_iEnemyStae = 1;

		if (m_iPlayerShield < 0)
		{
			m_iPlayerCurrentHP += m_iPlayerShield;
			m_iPlayerShield = 0;
		}

		m_pPlayerEffect->m_pTexture = m_pPlayerEffect->m_pEnemyIntentTextureList[0];
		m_pPlayerEffect->SetMask(m_pPlayerEffect->m_pTexture);
		m_pPlayerEffect->SetRect({ 1270, 290, 220, 170 });
		m_pPlayerEffect->SetPosition({ 200, 400 });
		break;
	}
}

void K_BattleScene::CardSetting()
{
	for (int i = 0; i < m_pCardList.size(); i++)
	{
		m_pCardList[i]->m_iCardNum = m_pDeck->m_HandList[i]; 

		if (m_pCardList[i]->m_iCardNum != 99) 
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pCardList[i]->m_iCardNum];
			if (m_pCardList[i]->m_iCardNum == 5) { m_pCardList[i]->m_iCardMana = 3; }
			else { m_pCardList[i]->m_iCardMana = 1; }
		}
	}
}

void K_BattleScene::EffectProcessing()
{
	if (m_pPlayerEffect->m_bEffectOn == true)
	{
		m_pPlayerEffect->m_fEffectTimer += g_fSecondPerFrame;
		if (m_pPlayerEffect->m_fEffectTimer > 0.5f)
		{
			m_pPlayerEffect->m_fEffectTimer = 0.0f;
			m_pPlayerEffect->m_bEffectOn = false;
		}
	}

	if (m_pEnemyEffect->m_bEffectOn == true)
	{
		m_pEnemyEffect->m_fEffectTimer += g_fSecondPerFrame;

		if(m_bEnemyTurn == false)
		{
			m_pEnemy->m_pTexture = m_pEnemy->m_pCharacterTextureList[2];
			m_pEnemy->SetMask(m_pEnemy->m_pTexture);
		}

		if (m_pEnemyEffect->m_fEffectTimer > 0.5f)
		{
			m_pEnemyEffect->m_fEffectTimer = 0.0f;
			m_pEnemy->m_pTexture = m_pEnemy->m_pCharacterTextureList[0];
			m_pEnemy->SetMask(m_pEnemy->m_pTexture);
			m_pEnemyEffect->m_bEffectOn = false;
		}
	}
}

void K_BattleScene::EnemyTurnProcessing()
{
	if (m_bEnemyTurn == true)
	{
		m_fEnemyTurnTimer += g_fSecondPerFrame;

		if (m_fEnemyTurnTimer > 2.5f)
		{
			m_fEnemyTurnTimer = 0.0f;
			m_bEnemyTurn = false;
		}
	}
}

void K_BattleScene::CharacterActionProcessing()
{
	// 플레이어
	switch (m_iPlayerState)
	{
	case 0:
		break;
	case 1: // 강타
		m_pPlayerSound = m_pEffectSoundList.find(L"Attack_Strike")->second;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[3];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);

		m_pPlayer->m_vPos.x += 3000.0f * g_fSecondPerFrame;
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();
		if (m_pPlayer->m_vPos.x > 950.0f) 
		{
			m_iPlayerState = 99; 
		}
		break;

	case 2: // 방어
		m_pPlayerSound = m_pEffectSoundList.find(L"Shield")->second;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[1];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();

		m_iPlayerState = 98;
		break;

	case 3: // 폼멜 타격
		m_pPlayerSound = m_pEffectSoundList.find(L"Attack_PommelStrike")->second;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[4];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);

		m_pPlayer->m_vPos.x += 3000.0f * g_fSecondPerFrame;
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();
		if (m_pPlayer->m_vPos.x > 950.0f) 
		{ 
			m_iPlayerState = 99; 
		}
		break;

	case 4: // 몽둥이질
		m_pPlayerSound = m_pEffectSoundList.find(L"Attack_Bludgeon")->second;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[2];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);

		m_pPlayer->m_vPos.x += 3000.0f * g_fSecondPerFrame;
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();
		if (m_pPlayer->m_vPos.x > 950.0f) 
		{ 
			m_iPlayerState = 99; 
		}
		break;

	case 5: // 철의 파동
		m_pPlayerSound = m_pEffectSoundList.find(L"Attack_IronWave")->second;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[3];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);

		m_pPlayer->m_vPos.x += 3000.0f * g_fSecondPerFrame;
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();
		if (m_pPlayer->m_vPos.x > 950.0f)
		{
			m_iPlayerState = 99;
		}
		break;

	case 50: // 적한테 맞았을때 (방어도 초과)
		m_pPlayerSound = m_pEffectSoundList.find(L"Attack_Strike")->second;

		m_fPlayerActionTimer += g_fSecondPerFrame;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[5];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();

		m_iPlayerState = 98;
		break;

	case 51: // 적한테 맞았을때 (방어도로 방어)
		m_pPlayerSound = m_pEffectSoundList.find(L"Block")->second;

		m_fPlayerActionTimer += g_fSecondPerFrame;

		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[6];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();

		m_iPlayerState = 98;
		break;


	case 98: // 안움직이고 방어도/버프 등등
		m_pPlayerSound->Play(0); // 사실 이거 여러번 돌면 안되는데.. 사운드 죄다 0.5초 이상이라는 가정하에 작동함

		m_fPlayerActionTimer += g_fSecondPerFrame;
		if (m_fPlayerActionTimer > 0.5f)
		{
			m_fPlayerActionTimer = 0.0f;
			m_iPlayerState = 100;
		}
		break;

	case 99: // 적한테 닿았을 때 플래그들 처리
		m_pPlayerSound->PlayEffect();

		m_pEnemyEffect->m_bEffectOn = true;
		m_iCameraState = 1; 
		m_iPlayerState = 100;
		break;

	case 100: // 때리고 후퇴
		m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[7];
		m_pPlayer->SetMask(m_pPlayer->m_pTexture);

		m_pPlayer->m_vPos.x -= 2000.0f * g_fSecondPerFrame;
		m_pPlayer->SetPosition(m_pPlayer->m_vPos);
		m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
		m_pPlayer->SetDrawPos();
		if (m_pPlayer->m_vPos.x < m_pPlayer->m_vPosOrgin.x)
		{
			m_pPlayer->m_pTexture = m_pPlayer->m_pCharacterTextureList[0];
			m_pPlayer->SetMask(m_pPlayer->m_pTexture);

			m_pPlayer->m_vPos.x = m_pPlayer->m_vPosOrgin.x;
			m_pPlayer->SetPosition(m_pPlayer->m_vPos);
			m_pPlayer->SetDrawSize({ m_pPlayer->m_rtInit.z / g_rtClient.right, m_pPlayer->m_rtInit.w / g_rtClient.bottom });
			m_pPlayer->SetDrawPos();
			m_iPlayerState = 0;
		}
		break;
	}

	// 몬스터
	if (m_fEnemyTurnTimer > 0.7f) // 1초 딜레이 후에 움직이도록, 존나 이상한 방법같긴한데 몰라 일단해
	{
		switch (m_iEnemyStae)
		{
		case 0: // 가만히 있는거
			break;

		case 1: // 공격
			m_pEnemy->m_vPos.x -= 3000.0f * g_fSecondPerFrame;
			m_pEnemy->SetPosition(m_pEnemy->m_vPos);
			m_pEnemy->SetDrawPos();
			if (m_pEnemy->m_vPos.x < 300.0f)
			{
				m_iEnemyStae = 99;
			}
			break;

		case 2: // 힐
			m_pEnemyEffect->m_bEffectOn = true;
			m_pEnemySound = m_pEffectSoundList.find(L"Heal")->second;
			for (int i = 0; i < 5; i++) { m_pEnemySound->PlayEffect(); } // 이펙트 소리가 너무 작아서..
			m_iEnemyStae = 100;
			break;

		case 99: // 몬스터가 때릴 때 플래그 처리
			m_pPlayerEffect->m_bEffectOn = true;
			m_iCameraState = 1;
			if (m_iPlayerShield > 0) { m_iPlayerState = 51; }
			else { m_iPlayerState = 50; }

			m_iEnemyStae = 100;
			break;

		case 100: // 때리고 후퇴
			m_pEnemy->m_vPos.x += 2000.0f * g_fSecondPerFrame;
			m_pEnemy->SetPosition(m_pEnemy->m_vPos);
			m_pEnemy->SetDrawPos();
			if (m_pEnemy->m_vPos.x > m_pEnemy->m_vPosOrgin.x)
			{
				m_pEnemy->m_vPos.x = m_pEnemy->m_vPosOrgin.x;
				m_pEnemy->SetPosition(m_pEnemy->m_vPos);
				m_pEnemy->SetDrawPos();
				m_iEnemyStae = 0;
			}
			break;
		}
	}
}

void K_BattleScene::CameraShake()
{
	switch(m_iCameraState)
	{
	case 0:
		break;
	case 1:
		m_vCameraPos.x += 3000.0f * g_fSecondPerFrame;
		m_vCameraPos.y += 3000.0f * g_fSecondPerFrame;

		m_pBackGround->SetCamera(m_vCameraPos);
		m_pBackGround->SetPosition(m_pBackGround->m_vPos);

		if (m_vCameraPos.x > 50.0f) m_iCameraState = 2;
		break;
	case 2:
		m_vCameraPos.x -= 3000.0f * g_fSecondPerFrame;
		m_vCameraPos.y -= 3000.0f * g_fSecondPerFrame;

		m_pBackGround->SetCamera(m_vCameraPos);
		m_pBackGround->SetPosition(m_pBackGround->m_vPos);

		if (m_vCameraPos.y < -50.0f) m_iCameraState = 3;
		break;
	case 3:
		m_vCameraPos.x += 3000.0f * g_fSecondPerFrame;
		m_vCameraPos.y += 3000.0f * g_fSecondPerFrame;

		m_pBackGround->SetCamera(m_vCameraPos);
		m_pBackGround->SetPosition(m_pBackGround->m_vPos);

		if (m_vCameraPos.x < 0.0f && m_vCameraPos.y < 0.0f)
		{
			m_vCameraPos = {0,0};
			m_pBackGround->SetCamera(m_vCameraPos);
			m_pBackGround->SetPosition(m_pBackGround->m_vPos);
			m_pPlayer->SetCamera(m_vCameraPos);
			m_pEnemy->SetCamera(m_vCameraPos);
			m_iCameraState = 0;
		}
		break;
	}
}

void K_BattleScene::EnemyDefeat()
{
	m_fEnemyTurnTimer += g_fSecondPerFrame;
	if (m_fEnemyTurnTimer > 0.8f) 
	{
		m_pEnemy->m_pTexture = m_pEnemy->m_pCharacterTextureList[1];
		m_pEnemy->SetMask(m_pEnemy->m_pTexture);
	}
}


