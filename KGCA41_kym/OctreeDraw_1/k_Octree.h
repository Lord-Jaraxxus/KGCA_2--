#pragma once
#include <iostream>
#include <vector>
#include "k_Node_Factory.h"

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
