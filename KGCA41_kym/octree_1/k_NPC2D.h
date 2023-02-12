#pragma once
#include "k_Object2D.h"

class k_NPC2D : public k_Object2D
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

