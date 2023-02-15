#pragma once
#include "K_Std.h"

class K_Timer : public K_Singleton<K_Timer>
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	float m_fGameTimer = 0.0f;
	float m_fElapseTimer = 0.0f;
	UINT m_iFPS = 0;
	std::wstring m_szTimer;

	float m_fFPSTimer = 0.0f;

//얘들은 temp라서 일단 외부접근 막아둠
private:
	DWORD dwBeforeTime = 0; 
	UINT m_iFPSCounter = 0;
	//float m_fFPSTimer = 0.0f;
};

#define I_Timer K_Timer::GetInstance()

