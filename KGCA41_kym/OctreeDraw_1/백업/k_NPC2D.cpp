#include "k_NPC2D.h"

void   k_NPC2D::Frame(float fDeltaTime, float fGameTime) 
{
    {
        k_Vector2D vStart(m_sRect.x1, m_sRect.y1); // 시작벡터
        // 벡터의 직전의 방정식
        // 결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간)

        k_Vector2D vDirection = m_vDirection * fDeltaTime;
        vStart = vStart + vDirection;
        m_sRect.x1 = vStart.x;
        m_sRect.y1 = vStart.y;

        if (vStart.x + m_sRect.w > 100.0f)
        {
            m_sRect.x1 = 200.0f - (vStart.x + 2 * m_sRect.w);
            m_vDirection.x *= -1.0f;
        }
        if (vStart.x < 0.0f)
        {
            m_sRect.x1 = -(vStart.x);
            m_vDirection.x *= -1.0f;
        }
        if (vStart.y + m_sRect.h > 100.0f)
        {
            m_sRect.y1 = 200.0f - (vStart.y + 2 * m_sRect.h);
            m_vDirection.y *= -1.0f;
        }
        if (vStart.y < 0.0f)
        {
            m_sRect.y1 = -(vStart.y);
            m_vDirection.y *= -1.0f;
        }

        SetPosition(m_sRect.x1, m_sRect.y1, m_sRect.w, m_sRect.h);

    }
}