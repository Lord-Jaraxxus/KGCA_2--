#pragma once
#include <iostream>
#include <vector>
#include "k_Object.h"



class k_Node
{
public:
    k_Rect m_sRect;
    int m_iDepth;
    std::vector<k_Object*> m_StaticObjectList;
    std::vector<k_Object*> m_DynamicObjectList;
    k_Node* m_pChild[4];
    k_Node* m_pParent;

public:
    k_Node() {}
    k_Node(k_Node* pParent, float x, float y, float w, float h)
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
    ~k_Node()
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
    k_Node* m_pRootNode;

public:
    void BuildQuadTree(k_Node* root, float width, float height);

    k_QuadTree(float width, float height) 
    {
        this->m_pRootNode = new k_Node(nullptr, 0.0f, 0.0f, width, height);
        BuildQuadTree(this->m_pRootNode, width, height);
    };
    k_QuadTree() {}; // 디폴트 생성자
    ~k_QuadTree()
    {
        delete m_pRootNode;
    }
};
