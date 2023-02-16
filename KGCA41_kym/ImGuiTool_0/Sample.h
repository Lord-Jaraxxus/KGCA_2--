#pragma once
#pragma comment(lib, "KymGameCore_d")

#include "K_GameCore.h"
#include "K_UIObject.h"
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
	bool IsToolActive = true;
	bool IsRandom = false;
	bool IsSelect = false;
	bool IsDisable = false;
	bool IsWireFrame = false;
	bool IsDepth = false;
	bool IsAlphaBlend = false;
	bool IsClear = false;

	float ImageXY[2] = { 0, };
	float ImageDA[2] = { 0, 1 };
	float ButtonXY[2] = { 0, };
	float ButtonDA[2] = { 0, 1 };
	float SpriteXY[2] = { 0, };
	float SpriteDA[2] = { 0, 1 };

public:
	bool CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
	bool CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
	bool CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
}; 

