#include "k_QuadTree.h"
#include <queue>

enum k_CollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};

class k_Collision
{
public:
	k_QuadTree QT;


public:
	static bool	RectToInRect(k_Rect& a, k_Rect& b); // a�� b�� ������ �����ϴ���
	static k_CollisionType RectToRect(k_Rect& a, k_Rect& b); // �� �簢���� �ٸ� �簢���� ��ġ����
	static bool CircleToCircle(k_Circle &a, k_Circle &b);

	k_Node* FindNodeToGo(k_Node* pNode, k_Object* pObj);
	void AddStaticObject(k_Object* pObj);
	void AddDynamicObject(k_Object* pObj);
	void DynamicObjectReset(k_Node* pNode);

	std::vector<k_Object*> COL(k_Object* pObj); // Collided Object List
	void GCO(k_Node* pNode, k_Object* pObj, std::vector<k_Object*> &list); //Get Collided Object
};


