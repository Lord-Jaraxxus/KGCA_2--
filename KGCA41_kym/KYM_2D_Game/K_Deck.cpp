#include "K_Deck.h"

K_Deck::K_Deck() 
{
	m_DeckList.push_back(Strike);
	m_DeckList.push_back(Strike);
	m_DeckList.push_back(Strike);
	m_DeckList.push_back(Defend);
	m_DeckList.push_back(Defend);
	m_DeckList.push_back(Defend);
	m_DeckList.push_back(PommelStrike);
	m_DeckList.push_back(IronWave); 
	//m_DeckList.push_back(ShrugItOff); // ㅅㅂ 대체 왜안되는거야
	
	for (auto obj : m_DeckList) 
	{
		m_RemainingCardList.push_back(obj);
	}

	std::random_shuffle(m_RemainingCardList.begin(), m_RemainingCardList.end());
}

void K_Deck::Draw(int num_of_draw)
{
	for (int i = 0; i < num_of_draw; i++)
	{
		if (m_RemainingCardList.empty() == true)
		{
			Shuffle();
		}
		
		int drawed_card = m_RemainingCardList.back();
		m_HandList.push_back(drawed_card);
		m_RemainingCardList.pop_back();
	}
}

void K_Deck::Shuffle()
{
	for (auto obj : m_DiscardList)
	{
		m_RemainingCardList.push_back(obj);
	}
	
	std::random_shuffle(m_RemainingCardList.begin(), m_RemainingCardList.end());

	m_DiscardList.clear();
}

void K_Deck::TurnEnd() 
{
	for (auto obj : m_HandList)
	{
		if (obj != 99) 
		{
			m_DiscardList.push_back(obj);
		}
	}
	m_HandList.clear();
}