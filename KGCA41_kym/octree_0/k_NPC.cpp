#include "k_NPC.h"

void   k_NPC::Frame(float fDeltaTime, float fGameTime) 
{
    {
        k_Vector vStart = m_Box.vMin; // 시작벡터
        // 벡터의 직전의 방정식
        // 결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간)

        k_Vector vDirection = m_vDirection; // *1.0f; - 얘는 t인데 지금은 뭐 굳이같아서
        vStart = vStart + vDirection;
        m_Box.vMin = vStart;

        if (vStart.x + m_Box.vSize.x > 100.0f)
        {
            m_Box.vMin.x = 200.0f - (vStart.x + 2 * m_Box.vSize.x);
            m_vDirection.x *= -1.0f;
        }
        if (vStart.x < 0.0f)
        {
            m_Box.vMin.x = -(vStart.x);
            m_vDirection.x *= -1.0f;
        }
        if (vStart.y + m_Box.vSize.y > 100.0f)
        {
            m_Box.vMin.y = 200.0f - (vStart.y + 2 * m_Box.vSize.y);
            m_vDirection.y *= -1.0f;
        }
        if (vStart.y < 0.0f)
        {
            m_Box.vMin.y = -(vStart.y);
            m_vDirection.y *= -1.0f;
        }

        SetPosition(m_Box.vMin, m_Box.vSize);

    }
} 

// 귀찮아 머리아프니까 옥트리부터 만들고 할래
// ㅅㅂ 플레이어는 대체 언제하냐