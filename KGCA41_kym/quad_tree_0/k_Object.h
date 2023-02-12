#pragma once
#include <vector> // fabs
#include "k_Vector.h"

struct Rect
{
    float x1; // ���� x��ǥ
    float y1; // ���� y��ǥ
    float w; // width
    float h; // heigt
};

struct k_Rect : Rect
{
    float x2; // ū x��ǥ
    float y2; // ū y��ǥ
    float cx; // �߽� x��ǥ
    float cy; // �߽� y��ǥ

    bool   operator == (k_Rect& dest) // ������ ������, ������������ �����ϴ� ��
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

    k_Rect() {} // ����Ʈ ������

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

class k_Object
{
public:
    k_Rect m_sRect;
    k_Circle m_sCir;
    k_Vector2D m_vDirection;

public:
    k_Vector2D m_vForces;       // ��¥��, �÷��̾�� �������� ��¥��
    k_Vector2D m_vAcceleration; // ���ӵ�, ���ӵ��� �����ΰ�? ���Ͷ�� �ϴµ�. �� ������? 
    k_Vector2D m_vVelocity;     // �ӵ�, ��� ���Ͷ�� ġ�°� ���� ���ذ� ���µ�
    float m_fMass;              // ����, ����? f=ma���� m�� ��� �Ǥ̤�
    float m_fFriction;          // �������
    float m_fSpeed;             // �ӷ�, ��� ��Į��

    void   SetPosition(float x, float y, float w, float h)
    {
        m_sRect.Set(x, y, w, h);

        float hw = w / 2.0f;
        float hh = h / 2.0f;
        float r1 = sqrt(hw * hw + hh * hh);
        m_sCir.Set(x + hw, y + hh, r1);
    }

    void SetVector(k_Vector2D &v, float x, float y) 
    {
        v.x = x;
        v.y = y;
    }

    virtual void    Frame(float fDeltaTime, float fGameTime) {};
    virtual void    AddForces(k_Vector2D f);

    k_Object()
    {
        SetPosition
        (
            (rand() % 90),
            (rand() % 90),
            5.0f + (rand() % 5),
            5.0f + (rand() % 5)
        );
        SetVector(m_vDirection, 1.0f + rand()%9, 1.0f + rand() % 9);
        SetVector(m_vForces, 1.0f, 1.0f);
        SetVector(m_vAcceleration, 0.0f, 0.0f); // �ٵ� �긦 ���� �����ϰ� �ʱ�ȭ�� ������ �ֳ�? �Լ��ȿ��� ó���ص�..
        SetVector(m_vVelocity, 0.0f, 0.0f); // ��� �� �߿�����
        m_fMass = 1.0f;
        m_fFriction = 0.1f;
    }
};
