#include "Sample.h"
#include "K_DxState.h"
#include "K_TextureManager.h" // GetSplitName 이것땜에 갖고옴 ㅋ;
#include <ShObjIdl.h> // 윈도우 파일탐색기
#include <comdef.h> // const wchar* -> const char*에 사용
//#include <fstream> // UI 오브젝트 헤더로 이동

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
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	I_Tex.Load(m_szImageFileName);

	return true; 
}

bool Sample::Frame()
{
	ImGuiFrame();

	if (IsClear) { Clear(); } // 새로 만들기 눌렀을때

	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH && IsSelect)
	{
		m_CursorPos.x = I_Input.m_ptPos.x;
		m_CursorPos.y = I_Input.m_ptPos.y;
		m_CursorPos.x = ((m_CursorPos.x / g_rtClient.right) * 2.0f) - 1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환
		m_CursorPos.y = (((m_CursorPos.y / g_rtClient.bottom) * 2.0f) - 1.0f) * -1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환
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

	//for (auto UIAlpha : m_pUIAlphaList){ UIAlpha->Render(); }
	//m_pUIAlphaList.clear();

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

void Sample::ImGuiFrame()
{
	//// 이무기 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//// 이무기 테스트 윈도우 생성
	ImGui::Begin(u8"UI Tool", &IsToolActive, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"파일"))
		{
			if (ImGui::MenuItem(u8"새로 만들기", "Ctrl+N")) { IsClear = true; }
			if (ImGui::MenuItem(u8"열기", "Ctrl+O")) { FileLoad(); }
			if (ImGui::MenuItem(u8"저장", "Ctrl+S")) { FileSave(m_szFileName); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader(u8"이미지", nullptr)) // 접을 수 있는 헤더
	{
		// 선택한 이미지와 이미지 이름
		ImVec2 imageSize(100.0f, 100.0f);
		ImGui::Image(I_Tex.Find(m_szImageFileName)->GetSRV(), imageSize, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
		ImGui::SameLine();
		_bstr_t fileName(m_szImageFileName.c_str());
		ImGui::Text(fileName);
		if (ImGui::Button(u8"이미지 가져오기")) 
		{
			std::wstring newFileName = FileOpen();
			if (newFileName != L"") // 취소 눌렀으면 거르려고
			{ 
				m_szImageFileName = newFileName;
				I_Tex.Load(m_szImageFileName); 
			}
		}

		ImGui::InputFloat2(u8"x,y (NDC) - image", ImageXY);
		ImGui::InputFloat2(u8"Width,Height (NDC) - image", ImageWH);
		ImGui::InputFloat2(u8"Depth,Alpha - image", ImageDA);

		ImGui::Checkbox(u8"랜덤 위치에 이미지 생성", &IsRandom);
		ImGui::Checkbox(u8"클릭한 위치에 이미지 생성", &IsSelect);

		if (ImGui::Button(u8"이미지 생성 버튼")) // 버튼이 눌렸다면
		{
			if (IsRandom) // -1 ~ 1 사이의 랜덤 xy좌표 생성
			{
				float randX = rand();
				float randY = rand();
				randX = ((randX / RAND_MAX) * 2) - 1.0f;
				randY = (((randY / RAND_MAX) * 2) - 1.0f) * -1.0f;
				CreateNewRect({ randX, randY }, { ImageWH[0], ImageWH[1] }, ImageDA[0], ImageDA[1]);
			}
			else CreateNewRect({ ImageXY[0],ImageXY[1] }, { ImageWH[0], ImageWH[1] }, ImageDA[0], ImageDA[1]);
		}
	}

	if (ImGui::CollapsingHeader(u8"버튼", nullptr))
	{
		ImGui::InputFloat2(u8"x,y (NDC) - button", ButtonXY);
		ImGui::InputFloat2(u8"Depth,Alpha - button", ButtonDA);

		if (ImGui::Button(u8"버튼 생성 버튼!"))
		{
			CreateNewButton({ ButtonXY[0], ButtonXY[1] }, { 0.5f, 0.5f }, ButtonDA[0], ButtonDA[1]);
		}
		ImGui::Checkbox(u8"버튼 비활성화", &IsDisable);
	}

	if (ImGui::CollapsingHeader(u8"스프라이트", nullptr))
	{
		ImGui::InputFloat2(u8"x,y (NDC) - sprite", SpriteXY);
		ImGui::InputFloat2(u8"Depth,Alpha - sprite", SpriteDA);

		ImGui::Text(u8"스프라이트 생성");
		if (ImGui::Button(u8"스프라이트 생성 버튼"))
		{
			CreateNewSprite({ SpriteXY[0], SpriteXY[1] }, { 0.5f, 0.5f }, SpriteDA[0], SpriteDA[1]);
		}
	}

	if (ImGui::CollapsingHeader(u8"상태 변환", nullptr))
	{
		ImGui::Checkbox(u8"와이어 프레임 사용", &IsWireFrame);
		ImGui::Checkbox(u8"깊이 버퍼 사용", &IsDepth);
		ImGui::Checkbox(u8"알파블렌딩 사용", &IsAlphaBlend);
	}

	ImGui::End();
}

bool Sample::CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha)
{
	bool success;

	K_UIObject* newRect = new K_UIObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, m_szImageFileName, L"../../data/shader/DefaultObject_Orgin.txt");
	
	if (success) 
	{
		m_pRectList.push_back(newRect);
		m_pUIList.push_back(newRect);

		newRect->SetPosition(orginPos, widthHeight, depth);
		newRect->SetAlpha(alpha);
		newRect->SetUV({ 0.0f, 0.0f }, { 1.0f, 1.0f });
		newRect->UpdateVertexBuffer();
		newRect->m_Type = IMAGE;
		newRect->m_ID = m_CurrentID;
		m_CurrentID++;

		newRect->AddCut(newRect->m_WidthHeight, { 0,0 }, { 1,1 }, newRect->m_szTextureName, newRect->m_szShaderName);
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
		newButton->m_Type = IMAGE;
		newButton->m_ID = m_CurrentID;
		m_CurrentID++;
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
		newSprite->m_Type = IMAGE;
		newSprite->m_ID = m_CurrentID;
		m_CurrentID++;
	}
	
	return success;
}

void Sample::Clear()
{
	for (auto UI : m_pUIList) { UI->Release(); }
	m_pUIList.clear();
	m_pRectList.clear();
	m_pButtonList.clear();
	m_pSpriteList.clear();

	m_CurrentID = 1;
	IsClear = false;
}

std::wstring Sample::FileOpen()
{
	IFileDialog* pFileDialog = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileDialog, (void**)&pFileDialog);
	std::wstring fileName = L"";

	if (SUCCEEDED(hr)) {
		// Set file type filter
		COMDLG_FILTERSPEC fileTypes[] = { { L"All Files", L"*.*" } };
		pFileDialog->SetFileTypes(1, fileTypes);

		// Show the dialog box
		if (SUCCEEDED(pFileDialog->Show(NULL))) {
			// Get the file name
			IShellItem* pShellItem;
			if (SUCCEEDED(pFileDialog->GetResult(&pShellItem))) {
				PWSTR filePath;
				if (SUCCEEDED(pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath))) {
					std::wstring filePathStr(filePath);
					CoTaskMemFree(filePath);
					fileName = filePathStr;
				}
				pShellItem->Release();
			}
		}
		pFileDialog->Release();
	}

	return fileName;
}


void Sample::FileSave(std::wstring saveFileName)
{
	HRESULT hr;
	IFileDialog* pfd;

	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	if (FAILED(hr))
	{
		// Handle the error
		return;
	}

	// Set the default file extension to .txt
	pfd->SetDefaultExtension(L"txt");

	// Set the file name
	pfd->SetFileName(saveFileName.c_str());

	// Show the save dialog
	hr = pfd->Show(NULL);
	if (FAILED(hr))
	{
		// Handle the error
		pfd->Release();
		return;
	}

	// Get the selected file name
	IShellItem* psiResult;
	hr = pfd->GetResult(&psiResult);
	if (FAILED(hr))
	{
		// Handle the error
		pfd->Release();
		return;
	}

	PWSTR pszFilePath;
	hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	if (FAILED(hr))
	{
		// Handle the error
		psiResult->Release();
		pfd->Release();
		return;
	}

	std::wstring szFilePath(pszFilePath);
	std::string sFilePath = to_wm(szFilePath);

	// Open the file and write some text
	std::ofstream outfile(sFilePath);
	if (outfile.is_open())
	{
		for (auto UI : m_pUIList) { UI->Save(outfile); }
		outfile.close();
	}

	// Clean up
	CoTaskMemFree(pszFilePath);
	psiResult->Release();
	pfd->Release();

	return;
}


void Sample::FileLoad()
{
	Clear();

	std::wstring newFileName = FileOpen();
	if (newFileName == L"") return;

	m_szFileName = I_Tex.GetSplitName(newFileName);
	m_szFileContent = FileReadToString(newFileName);
	m_splitContent = SplitString(m_szFileContent, L"="); // 일단 오브젝트 단위로 쪼갬
	
	UIType	Type;
	int		ID;
	ImVec2	OrginPos;
	float	Depth;
	float	Alpha;

	std::vector<CI> cutInfoList;
	int cutNum;
	ImVec2 WidthHeight;
	ImVec2 uvTL;
	ImVec2 uvBR;
	std::wstring TextureName;
	std::wstring ShaderName;

	for (auto obj : m_splitContent) 
	{
		std::vector<std::wstring> objContent = SplitString(obj, L"\n");
		if (objContent.size() == 0) { continue; }

		for (auto line : objContent) 
		{
			std::vector<std::wstring> LineContent = SplitString(line, L"\t");

			if (LineContent.size() == 0) { continue; }
			else if (LineContent[0] == L"Type") { Type = (UIType)std::stoi(LineContent[1]); }
			else if (LineContent[0] == L"ID") { ID = std::stoi(LineContent[1]); }
			else if (LineContent[0] == L"OrginPos") 
			{ 
				std::vector<std::wstring> OrginPosContent = SplitString(LineContent[1], L" ");
				OrginPos[0] = std::stof(OrginPosContent[0]);
				OrginPos[1] = std::stof(OrginPosContent[1]);
			}
			else if (LineContent[0] == L"Depth") { Depth = std::stof(LineContent[1]); }
			else if (LineContent[0] == L"Alpha") { Alpha = std::stof(LineContent[1]); }
			// else if (LineContent[0] == L"CutNum") { cutNum = std::stoi(LineContent[1]); } // 얘는 사실 필요없을지도?
			else if (LineContent[0] == L"CutInfo") 
			{
				std::vector<std::wstring> ciContent = SplitString(LineContent[1], L"|");

				CI newCutInfo;

				std::vector<std::wstring> cutNumContent = SplitString(ciContent[0], L" ");
				newCutInfo.cn = std::stoi(cutNumContent[0]);

				std::vector<std::wstring> cutWHContent = SplitString(ciContent[1], L" ");
				newCutInfo.pxWH.x = std::stof(cutWHContent[0]);
				newCutInfo.pxWH.y = std::stof(cutWHContent[1]);

				std::vector<std::wstring> cutUVContent = SplitString(ciContent[2], L" ");
				newCutInfo.uvTL.x = std::stof(cutUVContent[0]);
				newCutInfo.uvTL.y = std::stof(cutUVContent[1]);
				newCutInfo.uvBR.x = std::stof(cutUVContent[2]);
				newCutInfo.uvBR.y = std::stof(cutUVContent[3]);

				std::vector<std::wstring> cutTNContent = SplitString(ciContent[3], L" ");
				newCutInfo.tn = cutTNContent[0];

				std::vector<std::wstring> cutSNContent = SplitString(ciContent[4], L" ");
				newCutInfo.sn = cutSNContent[0];

				cutInfoList.push_back(newCutInfo);
			}
		}
		
		// 여기서 UI 오브젝트 생성
		if (Type == 0) 
		{
			K_UIObject* newRect = new K_UIObject;
			for (auto CI : cutInfoList) { newRect->AddCut(CI); }
			bool success = newRect->Create(m_pd3dDevice, m_pImmediateContext, cutInfoList[0].tn, cutInfoList[0].sn);

			if (success)
			{
				m_pRectList.push_back(newRect);
				m_pUIList.push_back(newRect);

				newRect->SetPosition(OrginPos, cutInfoList[0].pxWH, Depth);
				newRect->SetAlpha(Alpha);
				newRect->SetUV(cutInfoList[0].uvTL, cutInfoList[0].uvBR);
				newRect->UpdateVertexBuffer();
				newRect->m_Type = Type;
				newRect->m_ID = ID;
				m_CurrentID = ID;
			}
		}
		cutInfoList.clear();
	}
	m_CurrentID++;
}

std::wstring Sample::FileReadToString(std::wstring readFileName)
{
	std::ifstream file(readFileName);
	if (!file.is_open()) {
		// handle file open error
	}

	std::string fileContents;
	std::string line;
	while (std::getline(file, line)) {
		fileContents += line + "\n";
	}
	file.close();

	return to_mw(fileContents);
}

std::vector<std::wstring> Sample::SplitString(std::wstring inputStr, std::wstring delimiter)
{
	std::vector<std::wstring> substrings;

	size_t pos = 0;

	while (pos < inputStr.length()) {
		size_t next_delim = inputStr.find_first_of(delimiter, pos);
		if (next_delim == std::wstring::npos) {
			next_delim = inputStr.length();
		}
		if (inputStr.substr(pos, next_delim - pos) != L"") 
		{ substrings.push_back(inputStr.substr(pos, next_delim - pos)); }
		pos = next_delim + 1;
	}

	// Print the resulting substrings vector
	for (const auto& substring : substrings) {
		std::wcout << substring << std::endl;
	}

	return substrings;
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
