#pragma once
#include "K_BaseObject.h"
#include "K_Input.h"
#include <fstream>

enum UIType 
{
	IMAGE,
	BUTTON,
	SPRITE,
}; 

struct CI // Cut Information
{
	ImVec2 pxWH; // pixel Width Height

	ImVec2 uvTL; // UV Top & Left
	ImVec2 uvBR; // UV Bottom & Right

	std::wstring tn; // Texture name
	std::wstring sn; // Shader name

	K_Texture* tc; // Texture class

	int cn;	// cut number
};

class K_UIObject : public K_BaseObject
{
public:
	virtual bool	Frame() override;
	virtual bool	Release() override;

public:
	virtual bool	SetUV(ImVec2 uvTL, ImVec2 uvBR);
	virtual bool	SetPosition(ImVec2 orginPos, ImVec2 widthHeight, float depth);
	virtual bool	SetAlpha(float alpha);
	static ImVec2	PtoN(ImVec2 pxWH); // Pixel to NDC, pixle Width Height
	static ImVec2	PtoN_Pos(ImVec2 pxWH);

	virtual bool	Save(std::ofstream& outfile);
	virtual bool	Load();

	virtual bool	AddCut(ImVec2 pxWH, ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn);
	virtual bool	AddCut(CI newCut);

	virtual void	Drag();

public:
	UIType			m_Type;
	int				m_ID;
	std::wstring	m_szObjName;

	ImVec2 m_OrginPos;
	ImVec2 m_WidthHeight; // 일단 NDC
	float  m_fDepth = 0.0f;
	float  m_fAlpha = 1.0f;

	// 얘들은 베이스 오브젝트에 이미 있음
	//std::wstring	m_szTextureName;
	//std::wstring	m_szShaderName;

	std::vector<CI*> m_pCutInfoList;
	int m_iCurrentCutNum = 0;

	bool m_bDraggable = true; // 나중에 툴에 드래그가능 상호작용 가능케끔하고 기본 false로 해야할듯..
	bool m_bIsClicked = false;
};

