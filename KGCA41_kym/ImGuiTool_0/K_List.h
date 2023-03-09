#pragma once
#include "K_UIObject.h"
#include "K_Button.h"
#include "K_Sprite.h"

class K_List 
{
public:
	std::vector<K_UIObject*> m_pUIList;
	std::map<std::wstring, K_UIObject*> m_pUIMap;

public:
	virtual bool Frame();
	virtual bool Render();
};

