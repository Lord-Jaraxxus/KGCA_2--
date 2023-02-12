#include "k_NPC.h"

void   k_NPC::Frame(float fDeltaTime, float fGameTime) 
{
    {
        k_Vector vStart = m_Box.vMin; // ���ۺ���
        // ������ ������ ������
        // ������� = ���ۺ��� + ���⺤��* t(�Ÿ� ������ �ð�)

        k_Vector vDirection = m_vDirection; // *1.0f; - ��� t�ε� ������ �� ���̰��Ƽ�
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

// ������ �Ӹ������ϱ� ��Ʈ������ ����� �ҷ�
// ���� �÷��̾�� ��ü �����ϳ�