#pragma once
#include "K_UIObject.h"

enum ButtonState
{
	NORMAL,
	HOVER,
	CLICK,
	DISABLE,
};


class K_Button : public K_UIObject
{
public:
	virtual bool Frame() override;
	virtual bool SetState();

public:
	ButtonState m_BS;
	ImVec2 m_CollisionBox[2];	// 각각 Top Left, Bottom Right인데 NDC 기준
	bool m_bIsDisable = false;
};

