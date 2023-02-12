#pragma once
#include "K_Scene.h"

class K_BattleScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	K_BackGround*			m_pBackGround;
	K_Character*			m_pPlayer;
	K_Character*			m_pEnemy;
	K_Button*				m_pTurnEndButton;
	K_EnemyIntent*			m_pEnemyIntent;
	K_EnemyIntent*			m_pPlayerEffect; // 귀찮으니 일단.. 나중에 이름을 K_Effect로 바꾸든가 해야지
	K_EnemyIntent*			m_pEnemyEffect;
	int						m_iEnemyIntentPower;
	int						m_iMaxMana = 3;
	int						m_iCurrentMana = 3;
	bool					m_bNoMana = false;
	float					m_fWarningTimer = 0.0f;
	
	K_2dObject* m_pManaImage;
	K_2dObject* m_pShieldImage;

	K_Button* m_pRemainViewButton;
	K_Button* m_pDiscardViewButton;

	std::vector<K_Card*>	m_pCardList;
	std::vector<K_Texture*>	m_pCardTextureList;

	K_Sound*	m_pBGM;
	K_Sound*	m_pPlayerSound = nullptr;
	K_Sound*	m_pEnemySound = nullptr;
	std::map<std::wstring, K_Sound*>	m_pEffectSoundList;

public:
	int				m_iPlayerMaxHP;
	int				m_iPlayerCurrentHP;
	int				m_iPlayerShield = 0;
	int				m_iEnemyMaxHP;
	int				m_iEnemyCurrentHP;

public:
	void			SetDeck(K_Deck* deck);
	void			SetPlayerHP(int currentHP, int maxHP);

public:
	void			Combat(int turn);
	void			CardUsed(int card_num);
	void			EnemyTurn(int turn);
	void			CardSetting();
	void			EffectProcessing();
	void			EnemyTurnProcessing();
	void			CharacterActionProcessing();
	void			CameraShake();
	void			EnemyDefeat();

public:
	K_Deck*			m_pDeck;
	int				m_iTurn = 0;
	bool			m_bTurnStart = true;
	bool			m_bEnemyTurn = false;
	float			m_fEnemyTurnTimer = 0.0f;
	int				m_iPlayerState = 0;
	float			m_fPlayerActionTimer = 0.0f;
	int				m_iEnemyStae = 0;
	k_Vector2D		m_vCameraPos = { 0,0 };
	int				m_iCameraState = 0;
};

