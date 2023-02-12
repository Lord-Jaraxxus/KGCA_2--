#include <iostream>
#include "Node.h"

int g_iData[10] = { 50,22,66,35,43,59,42,78,63,14 };
int g_iCount = 0;
int g_iFlag = 0;

Node* NewNode()
{
    Node* new_node = new Node;
    return new_node;
}

Node* Build_BST(Node* pParent)
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
                pParent->m_pChild[0] = Build_BST(pParent->m_pChild[0]);
            }
            else
            {
                pParent->m_pChild[1] = Build_BST(pParent->m_pChild[1]);
            }
        }
        return pParent;
}

Node* Search_BST(Node* pParent, int iValue, Node* &pSearchedParent)
{
    if (pParent == nullptr)
    {
        std::cout << "찾는 숫자가 없음";
        return nullptr;                               
    }

    if (pParent->m_iData == iValue)  return pParent;                                                
    else if (iValue < pParent->m_iData)
    {
        pSearchedParent = pParent;
        Search_BST(pParent->m_pChild[0], iValue, pSearchedParent);
    }
    else
    {
        pSearchedParent = pParent;
        Search_BST(pParent->m_pChild[1], iValue, pSearchedParent);
    }
}

Node* LeftSlide(Node* pStart, Node* &pParent) 
{
    if (pStart == NULL) return NULL;
    else if (pStart->m_pChild[0] == NULL) return pStart; 
    else 
    {
        pParent = pStart;
        LeftSlide(pStart->m_pChild[0], pParent);
    }
        
}

Node* RightSlide(Node* pStart)
{
    if (pStart == NULL)
        return NULL;
    else if (pStart->m_pChild[0] == NULL)
        return pStart;
    else
        return RightSlide(pStart->m_pChild[0]);
}

void DeleteNode_BST(int iData, Node* pRoot) 
{
    Node* pTempParent = nullptr;
    Node* pTemp = Search_BST(pRoot, iData, pTempParent);

    if (pTemp->m_pChild[0] == nullptr && pTemp->m_pChild[1] == nullptr) // 1.목표 노드의 자식이 하나도 없을때
    {
        delete pTemp;
        // pTemp = nullptr; <-얘는 사실 굳이 해줄 필요가 없음, 들어온 포인터 변수와 pTemp는 다른 존재기 때문
    }

    else if (pTemp->m_pChild[0] && pTemp->m_pChild[1]) // 목표 노드의 자식이 둘 다 있는 경우
    {
        Node* pLastLeftParent = nullptr;
        Node* pLastLeft = LeftSlide(pTemp->m_pChild[1], pLastLeftParent); // 오른쪽 자식의 왼쪽으로 쭉 내려가서 마지막 노드를 pLastLeft에 대입

        if (pLastLeft == pTemp->m_pChild[1]) // 2.오른쪽 자식의 왼쪽 자식이 없음, 즉 꺾을 수 없을 때
        {
            pTemp->m_iData = pTemp->m_pChild[1]->m_iData;
            Node* pSacrificed = pTemp->m_pChild[1];
            pTemp->m_pChild[1] = pTemp->m_pChild[1]->m_pChild[1];
            delete pSacrificed;
        }

        else if (pLastLeft->m_pChild[0] == nullptr && pLastLeft->m_pChild[1] == nullptr) // 3.마지막 왼쪽 노드가 말단 노드일 때
        {
            pTemp->m_iData = pLastLeft->m_iData;
            delete pLastLeft;
        }

        else // 4.마지막 왼쪽 노드가 말단 노드가 아닐때, 즉 오른쪽 자식이 있을 때
        {
            pTemp->m_iData = pLastLeft->m_iData;
            pLastLeftParent->m_pChild[0] = pLastLeft->m_pChild[1];
            delete pLastLeft;
        }
    }
     
    else // 목표 노드의 자식이 하나인 경우
    {
        if (pTempParent->m_pChild[0] == pTemp) // 목표 노드가 부모의 왼쪽 자식인 경우
        {
            if (pTemp->m_pChild[0] != nullptr) // 목표 노드가 왼쪽 자식만 있는 경우
            {
                pTempParent->m_pChild[0] = pTemp->m_pChild[0];
                delete pTemp;
            } 
            else // 목표 노드가 오른쪽 자식만 있는 경우
            { 
                pTempParent->m_pChild[0] = pTemp->m_pChild[1]; 
                delete pTemp;
            } 
        }

        else // 목표 노드가 부모의 오른쪽 자식인 경우
        {
            if (pTemp->m_pChild[0] != nullptr) // 목표 노드가 왼쪽 자식만 있는 경우
            {
                pTempParent->m_pChild[1] = pTemp->m_pChild[0];
                delete pTemp;
            }
            else // 목표 노드가 오른쪽 자식만 있는 경우
            {
                pTempParent->m_pChild[1] = pTemp->m_pChild[1]; 
                delete pTemp;
            }
        }
    }
}



int main()
{
    Node* root = nullptr;
    
    while (g_iCount < 10) { root = Build_BST(root); }
    
    //Node* find = Search_BST(root, 88);

    DeleteNode_BST(50, root);
    std::cout << "Hello, World!";

}
