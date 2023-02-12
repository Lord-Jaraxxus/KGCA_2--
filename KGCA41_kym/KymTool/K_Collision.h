#pragma once
#include "k_Vector.h"

struct K_Sphere
{
    k_Vector vCenter;
    float fRadius;
    K_Sphere() {};

    K_Sphere(k_Vector vC, float r) 
    {
        vCenter = vC;
        fRadius = r;
    };
};

struct K_AABB
{
    k_Vector vMin;
    k_Vector vMax;
};
struct K_OBB
{
    k_Vector vCenter;
    k_Vector vAxis[3];
    float   fDistance[3];
};
struct K_BOX
{
    k_Vector vPos[8];

    // aabb
    k_Vector vMin;
    k_Vector vMax;

    // obb    
    k_Vector vCenter;
    k_Vector vAxis[3];
    float    fExtent[3];
};

class K_Collision
{
};

