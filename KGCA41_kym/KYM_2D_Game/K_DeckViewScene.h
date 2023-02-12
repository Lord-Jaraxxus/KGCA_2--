#pragma once

#include "K_Scene.h"

class K_DeckViewScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	K_Deck*	  m_pDeck;

public:
	K_Button* m_pBackButton;
	std::vector<K_Texture*> m_pCardTextureList;
	std::vector<K_Button*>	m_pCardList;

public:
	int	m_iDeckViewState = 0; // 0¿Ã∏È µ¶∏ÆΩ∫∆Æ, 1¿Ã∏È ªÃ¿ª µ¶, 2∏È πˆ∏∞ µ¶

public:
	void	DeckStateTitle();
	void	CardSetting();
};

