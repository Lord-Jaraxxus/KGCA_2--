#pragma once
#include "K_Scene.h"

class K_CardRewardScene : public K_Scene
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:
	K_Deck*		m_pDeck;
	K_Button*	m_pRewardCard1;
	K_Button*	m_pRewardCard2;
	K_Button*	m_pRewardCard3;

public:
	void	CardSelect();
};

