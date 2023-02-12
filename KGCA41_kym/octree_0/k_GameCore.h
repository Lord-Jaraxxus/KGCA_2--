#pragma once
#include <map> // map
#include <windows.h> //sleep, min, max, etc

#include "k_Collision.h"

#include "k_Player2D.h"
#include "k_MapObject2D.h"
#include "k_NPC2D.h"

#include "k_Player.h"
#include "k_MapObject.h"
#include "k_NPC.h"


class k_GameCore
{
public:
    k_Collision   kc;

public:
    k_Player2D                 player2D;
    std::map<int, k_Object2D*> npcList2D;
    std::map<int, k_Object2D*> AllObjectList2D;
    std::vector<k_Object2D*>   DrawList2D;

public:
    k_Player                    player;
    std::map<int, k_Object*>    npcList;
    std::map<int, k_Object*>    AllObjectList;
    std::vector<k_Object*>      DrawList;

public:
    bool    Init2D();     // 초기화	
    bool    Frame2D(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render2D();   // 화면에 드로우
    bool    Release2D();  // 소멸하자
    bool	Run2D();      // 실행하자

public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

