#include "k_Collision.h"

k_CollisionType k_Collision::RectToRect(k_Rect& a, k_Rect& b) 
{
// 0 : 완전제외(0)
// 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
// a.x(10)----30------a.x2(40)
// 합집합
    float fMinX;   float fMinY;
    float fMaxX;   float fMaxY;
    fMinX = a.x1 < b.x1 ? a.x1 : b.x1;
    fMinY = a.y1 < b.y1 ? a.y1 : b.y1;
    fMaxX = a.x2 > b.x2 ? a.x2 : b.x2;
    fMaxY = a.y2 > b.y2 ? a.y2 : b.y2;
    //  가로 판정
    if ((a.w + b.w) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.h + b.h) >= (fMaxY - fMinY))
        {
            //// 교차한다. 교집합 // 여긴 아직까진 아마 없어도 될?듯
            //float x, y, x2, y2;
            //k_Rect Intersect;
            //x = a.x1 > b.x1 ? a.x1 : b.y1;
            //y = a.y1 > b.y1 ? a.y1 : b.y1;
            //x2 = a.x2 < b.x2 ? a.x2 : b.x2;
            //y2 = a.y2 < b.y2 ? a.y2 : b.y2;
            //Intersect.Set(x, y, x2 - x, y2 - y);
            //if (Intersect == a || Intersect == b)
            //{
            //    return k_CollisionType::RECT_IN;
            //}
            return k_CollisionType::RECT_OVERLAP;
        }
    }
    return k_CollisionType::RECT_OUT;
}

bool k_Collision::RectToInRect(k_Rect& a, k_Rect& b) 
{
    if (a.x1 <= b.x1)
    {
        if ((a.x1 + a.w) >= b.x1 + b.w)
        {
            if (a.y1 <= b.y1)
            {
                if ((a.y1 + a.h) >= b.y1 + b.h)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool k_Collision::CircleToCircle(k_Circle& a, k_Circle& b)
{
    float x = a.cx - b.cx;
    float y = a.cy - b.cy;
    float distance = sqrt(x * x + y * y);

    if (distance <= a.fRadius + b.fRadius) return true;
    else return false;
}

void k_Collision::AddStaticObject(k_Object* pObj)
{
    k_Node* pFindNode = FindNodeToGo(QT.m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_StaticObjectList.push_back(pObj);
    }
}

void k_Collision::AddDynamicObject(k_Object* pObj)
{
    k_Node* pFindNode = FindNodeToGo(QT.m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        pFindNode->m_DynamicObjectList.push_back(pObj);
    }
}

k_Node* k_Collision::FindNodeToGo(k_Node* pNode, k_Object* pObj) // 선생님은 queue랑 do-while로 하셨는데 저어는 걍 재귀써봄
{
    if (pNode == nullptr) return nullptr;
    
    k_Node* final_node = pNode;

    for (int iChild = 0; iChild < 4; iChild++) 
    {
        if (pNode->m_pChild[iChild] != nullptr) 
        {
            if (RectToInRect(pNode->m_pChild[iChild]->m_sRect, pObj->m_sRect))
            {
                final_node = FindNodeToGo(pNode->m_pChild[iChild], pObj);
            }
        }
    }
    return final_node;
}

void k_Collision::DynamicObjectReset(k_Node* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_DynamicObjectList.clear();
    DynamicObjectReset(pNode->m_pChild[0]);
    DynamicObjectReset(pNode->m_pChild[1]);
    DynamicObjectReset(pNode->m_pChild[2]);
    DynamicObjectReset(pNode->m_pChild[3]);
}

std::vector<k_Object*> k_Collision::COL(k_Object* pObj)
{
    std::vector<k_Object*> list;
    GCO(QT.m_pRootNode, pObj, list);
    return list;
};

void k_Collision::GCO(k_Node* pNode, k_Object* pObj, std::vector<k_Object*>& list) 
{
    if (pNode == nullptr) return;

    for (int iObj = 0; iObj < pNode->m_StaticObjectList.size(); iObj++)
    {
        if (k_Collision::RectToRect(
            pNode->m_StaticObjectList[iObj]->m_sRect,
            pObj->m_sRect))
        {
            list.push_back(pNode->m_StaticObjectList[iObj]);
        }
    }

    for (int iObj = 0; iObj < pNode->m_DynamicObjectList.size(); iObj++)
    {
        if (k_Collision::RectToRect(
            pNode->m_DynamicObjectList[iObj]->m_sRect,
            pObj->m_sRect))
        {
            list.push_back(pNode->m_DynamicObjectList[iObj]);
        }
    }

    if (pNode->m_pChild[0] != nullptr)
    {
        for (int iChild = 0; iChild < 4; iChild++)
        {
            if (k_Collision::RectToRect(
                pNode->m_pChild[iChild]->m_sRect,
                pObj->m_sRect))
            {
                GCO(pNode->m_pChild[iChild], pObj, list);
            }
        }
    }
}