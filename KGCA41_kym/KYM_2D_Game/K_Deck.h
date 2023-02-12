#pragma once
#include <vector>
#include <algorithm>

enum CardNum
{
	Strike = 0,
	Defend = 1,
	PommelStrike = 2,
	ShrugItOff = 3,
	Hemokinesis = 4,
	Bludgeon = 5,
	IronWave = 6,
};

class K_Deck
{
public:
	K_Deck();

public:
	std::vector<int> m_DeckList;
	std::vector<int> m_RemainingCardList;
	std::vector<int> m_HandList;
	std::vector<int> m_DiscardList;

public:
	void	Draw(int num_of_draw);
	void	Shuffle();
	void	TurnEnd();
};

