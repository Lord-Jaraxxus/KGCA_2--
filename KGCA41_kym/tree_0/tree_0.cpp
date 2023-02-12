#include <iostream>
#include <queue>
#include "Node.h"

int g_iMaxDepth = 3;
int g_iCounter = 0;

std::queue<Node*> g_Queue; //레베루 오다에 쓰기 위함


Node* NewNode()
{
    Node* new_node = new Node;
    new_node->m_iIndex = g_iCounter;
    g_iCounter++;

    return new_node;
}

void Build(Node* parent)
{
    if (parent->m_iDepth == g_iMaxDepth) return;


    Node* leftChild = NewNode();

    parent->m_pChild[0] = leftChild;
    leftChild->m_pParent = parent;
    leftChild->m_iDepth = parent->m_iDepth + 1;

    Build(leftChild);   


    Node* rightChild = NewNode();

    parent->m_pChild[1] = rightChild;
    rightChild->m_pParent = parent;
    rightChild->m_iDepth = parent->m_iDepth + 1;

    Build(rightChild);
}

void Build_LO(Node* parent)
{
    if (parent->m_iDepth == g_iMaxDepth) 
    {
        while (!g_Queue.empty()) { g_Queue.pop(); } // 큐의 잔여물을 비워줌, 좀 무식한데 일단 이렇게 하자고
        return;
    }

    Node* leftChild = NewNode();
    parent->m_pChild[0] = leftChild;
    leftChild->m_pParent = parent;
    leftChild->m_iDepth = parent->m_iDepth + 1;

    Node* rightChild = NewNode();
    parent->m_pChild[1] = rightChild;
    rightChild->m_pParent = parent;
    rightChild->m_iDepth = parent->m_iDepth + 1;

    g_Queue.push(parent->m_pChild[0]);
    g_Queue.push(parent->m_pChild[1]);
    Node* pNext = g_Queue.front();
    g_Queue.pop();
    Build_LO(pNext);
}

void Travel(Node* pParent) 
{
    if (pParent == nullptr) return;

    std::cout << " " << pParent->m_iIndex; //- 전위(프리 오다)
    Travel(pParent->m_pChild[0]);
    //std::cout << " " << pParent->m_iIndex; //- 중위(인 오다) 
    Travel(pParent->m_pChild[1]);
    //std::cout << " " << pParent->m_iIndex; //- 후위(포스트 오다)
}

void Travel_LO(Node* pParent)
{
    if (pParent == nullptr) return;

    std::cout << " " << pParent->m_iIndex;
    g_Queue.push(pParent->m_pChild[0]);
    g_Queue.push(pParent->m_pChild[1]);

    Node* pNext = g_Queue.front();
    g_Queue.pop();
    Travel_LO(pNext);
}

void ReIndexing(Node* pParent)
{
    if (pParent == nullptr) return;

    ReIndexing(pParent->m_pChild[0]);
    pParent->m_iIndex = g_iCounter;  // 얘 위치에 따라서 
    g_iCounter++;                    // 전위,중위,후위로
    ReIndexing(pParent->m_pChild[1]);
}

void ReIndexing_LO(Node* pParent) //공식을 사용해서 레베루오다로
{
    if (pParent->m_pChild[0] == nullptr || pParent->m_pChild[1] == nullptr) return;
    pParent->m_pChild[0]->m_iIndex = (pParent->m_iIndex)*2 + 1;
    pParent->m_pChild[1]->m_iIndex = (pParent->m_iIndex + 1) * 2;

    ReIndexing_LO(pParent->m_pChild[0]);
    ReIndexing_LO(pParent->m_pChild[1]);
}

void ReIndexing_LO_Q(Node* pParent) //큐를 사용해서 레베루오다로
{
    if (pParent == nullptr) return;

    pParent->m_iIndex = g_iCounter; 
    g_iCounter++;

    if (pParent->m_pChild[0] != nullptr && pParent->m_pChild[1] != nullptr)
    {
        g_Queue.push(pParent->m_pChild[0]);
        g_Queue.push(pParent->m_pChild[1]);
    }

    if (!g_Queue.empty()) 
    {
        Node* pNext = g_Queue.front();
        g_Queue.pop();
        ReIndexing_LO_Q(pNext);
    }
}

void DeleteAll(Node** pParent) //후위순회하며 전부 삭제
{
   // if (pParent == nullptr) return;
   // DeleteAll(pParent->m_pChild[0]);
   // DeleteAll(pParent->m_pChild[1]);

    //int iTest = pParent->m_iIndex;
    delete *pParent;
    *pParent = nullptr;
   // std::cout << " " << iTest;
}

void DeleteAll_LO(Node* pParent) 
{
    if (pParent == nullptr) return;

    if (pParent->m_pChild[0] != nullptr && pParent->m_pChild[1] != nullptr)
    {
        g_Queue.push(pParent->m_pChild[0]);
        g_Queue.push(pParent->m_pChild[1]);
    }

    delete pParent;
    pParent = nullptr;

    if (!g_Queue.empty())
    {
        Node* pNext = g_Queue.front();
        g_Queue.pop();
        DeleteAll_LO(pNext);
    }
}

int main()
{
    Node* root = NewNode();

    Build_LO(root);

    //g_iCounter = 0;
    //ReIndexing_LO_Q(root);
    //Travel_LO(root);

    DeleteAll(&root);
    //root = nullptr;
    std::cout << "\nHello, World!\n";
}

