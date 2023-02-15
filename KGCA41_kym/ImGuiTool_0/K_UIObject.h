#pragma once
#include "K_BaseObject.h"

class K_UIObject : public K_BaseObject
{
public:
	virtual bool SetUV(ImVec2 uvTL, ImVec2 uvBR);
	virtual bool SetPosition(ImVec2 orginPos, ImVec2 widthHeight);

public:
	ImVec2 m_OrginPos;
	ImVec2 m_WidthHeight;
};

