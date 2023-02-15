#include "K_Sprite.h"
#include "K_Timer.h"

bool K_Sprite::Frame()
{
	UpdateCut();

	return true;
}

bool K_Sprite::Release()
{
	K_BaseObject::Release();

	for (auto CI : m_pCutInfoList)
	{
		CI->tc->Release();
		delete CI;
	}

	return true;
}

bool K_Sprite::UpdateCut()
{
	m_pTextureSRV = m_pCutInfoList[m_iCurrentFrame]->tc->m_pTextureSRV;
	SetUV(m_pCutInfoList[m_iCurrentFrame]->uvTL, m_pCutInfoList[m_iCurrentFrame]->uvBR);
	UpdateVertexBuffer();

	if (I_Timer.m_fFPSTimer > m_iCurrentFps / m_iMaxFps)
	{
		float fpstimer = I_Timer.m_fFPSTimer;

		m_iCurrentFrame++;
		if (m_iCurrentFrame >= m_iMaxFrame) { m_iCurrentFrame = 0; }

		m_iCurrentFps++;
		if (m_iCurrentFps >= m_iMaxFps) { m_iCurrentFps = 0; }
	}

	return true;
}

bool K_Sprite::AddCut(ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn)
{
	CI* newCI = new CI;
	newCI->uvTL = uvTL;
	newCI->uvBR = uvBR;
	newCI->tn = tn;
	newCI->sn = sn;

	K_Texture* newTexture = I_Tex.Load(tn);
	if (newTexture == nullptr) { return false; }
	
	newCI->tc = newTexture;
	m_pCutInfoList.push_back(newCI);

	return true;
}
