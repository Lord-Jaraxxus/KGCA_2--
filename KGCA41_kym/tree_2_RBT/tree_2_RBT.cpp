#include <iostream>
#include "Node.h"

// 테스트용 

//int g_iData[10] = { 50,22,66,35,43,59,42,78,63,14 }; 
int g_iData[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int g_iCount = 0;
int g_iFlag = 0;

Node* g_pRoot = nullptr;


Node* NewNode()
{
    Node* new_node = new Node;
    return new_node;
}

Node* AddNode_BST(Node* pParent)
{
    if (pParent == nullptr)
    {
        pParent = NewNode();
        pParent->m_iData = g_iData[g_iCount];
        pParent->m_iIndex = g_iCount;
        g_iCount++;
    }

    else if (g_iCount < 10)
    {
        if (g_iData[g_iCount] < pParent->m_iData)
        {
            pParent->m_pChild[0] = AddNode_BST(pParent->m_pChild[0]);
            pParent->m_pChild[0]->m_pParent = pParent;
        }
        else
        {
            pParent->m_pChild[1] = AddNode_BST(pParent->m_pChild[1]);
            pParent->m_pChild[1]->m_pParent = pParent;
        }
    }
    return pParent;
}

int GetColor(Node* node)
{
    if (node == nullptr) return BLACK;
    else return node->m_iColor;
}

void LeftRotate(Node* node)
{
    Node* pPr = node->m_pParent;

    //앞으로 연결이 끊길 노드들을 미리 세이브해둠
    Node* pLc = node->m_pChild[0]; // 노드의 원래 왼쪽 자식 (p Left chid)
    Node* pGpr = pPr->m_pParent; // 조부모 (p Grand parent)

    node->m_pChild[0] = pPr; // 노드의 왼쪽 자식을 부모로 변경
    pPr->m_pParent = node; // 부모의 부모를 노드로 변경, 사실상 위랑 세트

    pPr->m_pChild[1] = pLc; // 내려온 pPr의 오른쪽 자식을 떨어져나간 노드의 왼쪽 자식으로 변경
    if (pLc != nullptr) pLc->m_pParent = pPr; // 위랑 세트

    node->m_pParent = pGpr; // 노드의 부모를 조부모로 변경
    if (pGpr != nullptr)
    {
        if (pPr == pGpr->m_pChild[0]) pGpr->m_pChild[0] = node; // 부모가 조부모의 왼쪽이었다면 조부모의 왼쪽 자식을 노드로 변경
        else if (pPr == pGpr->m_pChild[1]) pGpr->m_pChild[1] = node; // 부모가 조부모의 오른쪽이었다면 조부모의 오른쪽 자식을 노드로 변경
    } // 얘도 위에 한줄이랑 세트
    else // 노드의 부모가 없다면, 노드가 루트노드라는 뜻
    {
        g_pRoot = node;
    }
}

void RightRotate(Node* node)
{
    Node* pPr = node->m_pParent;

    //앞으로 연결이 끊길 노드들을 미리 세이브해둠
    Node* pRc = node->m_pChild[1]; // 노드의 원래 오른쪽 자식 (p Right chid)
    Node* pGpr = pPr->m_pParent; // 조부모 (p Grand parent)

    node->m_pChild[1] = pPr; // 노드의 오른쪽 자식을 부모로 변경
    pPr->m_pParent = node; // 부모의 부모를 노드로 변경, 사실상 위랑 세트

    pPr->m_pChild[0] = pRc; // 내려온 pPr의 왼쪽 자식을 떨어져나간 노드의 오른쪽 자식으로 변경
    if (pRc != nullptr)pRc->m_pParent = pPr; // 위랑 세트

    node->m_pParent = pGpr; // 노드의 부모를 조부모로 변경
    if (pGpr != nullptr)
    {
        if (pPr == pGpr->m_pChild[0]) pGpr->m_pChild[0] = node; // 부모가 조부모의 왼쪽이었다면 조부모의 왼쪽 자식을 노드로 변경
        else if (pPr == pGpr->m_pChild[1]) pGpr->m_pChild[1] = node; // 부모가 조부모의 오른쪽이었다면 조부모의 오른쪽 자식을 노드로 변경
    } // 얘도 위에 한줄이랑 세트
    else // 노드의 부모가 없다면, 노드가 루트노드라는 뜻
    {
        g_pRoot = node;
    }
}

void FixRBT(Node* ptr)
{
    Node* pParent = nullptr;
    Node* pGrandParent = nullptr;
    Node* pUncle = nullptr;

    while (ptr != g_pRoot && GetColor(ptr) == RED && GetColor(ptr->m_pParent) == RED)
    {
        pParent = ptr->m_pParent;
        pGrandParent = pParent->m_pParent;

        if (pParent == pGrandParent->m_pChild[0]) // 부모가 조부모의 왼쪽 자식일 때 
        {
            pUncle = pGrandParent->m_pChild[1]; // 삼촌은 조부모의 오른쪽 자식, 삼촌이 없을수도 있음
            if (GetColor(pUncle) == RED)
            {
                pParent->m_iColor = BLACK;
                pUncle->m_iColor = BLACK;
                pGrandParent->m_iColor = RED;
                ptr = pGrandParent;
            }
            else if (GetColor(pUncle) == BLACK)
            {
                if (ptr == pParent->m_pChild[0])
                {
                    RightRotate(ptr->m_pParent);
                    pGrandParent->m_iColor = RED;
                    pParent->m_iColor = BLACK; // 쌤은 스왑으로 처리하신 부분
                    ptr = pParent;
                }
                else if (ptr == pParent->m_pChild[1])
                {
                    LeftRotate(ptr);
                    RightRotate(ptr);
                    pGrandParent->m_iColor = RED;
                    ptr->m_iColor = BLACK;
                }
            }
        }
        else if (pParent == pGrandParent->m_pChild[1]) // 부모가 조부모의 오른쪽 자식일 때, else 써도 되는데 보기 편하라고
        {
            pUncle = pGrandParent->m_pChild[0]; // 삼촌은 조부모의 왼쪽 자식, 삼촌이 없을수도 있음
            if (GetColor(pUncle) == RED)
            {
                pParent->m_iColor = BLACK;
                pUncle->m_iColor = BLACK;
                pGrandParent->m_iColor = RED;
                ptr = pGrandParent;
            }
            else if (GetColor(pUncle) == BLACK)
            {
                if (ptr == pParent->m_pChild[0])
                {
                    RightRotate(ptr);
                    LeftRotate(ptr);
                    pGrandParent->m_iColor = RED;
                    ptr->m_iColor = BLACK;
                }
                else if (ptr == pParent->m_pChild[1])
                {
                    LeftRotate(ptr->m_pParent);
                    pGrandParent->m_iColor = RED;
                    pParent->m_iColor = BLACK; // 쌤은 스왑으로 처리하신 부분
                    ptr = pParent;
                }
            }
        }
    }
    g_pRoot->m_iColor = BLACK;
}

Node* Search_BST(Node* pParent, int iValue)
{
    if (pParent == nullptr)
    {
        std::cout << "찾는 숫자가 없음";
        return nullptr;
    }

    if (pParent->m_iData == iValue)  return pParent;
    else if (iValue < pParent->m_iData)
    {
        Search_BST(pParent->m_pChild[0], iValue);
    }
    else
    {
        Search_BST(pParent->m_pChild[1], iValue);
    }
}




int main()
{
    Node* root = nullptr;
    root = AddNode_BST(root);
    g_pRoot = root;
    FixRBT(Search_BST(root, g_iData[g_iCount - 1]));

    while (g_iCount < 10)
    {
        root = AddNode_BST(g_pRoot);
        FixRBT(Search_BST(root, g_iData[g_iCount - 1]));
    }

    std::cout << "Hello, World!";
    std::cout << "Hello, World!";

}
