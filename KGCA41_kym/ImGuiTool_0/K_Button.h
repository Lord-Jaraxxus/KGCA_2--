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
	virtual bool Release() override;
	virtual bool AddTexture(std::wstring filename);
	virtual bool SetState();

public:
	ButtonState m_BS;
	std::vector<K_Texture*> m_pTextureList;
	bool m_bIsDisable = false;
};

