#pragma once
#include <vector> // fabs
#include "K_BaseObject.h"

struct Rect
{
    float x1; // 작은 x좌표
    float y1; // 작은 y좌표
    float w; // width
    float h; // heigt
};

struct k_Rect : Rect
{
    float x2; // 큰 x좌표
    float y2; // 큰 y좌표
    float cx; // 중심 x좌표
    float cy; // 중심 y좌표

    bool   operator == (k_Rect& dest) // 연산자 재정의, 허용오차까지는 ㅇㅋ하는 비교
    {
        if (fabs(x1 - dest.x1) < k_Epsilon)
        {
            if (fabs(y1 - dest.y1) < k_Epsilon)
            {
                if (fabs(w - dest.w) < k_Epsilon)
                {
                    if (fabs(h - dest.h) < k_Epsilon)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    k_Rect() {} // 디폴트 생성자

    k_Rect(float x, float y, float w, float h)
    {
        Set(x, y, w, h);
    }

    void  Set(float x, float y, float w, float h)
    {
        this->x1 = x;
        this->y1 = y;
        this->w = w;
        this->h = h;
        this->x2 = x + w;
        this->y2 = y + h;
        this->cx = x + w / 2.0f;
        this->cy = y + h / 2.0f;
    }
};

struct k_Circle
{
    float cx = 0;
    float cy = 0;
    float fRadius = 0;

    void Set(float x, float y, float r)
    {
        cx = x;
        cy = y;
        fRadius = r;
    }

    k_Circle() {};
    k_Circle(float x, float y, float r)
    {
        cx = x;
        cy = y;
        fRadius = r;
    }
};

class k_Object2D : public K_BaseObject
{
public:
    k_Rect m_sRect;
    k_Circle m_sCir;
    k_Vector2D m_vDirection;

public:
    k_Vector2D m_vForces;       // 알짜힘, 플레이어에게 가해지는 알짜힘
    k_Vector2D m_vAcceleration; // 가속도, 가속도는 벡터인가? 벡터라고 하는데. 왜 벡터지? 
    k_Vector2D m_vVelocity;     // 속도, 얘는 벡터라고 치는게 대충 이해가 가는데
    float m_fMass;              // 무게, 질량? f=ma에서 m을 담당 ㅗㅜㅑ
    float m_fFriction;          // 마찰계수
    float m_fSpeed;             // 속력, 얘는 스칼라

    void   SetPosition(float x, float y, float w, float h);
    
    void SetForces(float f);

    void SetVector(k_Vector2D& v, float x, float y);

    virtual void    Frame(float fDeltaTime, float fGameTime) {};
    virtual void    AddForces(k_Vector2D f);

    k_Object2D();
};
