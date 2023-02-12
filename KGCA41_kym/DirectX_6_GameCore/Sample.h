#pragma once
#include "K_GameCore.h"

// 얘들 왜 타이머 클래스 헤더에 넣으면 야랄남? 윈도우 헤더에 넣었을때도 잘 돌아가는데 
// 정답! 선생님은 타이머 클래스 헤더가 아니라 타이머 클래스 cpp에 넣으셨음! 난 빡대가리!
//float g_fGameTimer = 0.0f;
//float g_fSecondPerFrame = 0.0f;

class Sample : public K_GameCore
{
	int m_iTexIndex = 1;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
}; 

