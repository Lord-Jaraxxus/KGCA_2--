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
    virtual bool    Init() = 0;     // 초기화	
    virtual bool    Frame(float fDeltaTime, float fGameTime) = 0;    // 실시간 계산
    virtual bool    Render() = 0;  // 화면에 드로우
    virtual bool    Release() = 0; // 소멸하자
    virtual bool	Run() = 0;     // 실행하자

public:
    k_GameCore() {};
    virtual ~k_GameCore() {};
};

class k_GameCore2D : public k_GameCore
{
public:
    k_Player2D                 player2D;
    std::map<int, k_Object2D*> npcList2D;
    std::map<int, k_Object2D*> AllObjectList2D;
    std::vector<k_Object2D*>   DrawList2D;

public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자

    ~k_GameCore2D() {};
};

class k_GameCore3D : public k_GameCore
{
public:
    k_Player                    player;
    std::map<int, k_Object*>    npcList;
    std::map<int, k_Object*>    AllObjectList;
    std::vector<k_Object*>      DrawList;

public:
    bool    Init();     // 초기화	
    bool    Frame(float fDeltaTime, float fGameTime);    // 실시간 계산
    bool    Render();   // 화면에 드로우
    bool    Release();  // 소멸하자
    bool	Run();      // 실행하자

    ~k_GameCore3D() {};
};
