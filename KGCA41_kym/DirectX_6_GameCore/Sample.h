#pragma once
#include "K_GameCore.h"

// ��� �� Ÿ�̸� Ŭ���� ����� ������ �߶���? ������ ����� �־������� �� ���ư��µ� 
// ����! �������� Ÿ�̸� Ŭ���� ����� �ƴ϶� Ÿ�̸� Ŭ���� cpp�� ��������! �� ���밡��!
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

