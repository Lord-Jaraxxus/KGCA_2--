#pragma once
#pragma comment(lib, "KymGameCore_d")

#include "K_GameCore.h"
#include "K_Button.h"
#include "K_Sprite.h"

class Sample : public K_GameCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	std::vector<K_UIObject*> m_pUIList;
	std::vector<K_UIObject*> m_pUIAlphaList;

	std::vector<K_UIObject*> m_pRectList;
	std::vector<K_Button*> m_pButtonList;
	std::vector<K_Sprite*> m_pSpriteList;

	ImVec2 m_CursorPos;

public:
	bool IsRandom = false;
	bool IsSelect = false;
	bool IsDisable = false;
	bool IsWireFrame = false;
	bool IsDepth = false;
	bool IsAlphaBlend = false;

public:
	bool CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth);
	bool CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight, float depth);
	bool CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight, float depth);
}; 

