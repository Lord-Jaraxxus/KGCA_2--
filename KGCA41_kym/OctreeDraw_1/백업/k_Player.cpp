#include "k_Player.h"

void   k_Player::Frame(float fDeltaTime, float fGameTime)
{
    {
        k_Vector vStart = m_Box.vMin; // 시작벡터
        // 벡터의 직전의 방정식
        // 결과벡터 = 시작벡터 + 방향벡터* t(거리 내지는 시간)

        m_vAcceleration = m_vForces / m_fMass; // 1초 기준 가속도
        k_Vector vLinearAcc = m_vAcceleration * fDeltaTime;
        k_Vector vV0 = m_vVelocity;
        k_Vector vV1 = m_vVelocity + vLinearAcc;
        k_Vector vAverageVelocity = (vV0 + vV1) / 2; // fDeltaTime 동안의 평균 속도
        k_Vector vDisplacement = vAverageVelocity * fDeltaTime; // 변위

        m_vVelocity += vLinearAcc;
        vStart += vDisplacement;
        
        m_Box.Set(vStart, m_Box.vSize);

        // 플레이어가 벽에 부딪혔을 경우
        if (vStart.x + m_Box.vSize.x > 100.0f)
        {
            m_Box.vMin.x = 200.0f - (vStart.x + 2 * m_Box.vSize.x);
            m_vForces.x *= -1.0f;
            m_vVelocity.x *= -1.0f;
        }
        if (vStart.x < 0.0f)
        {
            m_Box.vMin.x = -(vStart.x);
            m_vForces.x *= -1.0f;
            m_vVelocity.x *= -1.0f;
        }
        if (vStart.y + m_Box.vSize.y > 100.0f)
        {
            m_Box.vMin.y = 200.0f - (vStart.y + 2 * m_Box.vSize.y);
            m_vForces.y *= -1.0f;
            m_vVelocity.y *= -1.0f;
        }
        if (vStart.y < 0.0f)
        {
            m_Box.vMin.y = -(vStart.y);
            m_vForces.y *= -1.0f;
            m_vVelocity.y *= -1.0f;
        } 
        if (vStart.z + m_Box.vSize.z > 100.0f)
        {
            m_Box.vMin.z = 200.0f - (vStart.z + 2 * m_Box.vSize.z);
            m_vForces.z *= -1.0f;
            m_vVelocity.z *= -1.0f;
        }
        if (vStart.z < 0.0f)
        {
            m_Box.vMin.z = -(vStart.z);
            m_vForces.z *= -1.0f;
            m_vVelocity.z *= -1.0f;
        }

        SetPosition(m_Box.vMin, m_Box.vSize);

    }
}