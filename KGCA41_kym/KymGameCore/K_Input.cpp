#include "K_Input.h"

bool K_Input::Init()
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD)*256);
	::GetCursorPos(&m_ptPos); // ȭ����ǥ
	::ScreenToClient(g_hWnd, &m_ptPos); // Ŭ���̾�Ʈ
	m_ptPrePos = m_ptPos;
	return true;
}

bool K_Input::Frame()
{
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = ::GetAsyncKeyState(iKey); // �񵿱� Ű ����
		if (sKey & 0x8000) // 1000 0000 0000 0000, sKey�� �ֻ��� ��Ʈ�� 1�̸� true�� �ȴ� 
		{
			if(m_dwKeyState[iKey] == KEY_FREE || m_dwKeyState[iKey] == KEY_UP) m_dwKeyState[iKey] = KEY_PUSH;
			else m_dwKeyState[iKey] = KEY_HOLD;
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_FREE || m_dwKeyState[iKey] == KEY_UP) m_dwKeyState[iKey] = KEY_FREE;
			else m_dwKeyState[iKey] = KEY_UP;
		}
	}


	::GetCursorPos(&m_ptPos); // ȭ����ǥ��� ���콺 ��ġ�� ����
	::ScreenToClient(g_hWnd, &m_ptPos); // ȭ����ǥ�踦 Ŭ���̾�Ʈ ��ǥ��� ����
	m_ptOffset.x = m_ptPos.x - m_ptPrePos.x;
	m_ptOffset.y = m_ptPos.y - m_ptPrePos.y;
	m_ptPrePos = m_ptPos;

	m_szCursorPos = std::to_wstring(m_ptPos.x);
	m_szCursorPos += L" , ";
	m_szCursorPos += std::to_wstring(m_ptPos.y);
	m_szCursorPos += L"\n";

	return true;
}

bool K_Input::Render()
{
	return true;
}

bool K_Input::Release()
{
	return true;
}

DWORD K_Input::GetKey(DWORD dwkey) 
{
	return m_dwKeyState[dwkey];
} 