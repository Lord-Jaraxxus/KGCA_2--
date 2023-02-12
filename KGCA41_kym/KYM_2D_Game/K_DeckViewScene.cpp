#include "K_DeckViewScene.h"

bool K_DeckViewScene::Init()
{
	// ī�� �ؽ�ó��
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

	// ȭ�鿡 ��� 10�� ������Ʈ, �ϴ� ��ư���� ����
	K_Texture* pCardMask = I_Tex.Load(L"../../data/img/card/4_Hemokinesis.png"); // ����ũ, ī�� �̹��� ���� �� ����ϴϱ� ��� �� ����
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

		if (m_iDeckViewState == 0) { m_iSceneState = 1; } // �ʿ��� ������ ������
		else if (m_iDeckViewState == 1 || m_iDeckViewState == 2) { m_iSceneState = 2; } // �������� ������ ������
		else if (m_iDeckViewState == 3 || m_iDeckViewState == 4) { m_iSceneState = 3; } // ����Ʈ�������� ������ ������
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
		DeckState += L"�� ����Ʈ ";
	}
	else if (m_iDeckViewState == 1 || m_iDeckViewState == 3)
	{
		DeckState.clear();
		DeckState += L"���� �����ִ� ī��� ";
	}
	else if (m_iDeckViewState == 2 || m_iDeckViewState == 4)
	{
		DeckState.clear();
		DeckState += L"������ ī��� ";
	}


	I_Write.Draw(700, 20, DeckState, { 1.0f, 1.0f, 1.0f, 1 });
}

void K_DeckViewScene::CardSetting()
{
	for (int i = 0; i < m_pCardList.size(); i++) // �ƹ��͵� �Ⱥ��̴� �ؽ�ó�� �ʱ�ȭ
	{
		m_pCardList[i]->m_pTexture = m_pCardTextureList.back();
	}

	if (m_iDeckViewState == 0) // �� -> �� ����Ʈ
	{
		for (int i = 0; i < m_pDeck->m_DeckList.size(); i++) // ������Ʈ�� 10�� �Ѿ�� ������������, 10���� �ȳѱ�� �� ����
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_DeckList[i]];
		}
	}

	else if(m_iDeckViewState == 1 || m_iDeckViewState == 3) // ���� -> ���� ī�� ����Ʈ
	{
		for (int i = 0; i < m_pDeck->m_RemainingCardList.size(); i++) 
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_RemainingCardList[i]];
		}
	}

	else if (m_iDeckViewState == 2 || m_iDeckViewState == 4) // ���� -> ���� ī�� ����Ʈ
	{
		for (int i = 0; i < m_pDeck->m_DiscardList.size(); i++)
		{
			m_pCardList[i]->m_pTexture = m_pCardTextureList[m_pDeck->m_DiscardList[i]];
		}
	}

}
