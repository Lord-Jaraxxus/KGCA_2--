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

	void ImGuiFrame();

public:
	std::vector<K_UIObject*> m_pUIList;
	// std::vector<K_UIObject*> m_pUIAlphaList; // �ʿ����� ���� �ö�? ���İ� �ִ°͸� ���� ���� ����Ʈ
	int m_CurrentID = 1;

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
	bool IsAlphaBlend = true;
	bool IsClear = false;

	float ImageXY[2] = { 0, };
	float ImageWH[2] = { 1, 1 };
	float ImageUV[2][2] = { { 0, 0 }, {1, 1} };
	float ImageDA[2] = { 0, 1 };
	float ButtonXY[2] = { 0, };
	float ButtonWH[4][2] = { 0,  };	// [N H C D] [X Y]
	float ButtonUV[4][2][2] = { {{0,0},{1,1}}, {{0,0},{1,1}}, {{0,0},{1,1}}, {{0,0},{1,1}} };	// [N H C D] [TopLeft BottomRight] [X Y]
	float ButtonDA[2] = { 0, 1 };
	float SpriteXY[2] = { 0, };
	float SpriteWH[2] = { 1, 1 };
	float SpriteDA[2] = { 0, 1 };

	std::wstring m_szFileName = L"newfile";
	std::wstring m_szFileContent = L"";
	std::vector<std::wstring> m_splitContent;

	std::wstring m_szDefaultShaderName = L"../../data/shader/DefaultObject_Orgin.txt";
	std::wstring m_szImageFileName = L"D://KGCA_2//data//img//charport.bmp";
	std::wstring m_szButtonFileName_N = L"D://KGCA_2//data//img//button//Button_N.png";
	std::wstring m_szButtonFileName_H = L"D://KGCA_2//data//img//button//Button_H.png";
	std::wstring m_szButtonFileName_C = L"D://KGCA_2//data//img//button//Button_C.png";
	std::wstring m_szButtonFileName_D = L"D://KGCA_2//data//img//button//Button_D.png";

public:
	K_UIObject* CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
	K_Button* CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
	K_Sprite* CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha);
	void Clear();

	std::wstring FileOpen();
	void FileSave(std::wstring saveFileName);
	void FileLoad();
	std::wstring FileReadToString(std::wstring readFileName);
	std::vector<std::wstring> SplitString(std::wstring inputStr, std::wstring delimiter);

	ImVec2 AtoV(float array[2]);
}; 

