#pragma once
#include <vector> // fabs
#include "k_Vector.h"

struct Box
{
    k_Vector vMin;
    k_Vector vSize;
};

struct k_Box : Box
{
    k_Vector vMax;
    k_Vector vCenter;

    bool   operator == (k_Box& dest) // 연산자 재정의
    {
        if (vMin == dest.vMin)
        {
            if (vSize == dest.vSize)
            {
                return true;
            }
        }
        return false;
    }

    k_Box() {} // 디폴트 생성자

    k_Box(k_Vector vPos, k_Vector vSize)
    {
        Set(vPos, vSize);
    }

    void  Set(k_Vector vPos, k_Vector vSize)
    {
        vMin = vPos;
        this->vSize = vSize;
        vMax = vMin + vSize;
        vCenter = (vMin + vMax) / 2.0f;
    }
};

struct k_Sphere
{
    k_Vector vCenter;
    float fRadius;

    void Set(k_Vector vC, float radius)
    {
        vCenter = vC;
        fRadius = radius;
    }

    k_Sphere()
    {
        Set(k_Vector(0,0,0), 0.0f);
    }
    k_Sphere(k_Vector vC, float radius)
    {
        Set(vC, radius);
    }
};

class k_Object
{
public:
    k_Box    m_Box;
    k_Sphere m_Sphere;
    k_Vector m_vDirection;

public:
    k_Vector m_vForces;       // 알짜힘, 플레이어에게 가해지는 알짜힘
    k_Vector m_vAcceleration; // 가속도, 가속도는 벡터인가? 벡터라고 하는데. 왜 벡터지? 
    k_Vector m_vVelocity;     // 속도, 얘는 벡터라고 치는게 대충 이해가 가는데
    float m_fMass;              // 무게, 질량? f=ma에서 m을 담당 ㅗㅜㅑ
    float m_fFriction;          // 마찰계수
    float m_fSpeed;             // 속력, 얘는 스칼라

    void   SetPosition(k_Vector Pos, k_Vector Size)
    {
        m_Box.Set(Pos, Size);
        float fRadius = m_Box.vSize.Length()/2; // 반지름
        m_Sphere.Set(m_Box.vCenter, fRadius);
    }

    virtual void    Frame(float fDeltaTime, float fGameTime) {};
    virtual void    AddForces(k_Vector f);

    k_Object()
    {
        k_Vector vPos, vSize;
        vPos.Set((rand() % 90), (rand() % 90), (rand() % 90));
        vSize.Set(5.0f + (rand() % 5), 5.0f + (rand() % 5), 5.0f + (rand() % 5));
        SetPosition(vPos, vSize);

        m_vDirection.Set(1.0f + rand() % 9, 1.0f + rand() % 9, 1.0f + rand() % 9);
        m_vDirection.Normalize();
        m_vForces.Set(1.0f, 1.0f, 1.0f);
        m_vAcceleration.Set(0.0f, 0.0f, 0.0f);
        m_vVelocity.Set(0.0f, 0.0f, 0.0f);  
        
        m_fMass = 1.0f;
        m_fFriction = 0.1f;
        m_fSpeed = 0.0f;
    }
};
