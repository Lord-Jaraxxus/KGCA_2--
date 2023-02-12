#pragma once
#include <windows.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h> // D3DCompileFromFile()
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")

#include"k_Vector.h"
#include"k_Vector2D.h"


extern HWND g_hWnd;
extern RECT g_rtClient;

#define GAME_START	int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) { 
#define GAME_WINDOW(s,x,y)	Sample demo; demo.SetWindow(hInstance, L#s, x, y); demo.Run(); return 1;
#define GAME_END		 }
#define GAME_RUN(s,x,y)	GAME_START GAME_WINDOW(s,x,y) GAME_END