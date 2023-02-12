#include "K_DeckViewScene.h"

bool K_DeckViewScene::Init()
{
	// 카드 텍스처들
	K_Texture* pTextureCard0 = I_Tex.Load(L"../../data/img/card/0_Strike.png");
	K_Texture* pTextureCard1 = I_Tex.Load(L"../../data/img/card/1_Defend.png");
	K_Texture* pTextureCard2 = I_Tex.Load(L"../../data/img/card/2_PommelStrike.png");
	K_Texture* pTextureCard3 = I_Tex.Load(L"../../data/img/card/3_ShrugItOff.png");
	K_Texture* pTextureCard4 = I_Tex.Load(L"../../data/img/card/4_Hemokinesis.png");
	K_Texture* pTextureCard5 = I_Tex.Load(L"../../data/img/card/5_Bludgeon.png");
	K_Texture* pTextureCard6 = I_Tex.Load(L"../../data/img/card/6_IronWave.png");
	K_Texture* pTextureVoidCard = I_Tex.Load(L"../../data/img/card/Nothing.png");
	m_pCardTextureList.push_back(pTextureCard0);
	m_pCardTextureList.push_back(pTextureCard1);
	m_pCardTextureList.push_back(pTextureCard2);
	m_pCardTextureList.push_back(pTextureCard3);
	m_pCardTextureList.push_back(pTextureCard4);
	m_pCardTextureList.push_back(pTextureCard5);
	m_pCardTextureList.push_back(pTextureCard6);
	m_pCardTextureList.push_back(pTextureVoidCard);

	// 화면에 띄울 10개 오브젝트, 일단 버튼으로 만듬
	K_Texture* pCardMask = I_Tex.Load(L"../../data/img/card/4_Hemokinesis.png"); // 마스크, 카드 이미지 대충 다 비슷하니까 얘로 다 쓰자
	for (int i = 0; i < 10; i++) 
	{
		K_Button* Card = new K_Button;
		Card->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/Nothing.png", L"../KymGameCore/MaskShape_Png.txt");
		Card->SetMask(pCardMask);
		Card->SetRect({ 0, 0, 250, 318 });
		Card->m_fSize = 2.0f;
		Card->SetPosition({ 200.0f + i*300.0f, 170.0f });
		if (i > 4) { Card->SetPosition({ 200.0f + (i-5) * 300.0f, 600.0f }); }
		m_pCardList.push_back(Card);
	}

	K_Texture* pBackButtonTexture = I_Tex.Load(L"../../data/img/card/BackButtonWhite.png");
	m_pBackButton = new K_Button;
	m_pBackButton->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/BackButtonWhite.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pBackButton->SetMask(pBackButtonTexture);
	m_pBackButton->SetRect({ 0, 0, (float)m_pBackButton->m_pTexture->m_Desc.Width, (float)m_pBackButton->m_pTexture->m_Desc.Height });
	m_pBackButton->m_fSize = 0.7f;
	m_pBackButton->SetPosition({ 1450.0f, 700.0f });
	
	return true;
}

bool K_DeckViewScene::Frame()
{
	CardSetting();
	for (auto obj : m_pCardList) { obj->Frame(); }
	m_pBackButton->Frame();

	return true;
}

bool K_DeckViewScene::Render()
{
	for (auto obj : m_pCardList) { obj->Render(); }
	m_pBackButton->Render();
	DeckStateTitle();

	if (m_pBackButton->m_bButtonPushed == true)
	{
		m_pBackButton->m_bButtonPushed = false;

		if (m_iDeckViewState == 0) { m_iSceneState = 1; } // 맵에서 덱으로 왔을때
		else if (m_iDeckViewState == 1 || m_iDeckViewState == 2) { m_iSceneState = 2; } // 전투에서 덱으로 왔을때
		else if (m_iDeckViewState == 3 || m_iDeckViewState == 4) { m_iSceneState = 3; } // 엘리트전투에서 덱으로 왔을때
	}

	return true;
}

bool K_DeckViewScene::Release()
{
	for (auto obj : m_pCardList) { obj->Release(); }
	m_pBackButton->Release();

	return true;
}

void K_DeckViewScene::DeckStateTitle()
{
	std::wstring DeckState;

	if (m_iDeckViewState == 0) 
	{
		DeckState.clear();
		DeckState += L"덱 리스트 ";
	}
	else if (m_iDeckViewState == 1 || m_iDeckViewState == 3)
	{
		DeckState.clear();
		DeckState += L"덱에 남아있는 카드들 ";
	}
	else if (m_iDeckViewState == 2 || m_iDeckViewState == 4)
	{
		DeckState.clear();
		DeckState += L"버려진 카드들 ";
	}


	I_Write.Draw(700, 20, DeckState, { 1.0f, 1.0f, 1.0f, 1 });
}

void K_DeckViewScene::CardSetting()
{
	for (int i = 0; i < m_pCardList.size(); i++) // 아무것도 안보이는 텍스처로 초기화
	{
		m_pCardList[i]->m_pTexture = m_pCardTextureList.back();
	}

	if (m_iDeckViewState == 0) // 맵 -> 덱 리스트
	{
		for (int i = 0; i < m_pDeck->m_DeckList.size(); i++) // 덱리스트가 10장 넘어가면 에러나겠지만, 10장을 안넘기면 됨 ㅋㅋ
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_DeckList[i]];
		}
	}

	else if(m_iDeckViewState == 1 || m_iDeckViewState == 3) // 전투 -> 남은 카드 리스트
	{
		for (int i = 0; i < m_pDeck->m_RemainingCardList.size(); i++) 
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_RemainingCardList[i]];
		}
	}

	else if (m_iDeckViewState == 2 || m_iDeckViewState == 4) // 전투 -> 버린 카드 리스트
	{
		for (int i = 0; i < m_pDeck->m_DiscardList.size(); i++)
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_DiscardList[i]];
		}
	}

}
