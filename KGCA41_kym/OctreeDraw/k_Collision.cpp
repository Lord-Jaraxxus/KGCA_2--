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


k_Node2D* k_Collision::FindNodeToGo(k_Node2D* pNode, k_Object2D* pObj) // 선생님은 queue랑 do-while로 하셨는데 저어는 걍 재귀써봄
{
    if (pNode == nullptr) return nullptr;

    k_Node2D* final_node = pNode;

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

void k_Collision::AddObject(k_Object2D* pObj, int iObjectType)
{
    k_Node2D* pFindNode = FindNodeToGo(QT.m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        if(iObjectType == 0) pFindNode->m_StaticObjectList.push_back(pObj);
        else if (iObjectType == 1) pFindNode->m_DynamicObjectList.push_back(pObj);
    }
}

void k_Collision::DynamicObjectReset(k_Node2D* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_DynamicObjectList.clear();
    DynamicObjectReset(pNode->m_pChild[0]);
    DynamicObjectReset(pNode->m_pChild[1]);
    DynamicObjectReset(pNode->m_pChild[2]);
    DynamicObjectReset(pNode->m_pChild[3]);
}

void k_Collision::GCO(k_Node2D* pNode, k_Object2D* pObj, std::vector<k_Object2D*>& list) //Get Collided Object 
{
    if (pNode == nullptr) return;

    for (int iObj = 0; iObj < pNode->m_StaticObjectList.size(); iObj++)
    {
        if (RectToRect(
            pNode->m_StaticObjectList[iObj]->m_sRect,
            pObj->m_sRect))
        {
            list.push_back(pNode->m_StaticObjectList[iObj]);
        }
    }

    for (int iObj = 0; iObj < pNode->m_DynamicObjectList.size(); iObj++)
    {
        if (RectToRect(
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
            if (RectToRect(
                pNode->m_pChild[iChild]->m_sRect,
                pObj->m_sRect))
            {
                GCO(pNode->m_pChild[iChild], pObj, list);
            }
        }
    }
}

std::vector<k_Object2D*> k_Collision::COL(k_Object2D* pObj) // Collided Object List
{
    std::vector<k_Object2D*> list;
    GCO(QT.m_pRootNode, pObj, list);
    return list;
};






// 이 밑으로는 3D, 옥트리용

k_CollisionType k_Collision::BoxToBox(k_Box& a, k_Box& b)
{
    // 0 : 완전제외(0)
    // 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
    // a.x(10)----30------a.x2(40)
    // 합집합
    float fMinX;   float fMinY; float fMinZ;
    float fMaxX;   float fMaxY; float fMaxZ;
    fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
    fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
    fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;
    fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
    fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;
    fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

    //  가로 판정
    if ((a.vSize.x + b.vSize.x) >= (fMaxX - fMinX))
    {
        //  세로 판정
        if ((a.vSize.y + b.vSize.y) >= (fMaxY - fMinY))
        {
            if ((a.vSize.z + b.vSize.z) >= (fMaxZ - fMinZ)) 
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
    }
    return k_CollisionType::RECT_OUT;
}

bool k_Collision::BoxToInBox(k_Box& a, k_Box& b)
{
    if (a.vMin <= b.vMin) 
    {
        if (a.vMax >= b.vMax) 
        {
            return true;
        }
    }
    return false;
}

bool k_Collision::SphereToSphere(k_Sphere& a, k_Sphere& b)
{
    float fSumRadius = a.fRadius + b.fRadius;
    k_Vector vDir = a.vCenter - b.vCenter;
    float fDistance = vDir.Length();

    if (fDistance <= fSumRadius) return true;
    else return false;
}


k_Node* k_Collision::FindNodeToGo(k_Node* pNode, k_Object* pObj) // 선생님은 queue랑 do-while로 하셨는데 저어는 걍 재귀써봄
{
    if (pNode == nullptr) return nullptr;

    k_Node* final_node = pNode;

    for (int iChild = 0; iChild < 8; iChild++)
    {
        if (pNode->m_pChild[iChild] != nullptr)
        {
            if (BoxToInBox(pNode->m_pChild[iChild]->m_Box, pObj->m_Box))
            {
                final_node = FindNodeToGo(pNode->m_pChild[iChild], pObj);
            }
        }
    }
    return final_node;
}

void k_Collision::AddObject(k_Object* pObj, int iObjectType)
{
    k_Node* pFindNode = FindNodeToGo(OT.m_pRootNode, pObj);
    if (pFindNode != nullptr)
    {
        if (iObjectType == 0) pFindNode->m_StaticObjectList.push_back(pObj);
        else if (iObjectType == 1) pFindNode->m_DynamicObjectList.push_back(pObj);
    }
}

void k_Collision::DynamicObjectReset(k_Node* pNode)
{
    if (pNode == nullptr) return;
    pNode->m_DynamicObjectList.clear();
    for (int iChild = 0; iChild < 8; iChild++) 
    {
        DynamicObjectReset(pNode->m_pChild[iChild]);
    }
}

void k_Collision::GCO(k_Node* pNode, k_Object* pObj, std::vector<k_Object*>& list) //Get Collided Object 
{
    if (pNode == nullptr) return;

    for (int iObj = 0; iObj < pNode->m_StaticObjectList.size(); iObj++)
    {
        if (BoxToBox(pNode->m_StaticObjectList[iObj]->m_Box, pObj->m_Box))
        {
            list.push_back(pNode->m_StaticObjectList[iObj]);
        }
    }

    for (int iObj = 0; iObj < pNode->m_DynamicObjectList.size(); iObj++)
    {
        if (BoxToBox(pNode->m_DynamicObjectList[iObj]->m_Box, pObj->m_Box))
        {
            list.push_back(pNode->m_DynamicObjectList[iObj]);
        }
    }

    if (pNode->m_pChild[0] != nullptr)
    {
        for (int iChild = 0; iChild < 8; iChild++)
        {
            if (BoxToBox(pNode->m_pChild[iChild]->m_Box, pObj->m_Box))
            {
                GCO(pNode->m_pChild[iChild], pObj, list);
            }
        }
    }
}

std::vector<k_Object*> k_Collision::COL(k_Object* pObj) // Collided Object List
{
    std::vector<k_Object*> list;
    GCO(OT.m_pRootNode, pObj, list);
    return list;
};
