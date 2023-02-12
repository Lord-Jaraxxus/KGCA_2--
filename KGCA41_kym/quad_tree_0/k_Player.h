#pragma once
#include "k_Object.h"

class k_Player : public k_Object
{
public:
	void   Frame(float fDeltaTime, float fGameTime) override;
};

