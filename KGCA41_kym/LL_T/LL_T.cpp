//#include "LinkedList.h"
//#include <stdio.h>
////#include "k_Stack.h"
//
//int main()
//{
//    LinkedList<int> k_list;
//    k_list.insertFront(1);
//    k_list.insertFront(2);
//    k_list.insertFront(3);
//    k_list.insertBack(4);
//    k_list.insertBack(5);
//    k_list.insertBack(6);
//
//    Box<int>* ed = k_list.end();
//    k_list.insertTargetFront(7, ed);
//
//    Box<int>* bg = k_list.begin();
//    k_list.insertTargetBack(8, bg);
//
//    Box<int>* sacrificed = bg->m_pNext->m_pNext;
//    Box<int>* sacrificed2 = sacrificed->m_pNext;
//    k_list.death(sacrificed);
//    k_list.raft(sacrificed2);
//
//    Box<int>* idx = k_list[3];
//    k_list.death(idx);
//    //Box<int>* find = k_list.find(3); //이놈의연산자오버로딩이또문제네...대충find를다른걸로바꾸든가해야지
//    //k_list.death(find);
//
//    for (Box<int>* temp = k_list.m_pHead->m_pNext; temp != k_list.m_pTail; temp = temp->m_pNext) { printf("%d\n", temp->m_Data); }
//
//
//    printf("ㅂㄿㅇㅌ");
//
//
//
//
// //   k_Stack<int> kkk;
//	//kkk.push(1);
//	//kkk.push(2);
//	//kkk.top();
//	//kkk.push(3);
//	//kkk.push(4);
//	//kkk.push(5);
//	//kkk.top();
//
//	//kkk.pop();
// //   kkk.pop();
//	//kkk.top();
//	//kkk.pop_all();
//	//kkk.top();
// //   kkk.pop();
//}