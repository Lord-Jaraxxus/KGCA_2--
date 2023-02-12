#pragma once
#include "K_BaseObject.h"
#include "K_DebugCamera.h"

class K_Map : public K_BaseObject
{
public:
	DWORD        m_dwNumRows;
	DWORD        m_dwNumColumns;
public:
	bool  Build(UINT iWidth, UINT iHeight);
};
