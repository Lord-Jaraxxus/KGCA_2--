#pragma once
#include "K_UIObject.h"

class K_Sprite : public K_UIObject
{
public:
	virtual bool Frame() override;
	virtual bool Release() override;
	virtual bool UpdateCut();

public:
	int m_iMaxFrame = 0;
	float m_fCurrentFrame = 0;
	int m_iFrameSpeed = 30;
};

