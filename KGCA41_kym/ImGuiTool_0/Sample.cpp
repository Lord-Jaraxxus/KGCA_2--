#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	// 이무기 세팅 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_pd3dDevice, m_pImmediateContext);
	ImGui::StyleColorsDark();

	m_pBG = new K_BaseObject;
	m_pBG->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");

	return true; 
}

bool Sample::Frame()
{
	// 이무기 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 이무기 테스트 윈도우 생성
	ImGui::Begin("test");
	ImGui::End();
	
	return true;
}

bool Sample::Render()
{
	m_pBG->Render();

	// Assemble Together Draw Data라는데뭔소리야
	ImGui::Render();

	// 실제로 그릴 데이타를 렌더하는 부분은 여기..인가
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool Sample::Release()
{
	m_pBG->Release();

	return true; 
}


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow) 
//{
//    Sample demo;
//    demo.SetWindow(hInstance);
//    demo.Run();
//    return 1;
//}  

GAME_RUN(CreateObject, 800, 900)
