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
	m_fCurrentFrame = m_fCurrentFrame + (g_fSecondPerFrame * m_iFrameSpeed);
	if (m_fCurrentFrame >= m_iMaxFrame) { m_fCurrentFrame = 0.0f; }

	m_pTextureSRV = m_pCutInfoList[m_fCurrentFrame]->tc->m_pTextureSRV;
	SetPosition(m_OrginPos, PtoN(m_pCutInfoList[m_fCurrentFrame]->pxWH), m_fDepth);
	SetUV(m_pCutInfoList[m_fCurrentFrame]->uvTL, m_pCutInfoList[m_fCurrentFrame]->uvBR);
	UpdateVertexBuffer();

	return true;
}

bool K_Sprite::AddCut(ImVec2 pxWH, ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn)
{
	CI* newCI = new CI;
	newCI->pxWH = pxWH;
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
