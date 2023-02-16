#include "Sample.h"
#include "K_DxState.h"

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
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	return true; 
}

bool Sample::Frame()
{
	//// �̹��� ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	 
	//// �̹��� �׽�Ʈ ������ ����
	ImGui::Begin(u8"UI Tool",&IsToolActive, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"����"))
		{
			if (ImGui::MenuItem(u8"���� �����", "Ctrl+N")) { IsClear = true; }
			if (ImGui::MenuItem(u8"����", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem(u8"����", "Ctrl+S")) { /* Do stuff */ }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader(u8"�̹���", nullptr)) // ���� �� �ִ� ���
	{
		ImGui::InputFloat2(u8"x,y (NDC) - image", ImageXY);
		ImGui::InputFloat2(u8"Depth,Alpha - image", ImageDA);

		ImGui::Checkbox(u8"���� ��ġ�� �̹��� ����", &IsRandom);
		ImGui::Checkbox(u8"Ŭ���� ��ġ�� �̹��� ����", &IsSelect);

		if (ImGui::Button(u8"�̹��� ���� ��ư")) // ��ư�� ���ȴٸ�
		{
			if (IsRandom) // -1 ~ 1 ������ ���� xy��ǥ ����
			{
				float randX = rand();
				float randY = rand();
				randX = ((randX / RAND_MAX) * 2) - 1.0f;
				randY = (((randY / RAND_MAX) * 2) - 1.0f) * -1.0f;
				CreateNewRect({ randX, randY }, { 1.0f, 1.0f }, ImageDA[0], ImageDA[1]);
			}
			else CreateNewRect( {ImageXY[0],ImageXY[1]} , { 1.0f, 1.0f }, ImageDA[0], ImageDA[1]);
		}
	}

	if (ImGui::CollapsingHeader(u8"��ư", nullptr)) 
	{
		ImGui::InputFloat2(u8"x,y (NDC) - button", ButtonXY);
		ImGui::InputFloat2(u8"Depth,Alpha - button", ButtonDA);

		if (ImGui::Button(u8"��ư ���� ��ư!"))
		{
			CreateNewButton({ ButtonXY[0], ButtonXY[1] }, { 0.5f, 0.5f }, ButtonDA[0], ButtonDA[1]);
		}
		ImGui::Checkbox(u8"��ư ��Ȱ��ȭ", &IsDisable);
	}

	if (ImGui::CollapsingHeader(u8"��������Ʈ", nullptr)) 
	{
		ImGui::InputFloat2(u8"x,y (NDC) - sprite", SpriteXY);
		ImGui::InputFloat2(u8"Depth,Alpha - sprite", SpriteDA);

		ImGui::Text(u8"��������Ʈ ����");
		if (ImGui::Button(u8"��������Ʈ ���� ��ư"))
		{
			CreateNewSprite({ SpriteXY[0], SpriteXY[1] }, { 0.5f, 0.5f }, SpriteDA[0], SpriteDA[1]);
		}
	}

	if (ImGui::CollapsingHeader(u8"���� ��ȯ", nullptr)) 
	{
		ImGui::Checkbox(u8"���̾� ������ ���", &IsWireFrame);
		ImGui::Checkbox(u8"���� ���� ���", &IsDepth);
		ImGui::Checkbox(u8"���ĺ��� ���", &IsAlphaBlend);
	}

	ImGui::End();

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH && IsSelect)
	{
		m_CursorPos.x = I_Input.m_ptPos.x;
		m_CursorPos.y = I_Input.m_ptPos.y;
		m_CursorPos.x = ((m_CursorPos.x / g_rtClient.right) * 2.0f) - 1.0f; // ���콺 ��ǥ�� -1 ~ 1 ���̷� ��ȯ
		m_CursorPos.y = (((m_CursorPos.y / g_rtClient.bottom) * 2.0f) - 1.0f) * -1.0f; // ���콺 ��ǥ�� -1 ~ 1 ���̷� ��ȯ
		CreateNewRect({ (float)m_CursorPos.x, (float)m_CursorPos.y }, { 1.0f, 1.0f }, ImageDA[0], ImageDA[1]);
	}
	
	for (auto button : m_pButtonList)
	{
		if (IsDisable) { button->m_bIsDisable = true; }
		else { button->m_bIsDisable = false; }
	}

	if (IsWireFrame) { K_DxState::g_pCurrentRS = K_DxState::g_pDefaultRSWireFrame; }
	else { K_DxState::g_pCurrentRS = K_DxState::g_pDefaultRSSolid; }
	if (IsDepth) { K_DxState::g_pCurrentDSS = K_DxState::g_pDefaultDepthStencil; }
	else { K_DxState::g_pCurrentDSS = K_DxState::g_pDisableDepthStencil; }
	if (IsAlphaBlend) { K_DxState::g_pCurrentBS = K_DxState::g_pAlphaBlend; }
	else { K_DxState::g_pCurrentBS = K_DxState::g_pAlphaBlendDisable; }

	for (auto UI : m_pUIList)
	{
		UI->Frame();
	}

	return true;
}

bool Sample::Render()
{
	for (auto UI : m_pUIList) { UI->Render(); }

	//for (auto UI : m_pUIList)
	//{
	//	if (UI->m_fAlpha < 1.0f) { m_pUIAlphaList.push_back(UI); }
	//	else UI->Render(); 
	//}

	//for (auto UIAlpha : m_pUIAlphaList){ UIAlpha->Render(); }
	//m_pUIAlphaList.clear();

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

bool Sample::CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha)
{
	bool success;

	K_UIObject* newRect = new K_UIObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject_Orgin.txt");
	
	if (success) 
	{
		m_pRectList.push_back(newRect);
		m_pUIList.push_back(newRect);

		newRect->SetPosition(orginPos, widthHeight, depth);
		newRect->SetAlpha(alpha);
		newRect->SetUV({ 0.0f, 0.0f }, { 1.0f, 1.0f });
		newRect->UpdateVertexBuffer();
	} 

	return success;
}

bool Sample::CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha)
{
	bool success;

	K_Button* newButton = new K_Button;
	success = newButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/button/BatteryDead.png", L"../../data/shader/DefaultObject_Orgin.txt");
	if (success)
	{
		m_pButtonList.push_back(newButton);
		m_pUIList.push_back(newButton);
		newButton->AddTexture(L"../../data/img/button/BatteryDead.png");
		newButton->AddTexture(L"../../data/img/button/BatteryFull.png");
		newButton->AddTexture(L"../../data/img/button/BatteryClick.png");
		newButton->AddTexture(L"../../data/img/button/BatteryDisable.png");

		newButton->SetPosition(orginPos, widthHeight, depth);
		newButton->SetAlpha(alpha);
		newButton->SetUV({ 0.0f, 0.0f }, { 1.0f, 1.0f });
		newButton->UpdateVertexBuffer();
	}

	return success;
}

bool Sample::CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha)
{
	bool success;

	K_Sprite* newSprite = new K_Sprite;
	success = newSprite->Create(m_pd3dDevice, m_pImmediateContext, 
		L"../../data/img/sprite/Golden/golden knight animation sword right edit_00001.png", L"../../data/shader/DefaultObject_Orgin.txt");
	
	if (success)
	{
		m_pSpriteList.push_back(newSprite);
		m_pUIList.push_back(newSprite);

		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00001.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00002.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00003.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00004.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00005.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00006.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00007.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00008.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00009.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00010.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00011.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00012.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00013.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00014.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00015.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00016.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00017.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({700, 500}, {0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00018.png", L"../../data/shader/DefaultObject.txt");
		newSprite->m_iMaxFrame = 18;

		newSprite->m_OrginPos = orginPos;
		newSprite->SetPosition(orginPos, widthHeight, depth);
		newSprite->SetAlpha(alpha);
		newSprite->UpdateVertexBuffer();
	}
	
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
