#include "Sample.h"

bool Sample::Init() 
{
	// 이무기 세팅 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean()); // 한글 사용가능하게
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pd3dDevice, m_pImmediateContext);
	ImGui::StyleColorsDark();

	return true; 
}

bool Sample::Frame()
{
	//// 이무기 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//// 이무기 테스트 윈도우 생성
	ImGui::Begin(u8"테스트");

	ImGui::Text(u8"이미지 생성");
	if (ImGui::Button(u8"누르지 마시오")) // 버튼이 눌렸다면
	{
		if (IsRandom) // -1 ~ 1 사이의 랜덤 xy좌표 생성
		{
			float randX = rand();
			float randY = rand();
			randX = ((randX / RAND_MAX) * 2) - 1.0f;
			randY = (((randY / RAND_MAX) * 2) - 1.0f) * -1.0f;
			CreateNewRect({ randX, randY }, { 1.0f, 1.0f });
		}
		else CreateNewRect({ 0,0 }, {1.0f, 1.0f});
	}
	ImGui::Checkbox(u8"란듐?", &IsRandom);
	ImGui::Checkbox(u8"클릭한 위치에 네모 생성", &IsSelect);

	ImGui::Text(u8"버튼 생성");
	if (ImGui::Button(u8"버튼 생성 버튼!")) 
	{
		CreateNewButton({ 0,0 }, { 0.5f, 0.5f });
	}
	ImGui::Checkbox(u8"버튼 비활성화", &IsDisable);

	ImGui::Text(u8"스프라이트 생성");
	if (ImGui::Button(u8"스프라이트 생성 버튼"))
	{
		CreateNewSprite({ -0.5f, -0.5f }, { 0.5f, 0.5f });
	}

	ImGui::End();

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH && IsSelect)
	{
		m_CursorPos.x = I_Input.m_ptPos.x;
		m_CursorPos.y = I_Input.m_ptPos.y;
		m_CursorPos.x = ((m_CursorPos.x / g_rtClient.right) * 2.0f) - 1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환
		m_CursorPos.y = (((m_CursorPos.y / g_rtClient.bottom) * 2.0f) - 1.0f) * -1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환
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
	//for (auto UI : m_pUIList){ UI->Render(); }

	for (int i = m_pUIList.size(); i > 0; i--)
	{
		m_pUIList[i - 1]->Render();
	}

	// Assemble Together Draw Data라는데뭔소리야
	ImGui::Render();
	// 실제로 그릴 데이타를 렌더하는 부분은 여기..인가
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

	K_UIObject* newRect = new K_UIObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	if (success) 
	{
		m_pRectList.push_back(newRect);
		m_pUIList.push_back(newRect);

		newRect->SetPosition(orginPos, widthHeight);
		newRect->SetUV({ 0.0f, 0.0f }, { 1.0f, 1.0f });
		newRect->UpdateVertexBuffer();
	} 

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

		newButton->SetPosition(orginPos, widthHeight);
		newButton->SetUV({ 0.0f, 0.0f }, { 1.0f, 1.0f });
		newButton->UpdateVertexBuffer();
	}

	return success;
}

bool Sample::CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight)
{
	bool success;

	K_Sprite* newSprite = new K_Sprite;
	success = newSprite->Create(m_pd3dDevice, m_pImmediateContext, 
		L"../../data/img/sprite/Golden/golden knight animation sword right edit_00001.png", L"../../data/shader/DefaultObject.txt");
	
	if (success)
	{
		m_pSpriteList.push_back(newSprite);
		m_pUIList.push_back(newSprite);

		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00001.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00002.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00003.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00004.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00005.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00006.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00007.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00008.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00009.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00010.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00011.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00012.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00013.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00014.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00015.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00016.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00017.png", L"../../data/shader/DefaultObject.txt");
		newSprite->AddCut({0.0f, 0.0f}, {1.0f, 1.0f}, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00018.png", L"../../data/shader/DefaultObject.txt");
		newSprite->m_iMaxFrame = 18;


		newSprite->SetPosition(orginPos, widthHeight);
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
