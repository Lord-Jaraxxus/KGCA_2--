#pragma once
#include "K_BaseObject.h"

class K_UIObject : public K_BaseObject
{
public:
	virtual bool SetUV(ImVec2 uvTL, ImVec2 uvBR);
	virtual bool SetPosition(ImVec2 orginPos, ImVec2 widthHeight, float depth);
	ImVec2 PtoN(ImVec2 pxWH); // Pixel to NDC, pixle Width Height
	virtual bool SetAlpha(float alpha);

public:
	ImVec2 m_OrginPos;
	ImVec2 m_WidthHeight;
	float  m_fDepth = 0.0f;
	float  m_fAlpha = 1.0f;

};

