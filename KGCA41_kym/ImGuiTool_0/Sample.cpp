#include "Sample.h"

bool Sample::Init() 
{
	// �̹��� ���� 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean()); // �ѱ� ��밡���ϰ�
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pd3dDevice, m_pImmediateContext);
	ImGui::StyleColorsDark();

	return true; 
}

bool Sample::Frame()
{
	//// �̹��� ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//// �̹��� �׽�Ʈ ������ ����
	ImGui::Begin(u8"�׽�Ʈ");

	ImGui::Text(u8"�̹��� ����");
	if (ImGui::Button(u8"������ ���ÿ�")) // ��ư�� ���ȴٸ�
	{
		if (IsRandom) // -1 ~ 1 ������ ���� xy��ǥ ����
		{
			float randX = rand();
			float randY = rand();
			randX = ((randX / RAND_MAX) * 2) - 1.0f;
			randY = (((randY / RAND_MAX) * 2) - 1.0f) * -1.0f;
			CreateNewRect({ randX, randY }, { 1.0f, 1.0f });
		}
		else CreateNewRect({ 0,0 }, {1.0f, 1.0f});
	}
	ImGui::Checkbox(u8"����?", &IsRandom);
	ImGui::Checkbox(u8"Ŭ���� ��ġ�� �׸� ����", &IsSelect);

	ImGui::Text(u8"��ư");
	if (ImGui::Button(u8"��ư ����!")) 
	{
		CreateNewButton({ 0,0 }, { 0.5f, 0.5f });
	}
	ImGui::Checkbox(u8"��ư ��Ȱ��ȭ", &IsDisable);

	ImGui::End();

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH && IsSelect)
	{
		m_CursorPos.x = I_Input.m_ptPos.x;
		m_CursorPos.y = I_Input.m_ptPos.y;
		m_CursorPos.x = ((m_CursorPos.x / g_rtClient.right) * 2.0f) - 1.0f; // ���콺 ��ǥ�� -1 ~ 1 ���̷� ��ȯ
		m_CursorPos.y = (((m_CursorPos.y / g_rtClient.bottom) * 2.0f) - 1.0f) * -1.0f; // ���콺 ��ǥ�� -1 ~ 1 ���̷� ��ȯ
		CreateNewRect({ (float)m_CursorPos.x, (float)m_CursorPos.y }, { 1.0f, 1.0f });
	}
	
	for (auto button : m_pButtonList)
	{
		if (IsDisable) { button->IsDisable = true; }
		else { button->IsDisable = false; }
	}

	for (auto UI : m_pUIList)
	{
		UI->Frame();
	}

	return true;
}

bool Sample::Render()
{
	//for (auto UI : m_pUIList)
	//{
	//	UI->Render();
	//}	

	for (int i = m_pUIList.size(); i > 0; i--)
	{
		m_pUIList[i - 1]->Render();
	}

	// Assemble Together Draw Data��µ����Ҹ���
	ImGui::Render();
	// ������ �׸� ����Ÿ�� �����ϴ� �κ��� ����..�ΰ�
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();

	return true;
}

bool Sample::Release()
{
	for (auto UI : m_pUIList)
	{
		UI->Release();
	}

	return true; 
}

bool Sample::CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight)
{
	bool success;

	K_BaseObject* newRect = new K_BaseObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	if (success) 
	{
		m_pRectList.push_back(newRect);
		m_pUIList.push_back(newRect);
	} 

	float OrginPosX = orginPos.x;
	float OrginPosY = orginPos.y;

	float fWidth = widthHeight.x;
	float fHeight = widthHeight.y;

	newRect->m_VertexList[0].p = { OrginPosX - fWidth/2, OrginPosY + fHeight/2, 0.0f };
	newRect->m_VertexList[1].p = { OrginPosX + fWidth/2, OrginPosY + fHeight/2,  0.0f };
	newRect->m_VertexList[2].p = { OrginPosX - fWidth/2, OrginPosY - fHeight/2, 0.0f };
	newRect->m_VertexList[3].p = { OrginPosX + fWidth/2, OrginPosY - fHeight/2, 0.0f };

	newRect->m_VertexList[0].t = { 0.0f, 0.0f };
	newRect->m_VertexList[1].t = { 1.0f, 0.0f };
	newRect->m_VertexList[2].t = { 0.0f, 1.0f };
	newRect->m_VertexList[3].t = { 1.0f, 1.0f };

	newRect->UpdateVertexBuffer();

	return success;
}

bool Sample::CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight)
{
	bool success;

	K_Button* newButton = new K_Button;
	success = newButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/button/BatteryDead.png", L"../../data/shader/DefaultObject.txt");
	if (success)
	{
		m_pButtonList.push_back(newButton);
		m_pUIList.push_back(newButton);
		newButton->AddTexture(L"../../data/img/button/BatteryDead.png");
		newButton->AddTexture(L"../../data/img/button/BatteryFull.png");
		newButton->AddTexture(L"../../data/img/button/BatteryClick.png");
		newButton->AddTexture(L"../../data/img/button/BatteryDisable.png");
	}

	newButton->m_VertexList[0].p = { orginPos.x - widthHeight.x / 2, orginPos.y + widthHeight.y / 2, 0.0f };
	newButton->m_VertexList[1].p = { orginPos.x + widthHeight.x / 2, orginPos.y + widthHeight.y / 2,  0.0f };
	newButton->m_VertexList[2].p = { orginPos.x - widthHeight.x / 2, orginPos.y - widthHeight.y / 2, 0.0f };
	newButton->m_VertexList[3].p = { orginPos.x + widthHeight.x / 2, orginPos.y - widthHeight.y / 2, 0.0f };
	
	newButton->m_VertexList[0].t = { 0.0f, 0.0f };
	newButton->m_VertexList[1].t = { 1.0f, 0.0f };
	newButton->m_VertexList[2].t = { 0.0f, 1.0f };
	newButton->m_VertexList[3].t = { 1.0f, 1.0f };

	newButton->UpdateVertexBuffer();

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
