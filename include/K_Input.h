#pragma once
#include "K_Std.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};

class K_Input : public K_Singleton<K_Input>
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	POINT m_ptPos; // ���콺 ��ġ
	POINT m_ptPrePos;
	POINT m_ptOffset;

public:
	DWORD GetKey(DWORD dwkey);

private:
	DWORD m_dwKeyState[256]; // Ű ��ư + ���콺 ��ư
};

#define I_Input K_Input::GetInstance()