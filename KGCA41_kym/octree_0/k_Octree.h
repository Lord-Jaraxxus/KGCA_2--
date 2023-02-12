#pragma once
#include <iostream>
#include <vector>
#include "k_Object.h"



class k_Node
{
public:
    k_Box m_Box;
    int m_iDepth;
    std::vector<k_Object*> m_StaticObjectList;
    std::vector<k_Object*> m_DynamicObjectList;
    k_Node* m_pChild[8]; // 나중에 노드를 밖으로 따로 뺄 예정, 쌤은 벡터 형식으로 바꾸심
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

class k_Octree
{
public:
    k_Node* m_pRootNode;

public:
    void BuildOctree(k_Node* parent, k_Vector Size);

    k_Octree(k_Vector Size)
    {
        k_Vector Pos(0.0f, 0.0f, 0.0f);
        this->m_pRootNode = new k_Node(nullptr, Pos, Size);
        BuildOctree(this->m_pRootNode, Size);
    };
    k_Octree() {}; // 디폴트 생성자
    ~k_Octree()
    {
        delete m_pRootNode;
    }
};
