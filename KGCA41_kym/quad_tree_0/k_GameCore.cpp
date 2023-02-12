#include "k_GameCore.h"

bool k_GameCore::Init()
{
    player.SetPosition(0, 0, 30, 30);
    player.SetVector(player.m_vForces, 1.0f, 0.0f);
    kc.QT.m_pRootNode = new k_Node;
    kc.QT.BuildQuadTree(kc.QT.m_pRootNode, 100.0f, 100.0f);

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object* pObj = new k_MapObject; // 여기 했던 것 같은데 어디더라 그 다형성 어쩌구저쩌구였는데 이거 좀 중요했는데
        AllObjectList.insert(std::make_pair(iObj, pObj));
        kc.AddStaticObject(pObj);
    }

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object* pObj = new k_NPC;
        npcList.insert(std::make_pair(iObj, pObj));
        AllObjectList.insert(std::make_pair(iObj, pObj));
        kc.AddDynamicObject(pObj);
    }

    return false;
}

bool k_GameCore::Frame(float fDeltaTime, float fGameTime)
{
    kc.DynamicObjectReset(kc.QT.m_pRootNode);
    for (auto obj : npcList)
    {
        k_Object* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        kc.AddDynamicObject(pObject);
    }
    player.Frame(fDeltaTime, fGameTime);

    DrawList = kc.COL(&player);

    return false;
}


bool k_GameCore::Render()
{
    std::cout << "player:"
        << player.m_sRect.x1 << "," << player.m_sRect.y1 << ","
        << player.m_sRect.x2 << "," << player.m_sRect.y2
        << std::endl;
    if (!DrawList.empty())
    {
        for (int iObj = 0; iObj < DrawList.size(); iObj++)
        {
            std::cout << "object:"
                << DrawList[iObj]->m_sRect.x1 << "," << DrawList[iObj]->m_sRect.y1 << ","
                << DrawList[iObj]->m_sRect.x2 << "," << DrawList[iObj]->m_sRect.y2
                << std::endl;
        }
    }
    return false;
}

bool k_GameCore::Release()
{
    for (auto object : AllObjectList)
    {
        delete object.second;
    }
    AllObjectList.clear();
    npcList.clear();
    return false;
}

bool k_GameCore::Run()
{
    Init();
    float  fGameTimer = 0.0f;
    //float  fDelay = 1000;
    float  fDelay = 1000; // 디버깅용
    while (fGameTimer < 60.0f)
    {
        Frame(fDelay / 1000.0f, fGameTimer);
        Render();
        Sleep(fDelay);
        system("cls");
        fGameTimer += fDelay / 1000.0f;
    }
    Release();
    return true;
}