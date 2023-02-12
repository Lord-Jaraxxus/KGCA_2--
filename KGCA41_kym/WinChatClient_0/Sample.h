#pragma once
#pragma comment(lib, "KymGameCore_d.lib")

#include "K_GameCore.h"


class Sample : public K_GameCore
{
public:
	bool Init()override;
	bool Frame()override;
	bool Render()override;
	bool Release()override;
	bool Run() override;
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};