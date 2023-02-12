#include "k_QuadTree.h"
#include "k_Octree.h"
#include <map>

enum k_CollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};

class k_Collision
{
public:
	k_QuadTree	QT;
	k_Octree	OT;

public:
	static k_CollisionType RectToRect(k_Rect& a, k_Rect& b); // 한 사각형이 다른 사각형과 겹치는지
	static bool	RectToInRect(k_Rect& a, k_Rect& b); // a가 b를 완전히 포함하는지
	static bool CircleToCircle(k_Circle &a, k_Circle &b);

	k_Node2D* FindNodeToGo(k_Node2D* pNode, k_Object2D* pObj);
	void AddObject(k_Object2D* pObj, int iObjectType); // 일단 0이면 스태틱, 1이면 다이나믹
	void DynamicObjectReset(k_Node2D* pNode);

	void GCO(k_Node2D* pNode, k_Object2D* pObj, std::vector<k_Object2D*>& list); //Get Collided Object
	std::vector<k_Object2D*> COL(k_Object2D* pObj); // Collided Object List


// 이 밑으로는 3D, 옥트리용
public:
	static k_CollisionType BoxToBox(k_Box& a, k_Box& b); // 한 박스가 다른 박스와 겹치는지
	static bool	BoxToInBox(k_Box& a, k_Box& b); // a가 b를 완전히 포함하는지
	static bool SphereToSphere(k_Sphere& a, k_Sphere& b);

	// 이 밑으로는 일단 오버로딩? 해도 괜찮겠지? 굳이 2D 3D 이름붙이기 좀 귀찮으니까
	k_Node* FindNodeToGo(k_Node* pNode, k_Object* pObj); 
	void AddObject(k_Object* pObj, int iObjectType); // 0이면 스태틱, 1이면 다이나믹
	void DynamicObjectReset(k_Node* pNode);

	std::vector<k_Object*> COL(k_Object* pObj); // Collided Object List
	void GCO(k_Node* pNode, k_Object* pObj, std::vector<k_Object*>& list); //Get Collided Object
};


