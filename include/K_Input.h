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
	POINT m_ptPos; // 마우스 위치
	POINT m_ptPrePos;
	POINT m_ptOffset;

public:
	DWORD GetKey(DWORD dwkey);

private:
	DWORD m_dwKeyState[256]; // 키 버튼 + 마우스 버튼
};

#define I_Input K_Input::GetInstance()