#include "k_Player2D.h"

void   k_Player2D::Frame(float fDeltaTime, float fGameTime)
{
    {
        k_Vector2D vStart(m_sRect.x1, m_sRect.y1); // ���ۺ���

        m_vAcceleration = m_vForces / m_fMass; // 1�� ���� ���ӵ�
        //k_Vector2D vFriction = m_vDirection * m_fFriction;
        k_Vector2D vLinearAcc = m_vAcceleration * fDeltaTime; 
        k_Vector2D vV0 = m_vVelocity;   // ó�� �ӵ�
        k_Vector2D vV1 = m_vVelocity + vLinearAcc; // fDeltaTime�� ���� ������ �ӵ�
        k_Vector2D vAverageVelocity = (vV0 + vV1) / 2; // fDeltaTime ������ ��� �ӵ�
        k_Vector2D vDisplacement = vAverageVelocity * fDeltaTime; // ����

        m_vVelocity += vLinearAcc;
        vStart += vDisplacement;
        

        m_sRect.x1 = vStart.x;
        m_sRect.y1 = vStart.y;

        if (vStart.x + m_sRect.w > 100.0f)
        {
            m_sRect.x1 = 200.0f - (vStart.x + 2 * m_sRect.w);
            m_vForces.x *= -1.0f;
            m_vVelocity.x *= -1.0f;
        }
        if (vStart.x < 0.0f)
        {
            m_sRect.x1 = -(vStart.x);
            m_vForces.x *= -1.0f;
            m_vVelocity.x *= -1.0f;
        }
        if (vStart.y + m_sRect.h > 100.0f)
        {
            m_sRect.y1 = 200.0f - (vStart.y + 2 * m_sRect.h);
            m_vForces.y *= -1.0f;
            m_vVelocity.y *= -1.0f;
        }
        if (vStart.y < 0.0f)
        {
            m_sRect.y1 = -(vStart.y);
            m_vForces.y *= -1.0f;
            m_vVelocity.y *= -1.0f;
        } //��� �̰� �����غ����� �ߴµ�.. �ϴ� �־��

        SetPosition(m_sRect.x1, m_sRect.y1, m_sRect.w, m_sRect.h);
    }

    K_BaseObject::Frame();

}