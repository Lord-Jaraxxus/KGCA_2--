#include "k_GameCore.h"

bool k_GameCore::Init2D()
{
    player2D.SetPosition(0, 0, 30, 30);
    player2D.SetVector(player2D.m_vForces, 5.0f, 5.0f);
    kc.QT.m_pRootNode = new k_Node2D;
    kc.QT.BuildQuadTree(kc.QT.m_pRootNode, 100.0f, 100.0f);

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object2D* pObj = new k_MapObject2D; // 여기 했던 것 같은데 어디더라 그 다형성 어쩌구저쩌구였는데 이거 좀 중요했는데
        pObj->m_vDirection.Set(0.0f, 0.0f);
        AllObjectList2D.insert(std::make_pair(iObj, pObj));
        kc.AddObject(pObj, 0); // 스태틱(정적) 오브젝트
    }

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object2D* pObj = new k_NPC2D;
        npcList2D.insert(std::make_pair(iObj, pObj));
        AllObjectList2D.insert(std::make_pair(iObj + 10, pObj));
        kc.AddObject(pObj, 1); // 대이내믹(동적) 오브젝트
    }

    return false;
}

bool k_GameCore::Frame2D(float fDeltaTime, float fGameTime)
{
    kc.DynamicObjectReset(kc.QT.m_pRootNode);
    for (auto obj : npcList2D)
    {
        k_Object2D* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        kc.AddObject(pObject, 1);
    }
    player2D.Frame(fDeltaTime, fGameTime);

    DrawList2D = kc.COL(&player2D);

    return false;
}

bool k_GameCore::Render2D()
{
    std::cout << "player:"
        << player2D.m_sRect.x1 << "," << player2D.m_sRect.y1 << "  //  "
        << player2D.m_sRect.x2 << "," << player2D.m_sRect.y2
        << std::endl;
    if (!DrawList2D.empty())
    {
        for (int iObj = 0; iObj < DrawList2D.size(); iObj++)
        {
            std::cout << "object:"
                << DrawList2D[iObj]->m_sRect.x1 << "," << DrawList2D[iObj]->m_sRect.y1
                << std::endl;
        }
    }
    return false;
}

bool k_GameCore::Release2D()
{
    for (auto object : AllObjectList2D)
    {
        delete object.second;
    }
    AllObjectList2D.clear();
    npcList2D.clear();
    return true;
}

bool k_GameCore::Run2D()
{
    Init2D();
    float  fGameTimer = 0.0f;
    //float  fDelay = 1000;
    float  fDelay = 10; // 디버깅용
    while (fGameTimer < 10.0f)
    {
        Frame2D(fDelay / 1000.0f, fGameTimer);
        Render2D();
        Sleep(fDelay);
        system("cls");
        fGameTimer += fDelay / 1000.0f;
    }
    Release2D();
    return true;
}



// 이 밑으로는 3D, 옥트리

bool k_GameCore::Init()
{
    player.SetPosition(k_Vector(0.0f, 0.0f, 0.0f), k_Vector(50.0f, 50.0f, 50.0f));
    player.m_vForces.Set(5.0f, 5.0f, 5.0f);
    kc.OT.m_pRootNode = new k_Node;
    kc.OT.BuildOctree(kc.OT.m_pRootNode, k_Vector(100.0f, 100.0f, 100.0f));

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object* pObj = new k_MapObject; // 여기 했던 것 같은데 어디더라 그 다형성 어쩌구저쩌구였는데 이거 좀 중요했는데
        pObj->m_vDirection.Set(0.0f, 0.0f, 0.0f);
        AllObjectList.insert(std::make_pair(iObj, pObj));
        kc.AddObject(pObj, 0); // 스태틱(정적) 오브젝트
    }

    for (int iObj = 0; iObj < 10; iObj++)
    {
        k_Object* pObj = new k_NPC;
        npcList.insert(std::make_pair(iObj, pObj));
        AllObjectList.insert(std::make_pair(iObj + 10, pObj));
        kc.AddObject(pObj, 1); // 대이내믹(동적) 오브젝트
    }

    return false;
}

bool k_GameCore::Frame(float fDeltaTime, float fGameTime)
{
    kc.DynamicObjectReset(kc.OT.m_pRootNode);
    for (auto obj : npcList)
    {
        k_Object* pObject = obj.second;
        pObject->Frame(fDeltaTime, fGameTime);
        kc.AddObject(pObject, 1);
    }
    player.Frame(fDeltaTime, fGameTime);

    DrawList = kc.COL(&player);

    return false;
}

bool k_GameCore::Render()
{
    std::cout << "player:"
        << player.m_Box.vMin.x << "," << player.m_Box.vMin.y << "," << player.m_Box.vMin.z << "     //      " 
        << player.m_Box.vMax.x << "," << player.m_Box.vMax.y << "," << player.m_Box.vMax.z
        << std::endl;

    if (!DrawList.empty())
    {
        for (int iObj = 0; iObj < DrawList.size(); iObj++)
        {
            k_Vector vStaticDirection(0.0f, 0.0f, 0.0f);
            if (DrawList[iObj]->m_vDirection == vStaticDirection) std::cout << "스태틱 : ";
            else std::cout << "대이내믹 : ";

            std::cout
                << DrawList[iObj]->m_Box.vMin.x << "," << DrawList[iObj]->m_Box.vMin.y << "," << DrawList[iObj]->m_Box.vMin.z
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
    return true;
}

bool k_GameCore::Run()
{
    Init();
    float  fGameTimer = 0.0f;
    //float  fDelay = 1000;
    float  fDelay = 10.0f; // 디버깅용
    while (fGameTimer < 1.0f)
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