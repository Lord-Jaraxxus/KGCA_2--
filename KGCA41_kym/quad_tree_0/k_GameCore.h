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
    bool    Init();     // �ʱ�ȭ	
    bool    Frame(float fDeltaTime, float fGameTime);    // �ǽð� ���
    bool    Render();   // ȭ�鿡 ��ο�
    bool    Release();  // �Ҹ�����
    bool	Run();      // ��������
};

