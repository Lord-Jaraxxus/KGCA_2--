#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	// �̹��� ���� 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean()); // �ѱ� ��밡���ϰ�
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pd3dDevice, m_pImmediateContext);
	ImGui::StyleColorsDark();

	return true; 
}

bool Sample::Frame()
{
	// �̹��� ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �̹��� �׽�Ʈ ������ ����
	ImGui::Begin(u8"�׽�Ʈ");
	ImGui::Text(u8"�����ٶ󸶹ٻ� ����");
	if (ImGui::Button(u8"������ ���ÿ�")) 
	{ 
		CreateNewRect(); 
	}
	ImGui::End();
	
	return true;
}

bool Sample::Render()
{
	for (auto rect : m_pRectList) 
	{
		rect->Render();
	}

	// Assemble Together Draw Data��µ����Ҹ���
	ImGui::Render();
	// ������ �׸� ����Ÿ�� �����ϴ� �κ��� ����..�ΰ�
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool Sample::Release()
{
	for (auto rect : m_pRectList)
	{
		rect->Release();
	}

	return true; 
}

bool Sample::CreateNewRect()
{
	bool success;

	K_BaseObject* newRect = new K_BaseObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	m_pRectList.push_back(newRect);

	newRect->m_VertexList[0].p = { -0.5f, 0.5f, 0.0f };
	newRect->m_VertexList[1].p = { +0.5f, 0.5f,  0.0f };
	newRect->m_VertexList[2].p = { -0.5f, -0.5f, 0.0f };
	newRect->m_VertexList[3].p = { 0.5f, -0.5f, 0.0f };

	newRect->m_VertexList[0].t = { 0.0f, 0.0f };
	newRect->m_VertexList[1].t = { 1.0f, 0.0f };
	newRect->m_VertexList[2].t = { 0.0f, 1.0f };
	newRect->m_VertexList[3].t = { 1.0f, 1.0f };

	newRect->UpdateVertexBuffer();

	return success;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) 
{
    Sample demo;
    demo.SetWindow(hInstance, L"UI Tool", 800, 900);
    demo.Run();
    return 1;
}  

//GAME_RUN(CreateObject, 800, 900)
