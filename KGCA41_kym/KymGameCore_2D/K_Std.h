//#pragma once
//#include <windows.h>
//#include <map>
#pragma once

#include <WinSock2.h>
#include <windows.h>

#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <tchar.h>
#include <codecvt>
#include <atlconv.h>

#include "TMath.h"
#include "K_Matrix.h"

#pragma comment(lib, "winmm.lib")	// timeGetTime
#pragma comment(lib, "ws2_32.lib")

// 윈도우
//#include <windows.h>

// 디바이스
#include <d3d11.h>
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")

// 쉐이더
//#pragma comment(lib, "d3dcompiler.lib")
//#include <d3dcompiler.h> // D3DCompileFromFile()

// 텍스처
//#include "WICTextureLoader.h"
//#include "DDSTextureLoader.h"
//#pragma comment(lib, "DirectXTK.lib")

// 베이스 오브젝트
//#include"k_Vector.h"
//#include"k_Vector2D.h"


extern HWND g_hWnd;
extern RECT g_rtClient;

extern float g_fGameTimer;
extern float g_fSecondPerFrame;

#define GAME_START	int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) { 
#define GAME_WINDOW(s,x,y)	Sample demo; demo.SetWindow(hInstance, L#s, x, y); demo.Run(); return 1;
#define GAME_END		 }
#define GAME_RUN(s,x,y)	GAME_START GAME_WINDOW(s,x,y) GAME_END


// 싱글톤 패턴
template<class T> class K_Singleton
{
public:
	static T& GetInstance() 
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};


typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<wchar_t> W_STR;
typedef std::basic_string<char>  C_STR;
typedef std::vector<std::basic_string<TCHAR>>		TCHAR_STRING_VECTOR;
typedef std::vector<DWORD>							DWORD_VECTOR;

//#include <atlconv.h> // A2W
static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};
//static std::wstring mtw(std::string str)
//{
//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
//	return conv.from_bytes(str);
//}
//#include <codecvt>
//static std::string wtm(std::wstring str)
//{
//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
//	return conv.to_bytes(str);
//}
static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength, // 대상
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  소스
		pDest, iLength, // 대상
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength); // 대상
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  소스
		pDest, iLength); // 대상		
	if (iRet == 0) return false;
	return true;
}
static void PRINT(char* fmt, ...) // 나열연산자
{
	va_list arg;
	va_start(arg, fmt);
	char buf[256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	printf("\n=====> %s", buf);
	va_end(arg);
}

static TBASIS_EX::TMatrix ConvertMatrixKtoT(K_Matrix KM)
{
	TBASIS_EX::TMatrix TM;

	TM._11 = KM._11; TM._12 = KM._12; TM._13 = KM._13; TM._14 = KM._14;
	TM._21 = KM._21; TM._22 = KM._22; TM._23 = KM._23; TM._24 = KM._24;
	TM._31 = KM._31; TM._32 = KM._32; TM._33 = KM._33; TM._34 = KM._34;
	TM._41 = KM._41; TM._42 = KM._42; TM._43 = KM._43; TM._44 = KM._44;

	return TM;
}

static K_Matrix ConvertMatrixTtoK(TBASIS_EX::TMatrix TM)
{
	K_Matrix KM;

	KM._11 = TM._11; KM._12 = TM._12; KM._13 = TM._13; KM._14 = TM._14;
	KM._21 = TM._21; KM._22 = TM._22; KM._23 = TM._23; KM._24 = TM._24;
	KM._31 = TM._31; KM._32 = TM._32; KM._33 = TM._33; KM._34 = TM._34;
	KM._41 = TM._41; KM._42 = TM._42; KM._43 = TM._43; KM._44 = TM._44;

	return KM;
}

static TBASIS_EX::TVector3 ConvertVector3KtoT(k_Vector KV) 
{
	TBASIS_EX::TVector3 TV;

	TV.x = KV.x;
	TV.y = KV.y;
	TV.z = KV.z;

	return TV;
}

static k_Vector ConvertVector3TtoK(TBASIS_EX::TVector3 TV)
{
	k_Vector KV;

	KV.x = TV.x;
	KV.y = TV.y;
	KV.z = TV.z;

	return KV;
}