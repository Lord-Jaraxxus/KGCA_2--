#pragma once
#include "K_UIObject.h"

struct CI // Cut Information
{
	ImVec2 pxWH; // pixel Width Height

	ImVec2 uvTL; // UV Top & Left
	ImVec2 uvBR; // UV Bottom & Right
	
	std::wstring tn; // Texture name
	std::wstring sn; // Shader name

	K_Texture* tc; // Texture class
};

class K_Sprite : public K_UIObject
{
public:
	virtual bool Frame() override;
	virtual bool Release() override;
	virtual bool UpdateCut();
	virtual bool AddCut(ImVec2 pxWH, ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn);

public:
	std::vector<CI*> m_pCutInfoList;
	int m_iMaxFrame = 0;
	float m_fCurrentFrame = 0;
	int m_iFrameSpeed = 30;
};

