#include "pch.h"

#include "K_Character.h"

bool K_Character::UpdateFrame(ID3D11DeviceContext* pContext)
{
	m_fAnimeCurrentFrame = m_fAnimeCurrentFrame + g_fSecondPerFrame * m_fAnimeSpeed * m_AnimeScene.fFrameSpeed * m_fAnimeInverse;
	if (m_fAnimeCurrentFrame > m_ActionCurrent.iEndFrame || m_fAnimeCurrentFrame < m_ActionCurrent.iStartFrame)
	{
		m_fAnimeCurrentFrame = min(m_fAnimeCurrentFrame, m_ActionCurrent.iEndFrame);
		m_fAnimeCurrentFrame = max(m_fAnimeCurrentFrame, m_ActionCurrent.iStartFrame);
		//m_fAnimeInverse *= -1.0f;
	}

	if (m_ActionCurrent.bLoop)
	{
		//if (m_fAnimeCurrentFrame >= m_ActionCurrent.iEndFrame) // ½ÜÄÚµå
		if (m_fAnimeCurrentFrame >= m_ActionCurrent.iEndFrame)
		{
			m_fAnimeCurrentFrame = m_ActionCurrent.iStartFrame;
		}
	}
	else
	{
		if (m_fAnimeCurrentFrame >= m_ActionCurrent.iEndFrame)
		{
			//m_ActionCurrent = m_ActionList.find(L"walking")->second;
			m_fAnimeCurrentFrame = m_ActionCurrent.iEndFrame;
		}
	}

	if (m_pCurrentActionFile)
	{
		m_pCurrentActionFile->UpdateSkeleton(pContext, m_fAnimeCurrentFrame);
		m_pFbxFile->UpdateSkinning(pContext, m_pCurrentActionFile->m_cbDataBone);
	}
	else
	{
		//m_pFbxFile->UpdateFrameTest(pContext, m_AnimeScene, m_fAnimeCurrentFrame);
		//m_pFbxFile->UpdateFrame(pContext);
		m_pFbxFile->UpdateSkeleton(pContext, m_fAnimeCurrentFrame);
		m_pFbxFile->UpdateSkinning(pContext, m_pFbxFile->m_cbDataBone);
	}

	//for (int iBone = 0; iBone < m_pFbxFile->m_pObjectListS.size(); iBone++)
	//{
	//	m_pFbxFile->m_cbDataBone.matBone[iBone] = m_pFbxFile->m_cbDataBone.matBone[iBone].Transpose();
	//}
	//pContext->UpdateSubresource(m_pFbxFile->m_pConstantBufferBone, 0, nullptr, &m_pFbxFile->m_cbDataBone, 0, 0);

	//for (int iDraw = 0; iDraw < m_pFbxFile->m_pDrawObjListS.size(); iDraw++) 
	//{
	//	pContext->UpdateSubresource(m_pFbxFile->m_pDrawObjListS[iDraw]->m_pConstantBufferBone, 0, nullptr, &m_pFbxFile->m_pDrawObjListS[iDraw]->m_cbDataBone, 0, 0);
	//}
	

	return true;
}

void K_Character::SetMatrix(K_Matrix* matWorld, K_Matrix* matView, K_Matrix* matProj)
{
	if (matWorld != nullptr) m_matWorld = *matWorld;
	if (matView != nullptr) m_matView = *matView;
	if (matProj != nullptr) m_matProj = *matProj;
}

bool K_Character::Render(ID3D11DeviceContext* pContext)
{
	UpdateFrame(pContext);

	pContext->VSSetConstantBuffers(1, 1, &m_pFbxFile->m_pConstantBufferBone);
	for (int iObj = 0; iObj < m_pFbxFile->m_pDrawObjListS.size(); iObj++)
	{
		K_FbxObjectSkinning* pObj = m_pFbxFile->m_pDrawObjListS[iObj];
		//pObj->m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pFbxFile->m_pConstantBufferBone); // ½ºÅ°´×ÀÌ ¾Æ´Ò¶§

		pObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pObj->Render();
	}

	return true;
}

bool K_Character::Release()
{
	return true;
}
