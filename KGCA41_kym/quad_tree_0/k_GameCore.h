#pragma once
#include <map> // map
#include <windows.h> //sleep, min, max, etc
#include "k_Collision.h"
#include "k_Player.h"
#include "k_MapObject.h"
#include "k_NPC.h"


class k_GameCore
{
public:
    k_Player    player;
    k_Collision   kc;
    std::map<int, k_Object*> npcList;
    std::map<int, k_Object*> AllObjectList;
    std::vector<k_Object*>   DrawList;
public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자
};

