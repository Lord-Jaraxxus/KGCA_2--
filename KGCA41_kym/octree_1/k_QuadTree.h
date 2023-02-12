#pragma once
#include <iostream>
#include <vector>
#include "k_Node_Factory.h"

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
