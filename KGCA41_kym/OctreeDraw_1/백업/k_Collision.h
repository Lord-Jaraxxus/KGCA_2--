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
	static k_CollisionType RectToRect(k_Rect& a, k_Rect& b); // �� �簢���� �ٸ� �簢���� ��ġ����
	static bool	RectToInRect(k_Rect& a, k_Rect& b); // a�� b�� ������ �����ϴ���
	static bool CircleToCircle(k_Circle &a, k_Circle &b);

	k_Node2D* FindNodeToGo(k_Node2D* pNode, k_Object2D* pObj);
	void AddObject(k_Object2D* pObj, int iObjectType); // �ϴ� 0�̸� ����ƽ, 1�̸� ���̳���
	void DynamicObjectReset(k_Node2D* pNode);

	void GCO(k_Node2D* pNode, k_Object2D* pObj, std::vector<k_Object2D*>& list); //Get Collided Object
	std::vector<k_Object2D*> COL(k_Object2D* pObj); // Collided Object List


// �� �����δ� 3D, ��Ʈ����
public:
	static k_CollisionType BoxToBox(k_Box& a, k_Box& b); // �� �ڽ��� �ٸ� �ڽ��� ��ġ����
	static bool	BoxToInBox(k_Box& a, k_Box& b); // a�� b�� ������ �����ϴ���
	static bool SphereToSphere(k_Sphere& a, k_Sphere& b);

	// �� �����δ� �ϴ� �����ε�? �ص� ��������? ���� 2D 3D �̸����̱� �� �������ϱ�
	k_Node* FindNodeToGo(k_Node* pNode, k_Object* pObj); 
	void AddObject(k_Object* pObj, int iObjectType); // 0�̸� ����ƽ, 1�̸� ���̳���
	void DynamicObjectReset(k_Node* pNode);

	std::vector<k_Object*> COL(k_Object* pObj); // Collided Object List
	void GCO(k_Node* pNode, k_Object* pObj, std::vector<k_Object*>& list); //Get Collided Object
};


