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
    bool    Init2D();     // �ʱ�ȭ	
    bool    Frame2D(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render2D();   // ȭ�鿡 ��ο�
    bool    Release2D();  // �Ҹ�����
    bool	Run2D();      // ��������

public:
    bool    Init();     // �ʱ�ȭ	
    bool    Frame(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render();   // ȭ�鿡 ��ο�
    bool    Release();  // �Ҹ�����
    bool	Run();      // ��������
};

