#include "K_CardRewardScene.h"

bool K_CardRewardScene::Init()
{
	K_Texture* pRewardCardMask = I_Tex.Load(L"../../data/img/card/4_Hemokinesis.png"); // 마스크, 카드 이미지 대충 다 비슷하니까 얘로 다 쓰자

	m_pRewardCard1 = new K_Button;
	m_pRewardCard1->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/4_Hemokinesis.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pRewardCard1->SetMask(pRewardCardMask);
	m_pRewardCard1->SetRect({ 0, 0, (float)m_pRewardCard1->m_pTexture->m_Desc.Width, (float)m_pRewardCard1->m_pTexture->m_Desc.Height });
	m_pRewardCard1->m_fSize = 2.0f;
	m_pRewardCard1->SetPosition({ 300, 400 });
	

	m_pRewardCard2 = new K_Button;
	m_pRewardCard2->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/5_Bludgeon.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pRewardCard2->SetMask(pRewardCardMask);
	m_pRewardCard2->SetRect({ 0, 0, (float)m_pRewardCard2->m_pTexture->m_Desc.Width, (float)m_pRewardCard2->m_pTexture->m_Desc.Height });
	m_pRewardCard2->m_fSize = 2.0f;
	m_pRewardCard2->SetPosition({ 800, 400 });

	m_pRewardCard3 = new K_Button;
	m_pRewardCard3->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/card/6_IronWave.png", L"../KymGameCore/MaskShape_Png.txt");
	m_pRewardCard3->SetMask(pRewardCardMask);
	m_pRewardCard3->SetRect({ 0, 0, (float)m_pRewardCard3->m_pTexture->m_Desc.Width, (float)m_pRewardCard3->m_pTexture->m_Desc.Height });
	m_pRewardCard3->m_fSize = 2.0f;
	m_pRewardCard3->SetPosition({ 1300, 400 });

	return true;
}

bool K_CardRewardScene::Frame()
{
	m_pRewardCard1->Frame();
	m_pRewardCard2->Frame();
	m_pRewardCard3->Frame();
	return true;
}

bool K_CardRewardScene::Render()
{
	m_pRewardCard1->Render();
	m_pRewardCard2->Render();
	m_pRewardCard3->Render();

	CardSelect();

	std::wstring Notice = L"승리! 보상을 선택하세요.";
	I_Write.Draw(630, 200, Notice, { 1,1,0.5,1 });
	return true;
}

bool K_CardRewardScene::Release()
{
	m_pRewardCard1->Release();
	m_pRewardCard2->Release();
	m_pRewardCard3->Release();
	return true;
}

void K_CardRewardScene::CardSelect()
{
	if (m_pRewardCard1->m_bButtonPushed == true)
	{
		m_pDeck->m_DeckList.push_back(4); // 혈류
		m_iSceneState = 1;
	}
	else if (m_pRewardCard2->m_bButtonPushed == true)
	{
		m_pDeck->m_DeckList.push_back(5); // 몽둥이질
		m_iSceneState = 1;
	}
	else if (m_pRewardCard3->m_bButtonPushed == true)
	{
		m_pDeck->m_DeckList.push_back(6); // 철의 파동
		m_iSceneState = 1;
	}
}
