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
	I_Tex.Load(m_szButtonFileName_N);
	I_Tex.Load(m_szButtonFileName_H);
	I_Tex.Load(m_szButtonFileName_C);
	I_Tex.Load(m_szButtonFileName_D);
	ImageWH[0] = I_Tex.Find(m_szImageFileName)->m_Desc.Width;
	ImageWH[1] = I_Tex.Find(m_szImageFileName)->m_Desc.Height;
	ButtonWH[0][0] = I_Tex.Find(m_szButtonFileName_N)->m_Desc.Width;
	ButtonWH[0][1] = I_Tex.Find(m_szButtonFileName_N)->m_Desc.Height;
	ButtonWH[1][0] = I_Tex.Find(m_szButtonFileName_H)->m_Desc.Width;
	ButtonWH[1][1] = I_Tex.Find(m_szButtonFileName_H)->m_Desc.Height;
	ButtonWH[2][0] = I_Tex.Find(m_szButtonFileName_C)->m_Desc.Width;
	ButtonWH[2][1] = I_Tex.Find(m_szButtonFileName_C)->m_Desc.Height;
	ButtonWH[3][0] = I_Tex.Find(m_szButtonFileName_D)->m_Desc.Width;
	ButtonWH[3][1] = I_Tex.Find(m_szButtonFileName_D)->m_Desc.Height;

	return true; 
}

bool Sample::Frame()
{
	ImGuiFrame();

	if (IsClear) { Clear(); } // 새로 만들기 눌렀을때

	HierarchyFrame();


	if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH && IsSelect)
	{
		m_CursorPos.x = I_Input.m_ptPos.x;
		m_CursorPos.y = I_Input.m_ptPos.y;
		m_CursorPos.x = ((m_CursorPos.x / g_rtClient.right) * 2.0f) - 1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환
		m_CursorPos.y = (((m_CursorPos.y / g_rtClient.bottom) * 2.0f) - 1.0f) * -1.0f; // 마우스 좌표를 -1 ~ 1 사이로 변환

		K_UIObject* newRect;
		newRect = CreateNewRect({ (float)m_CursorPos.x, (float)m_CursorPos.y }, AtoV(ImageWH), ImageDA[0], ImageDA[1]);
		newRect->AddCut(AtoV(ImageWH), AtoV(ImageUV[0]), AtoV(ImageUV[1]), m_szImageFileName, m_szDefaultShaderName);
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

	for (auto Iter : m_pUIList)
	{
		Iter->Frame();
	}

	return true;
}

bool Sample::Render()
{
	for (auto Iter : m_pUIList)
	{
		Iter->Render();
	}

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
	for (auto Iter : m_pUIList)
	{
		Iter->Release();
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
	ImGui::PushItemWidth(150); // 입력창 너비 설정


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
		K_Texture* texture = I_Tex.Find(m_szImageFileName);
		ImGui::Image(texture->GetSRV(), { 100.0f, 100.0f }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
		ImGui::SameLine();
		_bstr_t fileName(m_szImageFileName.c_str());
		if (ImGui::Button(fileName))
		{
			std::wstring newFileName = FileOpen();
			if (newFileName != L"") // 취소 눌렀으면 거르려고
			{
				m_szImageFileName = newFileName;
				texture = I_Tex.Load(m_szImageFileName);
				ImageWH[0] = texture->m_Desc.Width;
				ImageWH[1] = texture->m_Desc.Height;
			}
		}
		ImGui::InputTextWithHint(u8"이미지 이름", "NewImage", ImageName, sizeof(ImageName));
		ImGui::Dummy(ImVec2(0, 5));
		ImGui::InputFloat2(u8"원점 x,y 좌표 (NDC) - I", ImageXY);
		ImGui::InputFloat2(u8"이미지 크기 (픽셀) - I", ImageWH);
		ImGui::InputFloat2("", ImageUV[0]);
		ImGui::SameLine();
		ImGui::InputFloat2(u8"UV 좌표 (좌상단, 우하단) - I", ImageUV[1]);
		ImGui::InputFloat2(u8"깊이, 알파 - I", ImageDA);

		ImGui::Checkbox(u8"랜덤 위치에 이미지 생성", &IsRandom);
		ImGui::Checkbox(u8"클릭한 위치에 이미지 생성", &IsSelect);

		if (ImGui::Button(u8"이미지 생성 버튼")) // 버튼이 눌렸다면
		{
			std::wstring name(ImageName, ImageName + std::strlen(ImageName));
			if (m_pUIMap.find(name) == m_pUIMap.end()) // 이름이 중복이 아닐때만 만들도록
			{
				K_UIObject* newRect;
				if (IsRandom) // -1 ~ 1 사이의 랜덤 xy좌표 생성
				{
					float randX = rand();
					float randY = rand();
					randX = ((randX / RAND_MAX) * 2) - 1.0f;
					randY = (((randY / RAND_MAX) * 2) - 1.0f) * -1.0f;
					newRect = CreateNewRect({ randX, randY }, AtoV(ImageWH), ImageDA[0], ImageDA[1], name);
				}
				else newRect = CreateNewRect(AtoV(ImageXY), AtoV(ImageWH), ImageDA[0], ImageDA[1], name);
				newRect->AddCut(AtoV(ImageWH), AtoV(ImageUV[0]), AtoV(ImageUV[1]), m_szImageFileName, m_szDefaultShaderName);
			}
			else // 이름이 중복이면 경고 팝업창뜨도록
			{
				ImGui::OpenPopup(u8"이름 중복 경고");
			}
		}
	}


	if (ImGui::CollapsingHeader(u8"버튼", nullptr))
	{
		// 노멀
		if (ImGui::TreeNodeEx(u8"노말", ImGuiTreeNodeFlags_DefaultOpen))
		{
			K_Texture* texture = I_Tex.Find(m_szButtonFileName_N);
			ImGui::Image(texture->GetSRV(), { 100, 100 }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
			ImGui::SameLine();
			_bstr_t fileName(m_szButtonFileName_N.c_str());
			if (ImGui::Button(fileName))
			{
				std::wstring newFileName = FileOpen();
				if (newFileName != L"") // 취소 눌렀으면 거르려고
				{
					m_szButtonFileName_N = newFileName;
					texture = I_Tex.Load(m_szButtonFileName_N);
					ButtonWH[0][0] = texture->m_Desc.Width;
					ButtonWH[0][1] = texture->m_Desc.Height;
				}
			}
			ImGui::InputFloat2(u8"이미지 크기 (픽셀) - N", ButtonWH[0]);
			ImGui::InputFloat2("", ButtonUV[0][0]);
			ImGui::SameLine();
			ImGui::InputFloat2(u8"UV좌표 (좌상,우하) - N", ButtonUV[0][1]);
			ImGui::TreePop();
		}
		// 호버
		if (ImGui::TreeNodeEx(u8"호버", ImGuiTreeNodeFlags_DefaultOpen))
		{
			K_Texture* texture = I_Tex.Find(m_szButtonFileName_H);
			ImGui::Image(texture->GetSRV(), { 100, 100 }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
			ImGui::SameLine();
			_bstr_t fileName(m_szButtonFileName_H.c_str());
			if (ImGui::Button(fileName))
			{
				std::wstring newFileName = FileOpen();
				if (newFileName != L"") // 취소 눌렀으면 거르려고
				{
					m_szButtonFileName_H = newFileName;
					texture = I_Tex.Load(m_szButtonFileName_H);
					ButtonWH[1][0] = texture->m_Desc.Width;
					ButtonWH[1][1] = texture->m_Desc.Height;
				}
			}
			ImGui::InputFloat2(u8"이미지 크기 (픽셀) - H", ButtonWH[1]);
			ImGui::InputFloat2("", ButtonUV[1][0]);
			ImGui::SameLine();
			ImGui::InputFloat2(u8"UV좌표 (좌상,우하) - H", ButtonUV[1][1]);
			ImGui::TreePop();
		}
		// 클릭
		if (ImGui::TreeNodeEx(u8"클릭", ImGuiTreeNodeFlags_DefaultOpen))
		{
			K_Texture* texture = I_Tex.Find(m_szButtonFileName_C);
			ImGui::Image(texture->GetSRV(), { 100, 100 }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
			ImGui::SameLine();
			_bstr_t fileName(m_szButtonFileName_C.c_str());
			if (ImGui::Button(fileName))
			{
				std::wstring newFileName = FileOpen();
				if (newFileName != L"") // 취소 눌렀으면 거르려고
				{
					m_szButtonFileName_C = newFileName;
					texture = I_Tex.Load(m_szButtonFileName_C);
					ButtonWH[2][0] = texture->m_Desc.Width;
					ButtonWH[2][1] = texture->m_Desc.Height;
				}
			}
			ImGui::InputFloat2(u8"이미지 크기 (픽셀) - C", ButtonWH[2]);
			ImGui::InputFloat2("", ButtonUV[2][0]);
			ImGui::SameLine();
			ImGui::InputFloat2(u8"UV좌표 (좌상,우하) - C", ButtonUV[2][1]);
			ImGui::TreePop();
		}
		// 비활성화
		if (ImGui::TreeNodeEx(u8"비활성화", ImGuiTreeNodeFlags_DefaultOpen))
		{
			K_Texture* texture = I_Tex.Find(m_szButtonFileName_D);
			ImGui::Image(texture->GetSRV(), { 100, 100 }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
			ImGui::SameLine();
			_bstr_t fileName(m_szButtonFileName_D.c_str());
			if (ImGui::Button(fileName))
			{
				std::wstring newFileName = FileOpen();
				if (newFileName != L"") // 취소 눌렀으면 거르려고
				{
					m_szButtonFileName_D = newFileName;
					texture = I_Tex.Load(m_szButtonFileName_D);
					ButtonWH[3][0] = texture->m_Desc.Width;
					ButtonWH[3][1] = texture->m_Desc.Height;
				}
			}
			ImGui::InputFloat2(u8"이미지 크기 (픽셀) - D", ButtonWH[3]);
			ImGui::InputFloat2("", ButtonUV[3][0]);
			ImGui::SameLine();
			ImGui::InputFloat2(u8"UV좌표 (좌상,우하) - D", ButtonUV[3][1]);
			ImGui::TreePop();
		}

		ImGui::Dummy(ImVec2(0, 30));
		ImGui::InputTextWithHint(u8"버튼 이름", "Tim Burton", ButtonName, sizeof(ButtonName));
		ImGui::Dummy(ImVec2(0, 5));
		ImGui::InputFloat2(u8"원점 x,y좌표 (NDC) - B", ButtonXY);
		ImGui::InputFloat2(u8"깊이, 알파 - B", ButtonDA);

		if (ImGui::Button(u8"버튼 생성 버튼!"))
		{
			std::wstring name(ButtonName, ButtonName + strlen(ButtonName));
			if (m_pUIMap.find(name) == m_pUIMap.end()) // 중복검사
			{
				K_Button* newButton = CreateNewButton({ ButtonXY[0], ButtonXY[1] }, { ButtonWH[0][0], ButtonWH[0][1] }, ButtonDA[0], ButtonDA[1], name);
				newButton->AddCut(AtoV(ButtonWH[0]), AtoV(ButtonUV[0][0]), AtoV(ButtonUV[0][1]), m_szButtonFileName_N, L"../../data/shader/DefaultObject_Orgin.txt");
				newButton->AddCut(AtoV(ButtonWH[1]), AtoV(ButtonUV[1][0]), AtoV(ButtonUV[1][1]), m_szButtonFileName_H, L"../../data/shader/DefaultObject_Orgin.txt");
				newButton->AddCut(AtoV(ButtonWH[2]), AtoV(ButtonUV[2][0]), AtoV(ButtonUV[2][1]), m_szButtonFileName_C, L"../../data/shader/DefaultObject_Orgin.txt");
				newButton->AddCut(AtoV(ButtonWH[3]), AtoV(ButtonUV[3][0]), AtoV(ButtonUV[3][1]), m_szButtonFileName_D, L"../../data/shader/DefaultObject_Orgin.txt");
			}
			else { ImGui::OpenPopup(u8"이름 중복 경고"); }
		}
		ImGui::Checkbox(u8"버튼 비활성화", &IsDisable);
	}

	if (ImGui::CollapsingHeader(u8"스프라이트", nullptr))
	{
		ImGui::InputFloat2(u8"x,y (NDC) - S", SpriteXY);
		ImGui::InputFloat2(u8"Depth,Alpha - S", SpriteDA);

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

	// 경고 팝업창
	if (ImGui::BeginPopupModal(u8"이름 중복 경고", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(u8"중복ㄴㄴ해");
		if (ImGui::Button(u8"힝..", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

void Sample::HierarchyFrame()
{
	bool PopupOpen = false;

	ImGui::Begin(u8"아웃라이너");
	ImGui::PushItemWidth(150); // 입력창 너비 설정

	_bstr_t fileName(m_szFileName.c_str());
	if (ImGui::CollapsingHeader(fileName, ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		for (auto Iter : m_pUIList)
		{
			K_UIObject* UI = Iter;
			_bstr_t UI_Name(UI->m_szObjName.c_str());
			if (ImGui::TreeNodeEx(UI_Name, ImGuiTreeNodeFlags_None))
			{
				if (ImGui::IsItemClicked(1)) { PopupOpen = true; m_SelectedUI = UI; } // 우클릭 검사

				switch (UI->m_Type) 
				{
				case IMAGE:
				{
					ImGui::Image(UI->m_pCutInfoList[0]->tc->GetSRV(), { 100.0f, 100.0f }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
					ImGui::SameLine();
					_bstr_t fileName(UI->m_pCutInfoList[0]->tn.c_str());
					if (ImGui::Button(fileName))
					{
						std::wstring newFileName = FileOpen();
						if (newFileName != L"") // 취소 눌렀으면 거르려고
						{
							UI->m_pCutInfoList[0]->tn = newFileName;
							UI->m_pCutInfoList[0]->tc = I_Tex.Load(newFileName);
						}
					}
					ImGui::InputFloat2("##" + UI_Name + "op", &UI->m_OrginPos.x);
					ImGui::SameLine();
					ImGui::Text(u8"원점 x,y 좌표 (NDC)");
					ImGui::InputFloat2("##" + UI_Name + "wh", &UI->m_pCutInfoList[0]->pxWH.x);
					ImGui::SameLine();
					ImGui::Text(u8"이미지 크기 (픽셀)");
					ImGui::InputFloat2("##" + UI_Name + "uvTL", &UI->m_pCutInfoList[0]->uvTL.x);
					ImGui::SameLine();
					ImGui::InputFloat2("##" + UI_Name + "uvBR", &UI->m_pCutInfoList[0]->uvBR.x);
					ImGui::SameLine();
					ImGui::Text(u8"UV 좌표 (좌상단, 우하단)");
					ImGui::InputFloat2("##" + UI_Name + "d", &UI->m_fDepth);
					ImGui::SameLine();
					ImGui::Text(u8"깊이, 알파");
				} break;
				
				case BUTTON: 
				{
					char state[4][64] = {u8"노말", u8"호버", u8"클릭", u8"비활성화"};
					for (int i = 0; i < 4; i++) 
					{
						if (ImGui::TreeNodeEx(state[i], ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Image(UI->m_pCutInfoList[i]->tc->GetSRV(), { 100.0f, 100.0f }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, { 0,0,0,1 });
							ImGui::SameLine();
							_bstr_t fileName(UI->m_pCutInfoList[i]->tn.c_str());
							if (ImGui::Button(fileName))
							{
								std::wstring newFileName = FileOpen();
								if (newFileName != L"") // 취소 눌렀으면 거르려고
								{
									UI->m_pCutInfoList[i]->tn = newFileName;
									UI->m_pCutInfoList[i]->tc = I_Tex.Load(newFileName);
								}
								ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
							}
							std::string lavel = "##" + std::to_string(i) + to_wm(UI->m_szObjName); // 라벨 이름이 다 달라야해서 설정, 쓸데없이 길어졌네 (앞에 ##달면 안보임)
							ImGui::InputFloat2((lavel + "wh").c_str(), &UI->m_pCutInfoList[i]->pxWH.x);
							ImGui::SameLine();
							ImGui::Text(u8"이미지 크기 (픽셀)");
							ImGui::NextColumn();
							ImGui::InputFloat2((lavel + "uvTL").c_str(), &UI->m_pCutInfoList[i]->uvTL.x);
							ImGui::SameLine();
							ImGui::InputFloat2((lavel + "uvBR").c_str(), &UI->m_pCutInfoList[i]->uvBR.x);
							ImGui::SameLine();
							ImGui::Text(u8"UV 좌표 (좌상단, 우하단)");

							ImGui::TreePop();
						}
					}
					ImGui::InputFloat2("##" + UI_Name + "op", &UI->m_OrginPos.x);
					ImGui::SameLine();
					ImGui::Text(u8"원점 x,y 좌표 (NDC)");
					ImGui::InputFloat2("##" + UI_Name + "d", &UI->m_fDepth);
					ImGui::SameLine();
					ImGui::Text(u8"깊이, 알파");
				}break;

				}		
				ImGui::TreePop();
			}
			else if (ImGui::IsItemClicked(1)) { PopupOpen = true; m_SelectedUI = UI; }
		}
	}
	
	bool rename = false;
	if (PopupOpen) { ImGui::OpenPopup("삭제/이름바꾸기"); }
	if (ImGui::BeginPopup("삭제/이름바꾸기"))
	{
		if (ImGui::MenuItem(u8"삭제"))
		{
			//m_pUIMap.erase(m_pUIMap.find(m_SelectedUI->m_szObjName));
			m_pUIMap.erase(m_SelectedUI->m_szObjName);
			for (auto UI = m_pUIList.begin(); UI != m_pUIList.end(); ++UI)
			{
				if (m_SelectedUI->m_ID == (*UI)->m_ID) 
				{ m_pUIList.erase(UI); break; }
			}
		}
		if (ImGui::MenuItem(u8"이름 바꾸기"))
		{
			WideCharToMultiByte(CP_UTF8, 0, m_SelectedUI->m_szObjName.c_str(), -1, NewName, sizeof(NewName), NULL, NULL);
			rename = true;
		}
		ImGui::EndPopup();
	}

	if (rename) 
	{ 
		ImGui::OpenPopup(u8"이름 바꾸기");
	}
	if (ImGui::BeginPopupModal(u8"이름 바꾸기", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputTextWithHint(u8"새 이름", u8"이름없는 자", NewName, sizeof(ButtonName));
		if (ImGui::Button(u8"완료!", ImVec2(120, 0)))
		{
			m_SelectedUI->m_szObjName = std::wstring(NewName, NewName + strlen(NewName));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

K_UIObject* Sample::CreateNewRect(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha, std::wstring name)
{
	bool success;

	K_UIObject* newRect = new K_UIObject;
	success = newRect->Create(m_pd3dDevice, m_pImmediateContext, m_szImageFileName, L"../../data/shader/DefaultObject_Orgin.txt");

	if (success) 
	{
		if (name == L"") newRect->m_szObjName = std::to_wstring(m_CurrentID);
		else newRect->m_szObjName = name;
		m_pUIMap.insert(std::make_pair(newRect->m_szObjName, newRect));
		m_pUIList.push_back(newRect);
		m_pRectList.push_back(newRect);

		newRect->SetPosition(orginPos, K_UIObject::PtoN(widthHeight), depth);
		newRect->SetAlpha(alpha);
		newRect->SetUV(AtoV(ImageUV[0]), AtoV(ImageUV[1]));
		newRect->m_Type = IMAGE;
		newRect->m_ID = m_CurrentID;
		m_CurrentID++;

		return newRect;
	} 

	delete newRect;
	return nullptr;
}

K_Button* Sample::CreateNewButton(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha, std::wstring name)
{
	bool success;

	K_Button* newButton = new K_Button;
	success = newButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/button/Button_N.png", L"../../data/shader/DefaultObject_Orgin.txt");
	
	if (success)
	{
		if (name == L"") newButton->m_szObjName = std::to_wstring(m_CurrentID);
		else newButton->m_szObjName = name;
		m_pUIMap.insert(std::make_pair(newButton->m_szObjName, newButton));
		m_pUIList.push_back(newButton);
		m_pButtonList.push_back(newButton);

		newButton->SetPosition(orginPos, K_UIObject::PtoN(widthHeight), depth);
		newButton->SetAlpha(alpha);
		newButton->m_Type = BUTTON;
		newButton->m_ID = m_CurrentID;
		m_CurrentID++;

		// 노말상태 이미지 크기 기준으로 충돌범위 설정
		newButton->m_CollisionBox[0] = { newButton->m_VertexList[0].p.x, newButton->m_VertexList[0].p.y };
		newButton->m_CollisionBox[1] = { newButton->m_VertexList[3].p.x, newButton->m_VertexList[3].p.y };

		return newButton;
	}

	delete newButton;
	return nullptr;
}

K_Sprite* Sample::CreateNewSprite(ImVec2 orginPos, ImVec2 widthHeight, float depth, float alpha, std::wstring name)
{
	bool success;

	K_Sprite* newSprite = new K_Sprite;
	success = newSprite->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/sprite/Golden/golden knight animation sword right edit_00001.png", L"../../data/shader/DefaultObject_Orgin.txt");
	
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
		if (name == L"") newSprite->m_szObjName = std::to_wstring(newSprite->m_ID);
		else newSprite->m_szObjName = name;

		return newSprite;
	}
	
	delete newSprite;
	return nullptr;
}

void Sample::Clear()
{
	for (auto Iter : m_pUIMap) { Iter.second->Release(); }
	m_pUIMap.clear();
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
		// 실질적으로 여기만 건드리면 됨
		for (auto Iter : m_pUIMap) { Iter.second->Save(outfile); }
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
	std::wstring newFileName = FileOpen();
	if (newFileName == L"") return;
	Clear();

	m_szFileName = I_Tex.GetSplitName(newFileName);
	m_szFileContent = FileReadToString(newFileName);
	m_splitContent = SplitString(m_szFileContent, L"="); // 일단 오브젝트 단위로 쪼갬
	
	UIType			Type;
	int				ID;
	std::wstring	Name;
	ImVec2			OrginPos;
	float			Depth;
	float			Alpha;

	std::vector<CI> cutInfoList;
	int				cutNum;
	ImVec2			WidthHeight;
	ImVec2			uvTL;
	ImVec2			uvBR;
	std::wstring	TextureName;
	std::wstring	ShaderName;

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
			else if (LineContent[0] == L"Name") { Name = LineContent[1]; }
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
		if (Type == IMAGE) // 그냥 이미지
		{
			K_UIObject* newRect = CreateNewRect(OrginPos, cutInfoList[0].pxWH, Depth, Alpha, Name);
			if (newRect != nullptr)
			{
				for (auto CI : cutInfoList) { newRect->AddCut(CI); }
				newRect->m_pTextureSRV = newRect->m_pCutInfoList[0]->tc->GetSRV();
			}
		}
		else if (Type == BUTTON) // 버튼
		{
			K_Button* newButton = CreateNewButton(OrginPos, cutInfoList[0].pxWH, Depth, Alpha, Name);
			if (newButton != nullptr)
			{
				for (auto CI : cutInfoList) { newButton->AddCut(CI); }
			}
		}

		cutInfoList.clear();
	}
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

ImVec2 Sample::AtoV(float array[2])
{
	return ImVec2(array[0], array[1]);
}

float* Sample::VtoA(ImVec2 vector)
{
	float array[2] = { vector[0],vector[1] };
	return array;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Sample demo;
	demo.SetWindow(hInstance, L"UI Tool", 1600, 900);
	demo.Run();
	return 1;
}

//GAME_RUN(CreateObject, 1600, 900)
