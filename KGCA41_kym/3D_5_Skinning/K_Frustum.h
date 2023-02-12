#pragma once

#include "K_Shape.h"
#include "TMath.h"
#include "K_Collision.h"

enum K_POSITION
{
	K_BACK = 0,
	K_FRONT ,
	K_ONPLANE ,
	K_SPANNING ,
};

struct K_Plane
{
	float a, b, c, d;
	void  Create(k_Vector v0, k_Vector v1, k_Vector v2)
	{
		k_Vector vE0 = v1 - v0;
		k_Vector vE1 = v2 - v0;
		k_Vector vNormal = vE0 ^ vE1;
		vNormal.Normalize();
		a = vNormal.x;
		b = vNormal.y;
		c = vNormal.z;
		d = -(vNormal | v0);
		// p = n.x*v.x + n.y*v.y + v.z*v.z + d=0;
		// p = n.x*v.x + n.y*v.y + v.z*v.z = -d;
		// p = (n.x*v.x + n.y*v.y + v.z*v.z)*-1.0f = d;
		// p = -(n dot v) = d;
	}
	void  Create(k_Vector  normal, k_Vector  v0)
	{
		normal.Normalize();
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -(normal | v0);
	}
};

class K_Frustum
{
	K_Matrix   m_matView;
	K_Matrix   m_matProj;

public:
	TBASIS_EX::TVector3  m_vFrustum[8];
	K_Plane   m_Plane[6];

public:
	void	 CreateFrustum(K_Matrix* matView, K_Matrix* matProj);
	bool	 ClassifyPoint(k_Vector v);
	K_POSITION	 ClassifySphere(K_Sphere s);
	//bool	 ClassifyAABB(T_AABB v);
	//bool	 ClassifyOBB(T_OBB v);
	K_POSITION	 ClassifyBox(K_BOX box);
};

