#pragma once

#include "k_Object.h"
#include "k_Object2D.h"


class k_Node_Factory
{
public:
    k_Node_Factory() {};
    virtual ~k_Node_Factory() {}
};

class k_Node2D : k_Node_Factory
{
public:
    k_Rect m_sRect;
    int m_iDepth;
    std::vector<k_Object2D*> m_StaticObjectList;
    std::vector<k_Object2D*> m_DynamicObjectList;
    k_Node2D* m_pChild[4];
    k_Node2D* m_pParent;

public:
    k_Node2D() {};
    k_Node2D(k_Node2D* pParent, float x, float y, float w, float h)
    {
        m_sRect.Set(x, y, w, h);
        m_iDepth = 0;
        m_pParent = nullptr;
        if (pParent != nullptr)
        {
            m_pParent = pParent;
            m_iDepth = pParent->m_iDepth + 1;
        }
    }
    ~k_Node2D()
    {
        delete m_pChild[0];
        delete m_pChild[1];
        delete m_pChild[2];
        delete m_pChild[3];
    }
};

class k_Node : k_Node_Factory
{
public:
    k_Box m_Box;
    int m_iDepth;
    std::vector<k_Object*> m_StaticObjectList;
    std::vector<k_Object*> m_DynamicObjectList;
    k_Node* m_pChild[8];
    k_Node* m_pParent;

public:
    k_Node() {}
    k_Node(k_Node* pParent, k_Vector Pos, k_Vector Size)
    {
        m_Box.Set(Pos, Size);
        m_iDepth = 0;
        m_pParent = nullptr;
        if (pParent != nullptr)
        {
            m_pParent = pParent;
            m_iDepth = pParent->m_iDepth + 1;
        }
    }
    ~k_Node()
    {
        for (int iChild = 0; iChild < 8; iChild++)
        {
            delete m_pChild[iChild];
        }
    }
};
