#pragma once
#include <iostream>
#include <vector>
#include "k_Object2D.h"



class k_Node2D
{
public:
    k_Rect m_sRect;
    int m_iDepth;
    std::vector<k_Object2D*> m_StaticObjectList;
    std::vector<k_Object2D*> m_DynamicObjectList;
    k_Node2D* m_pChild[4];
    k_Node2D* m_pParent;

public:
    k_Node2D() {}
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

class k_QuadTree
{
public:
    k_Node2D* m_pRootNode;

public:
    void BuildQuadTree(k_Node2D* root, float width, float height);

    k_QuadTree(float width, float height) 
    {
        this->m_pRootNode = new k_Node2D(nullptr, 0.0f, 0.0f, width, height);
        BuildQuadTree(this->m_pRootNode, width, height);
    };
    k_QuadTree() {}; // 디폴트 생성자
    ~k_QuadTree()
    {
        delete m_pRootNode;
    }
};
