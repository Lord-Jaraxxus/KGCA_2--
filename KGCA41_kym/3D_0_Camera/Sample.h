#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_GameCore.h"
#include "K_2dObject.h"
#include "K_Shape.h"
#include "K_Map.h"
#include "K_Camera.h"
#include "K_DebugCamera.h"

class Sample : public K_GameCore
{
public:
	K_BaseObject*			m_pBG = nullptr;
	K_Map*					m_pMap = nullptr;
	K_Box*					m_pBoxObjA = nullptr;
	K_ShapeDirectionLine*	m_pBoxADL = nullptr;
	K_Box*					m_pBoxObjB = nullptr;
	K_ShapeDirectionLine*	m_pBoxBDL = nullptr;
	K_Camera*				m_pMainCamera = nullptr;
	K_DebugCamera*			m_pDebugCamera = nullptr;
	K_ShapeLine*			m_pZLine = nullptr;
	K_ShapeDirectionLine*	m_pDL = nullptr;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

