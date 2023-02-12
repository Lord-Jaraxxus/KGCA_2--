#include "pch.h"

#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	K_FBXLoader* pFbxLoaderA = new K_FBXLoader;
	if (pFbxLoaderA->Init())
	{
		//if (pFbxLoaderA->Load("../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx")) 
		if (pFbxLoaderA->Load("../../data/fbx/Man.fbx"))
		{
			pFbxLoaderA->CreateConstantBuffer(m_pd3dDevice);
		}
	}
	m_FBXList.push_back(pFbxLoaderA);

	K_FBXLoader* pFbxLoaderB = new K_FBXLoader;
	if (pFbxLoaderB->Init())
	{
		if (pFbxLoaderB->Load("../../data/fbx/Swat.fbx"))
		{
			pFbxLoaderB->CreateConstantBuffer(m_pd3dDevice);
		}
	}
	m_FBXList.push_back(pFbxLoaderB);

	K_FBXLoader* pFbxLoaderC = new K_FBXLoader;
	if (pFbxLoaderC->Init())
	{
		//if (pFbxLoaderC->Load("../../data/fbx/Swat@turning_right_45_degrees.fbx"))
		if (pFbxLoaderC->Load("../../data/fbx/Swat@rifle_jump.fbx"))
		{
			pFbxLoaderC->CreateConstantBuffer(m_pd3dDevice);
		}
	}
	m_FBXList.push_back(pFbxLoaderC);



	W_STR szDefaultDir = L"../../data/fbx/";
	std::wstring shaderfilename = L"../../data/shader/Skinning.txt";
	//std::wstring shaderfilename = L"../../../KGCA/data/shader/DefaultObject.txt";
	for (auto fbx : m_FBXList)
	{
		for (int iObj = 0; iObj < fbx->m_pDrawObjListS.size(); iObj++)
		{
			K_FbxObjectSkinning* pObj = fbx->m_pDrawObjListS[iObj];
			std::wstring  szLoad = szDefaultDir + pObj->m_vTextureNameList[0];

			pObj->Create(m_pd3dDevice, m_pImmediateContext, szLoad, shaderfilename);
		}
	}

	for (int iObj = 0; iObj < 5; iObj++)
	{
		K_Character* pNpc = new K_Character;
		pNpc->m_pFbxFile = m_FBXList[0];
		pNpc->m_matWorld._41 = 100 + iObj * 100.0f;
		m_NpcList.push_back(pNpc);
	}
	m_NpcList[0]->m_ActionCurrent.iStartFrame = 61;
	m_NpcList[0]->m_ActionCurrent.iEndFrame = 91;
	m_NpcList[0]->m_fAnimeCurrentFrame = m_NpcList[0]->m_AnimeScene.iStartFrame;
	m_NpcList[1]->m_ActionCurrent.iStartFrame = 92;
	m_NpcList[1]->m_ActionCurrent.iEndFrame = 116;
	m_NpcList[1]->m_fAnimeCurrentFrame = m_NpcList[1]->m_AnimeScene.iStartFrame;
	m_NpcList[2]->m_ActionCurrent.iStartFrame = 120;
	m_NpcList[2]->m_ActionCurrent.iEndFrame = 204;
	m_NpcList[2]->m_fAnimeCurrentFrame = m_NpcList[2]->m_AnimeScene.iStartFrame;
	m_NpcList[3]->m_ActionCurrent.iStartFrame = 205;
	m_NpcList[3]->m_ActionCurrent.iEndFrame = 289;
	m_NpcList[3]->m_fAnimeCurrentFrame = m_NpcList[3]->m_AnimeScene.iStartFrame;
	m_NpcList[4]->m_ActionCurrent.iStartFrame = 0;
	m_NpcList[4]->m_ActionCurrent.iEndFrame = 60;
	m_NpcList[4]->m_fAnimeCurrentFrame = m_NpcList[4]->m_AnimeScene.iStartFrame;

	m_UserCharacter = new K_Character;
	m_UserCharacter->m_pFbxFile = pFbxLoaderB;
	m_UserCharacter->m_pCurrentActionFile = pFbxLoaderC;
	m_UserCharacter->m_AnimeScene = m_UserCharacter->m_pCurrentActionFile->m_AnimeScene;
	m_UserCharacter->m_ActionFileList.insert(std::make_pair(L"walking", pFbxLoaderC));
	m_UserCharacter->m_ActionCurrent.iStartFrame = 0;
	m_UserCharacter->m_ActionCurrent.iEndFrame = 50; //pFbxLoaderC->m_AnimeScene.iEndFrame; -> 얘는 점프한번하고 한참동안 가만있음

	m_pDebugCamera = new K_DebugCamera;
	m_pDebugCamera->CreateViewMatrix(k_Vector(50, 6, -50), k_Vector(0, 6, 0), k_Vector(0, 1, 0));
	m_pDebugCamera->CreateProjMatrix(1.0f, 1000000.0f, 3.141592f * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	return true; 
}

bool Sample::Frame()
{
	m_pDebugCamera->Frame();
	for (auto fbx : m_FBXList)
	{
		//fbx->UpdateFrame(m_pImmediateContext);
	}

	return true;
}

bool Sample::Render()
{
	for (int iNpc = 0; iNpc < m_NpcList.size(); iNpc++)
	{
		m_NpcList[iNpc]->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
		m_NpcList[iNpc]->Render(m_pImmediateContext);
	}

	m_UserCharacter->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_UserCharacter->Render(m_pImmediateContext);

	//m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_FBXList[1]->m_pConstantBufferBone);
	//for (int iObj = 0; iObj < m_FBXList[1]->m_pDrawObjListS.size(); iObj++)
	//{
	//	K_FbxObjectSkinning* pObj = m_FBXList[1]->m_pDrawObjListS[iObj];
	//	pObj->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//	pObj->Render();
	//}	
	//m_FBXList[0]->UpdateFrame(m_pImmediateContext);
	//for (int iObj = 0; iObj < m_FBXList[0]->m_pDrawObjListS.size(); iObj++)
	//{
	//	K_FbxObjectSkinning* pObj = m_FBXList[0]->m_pDrawObjListS[iObj];
	//	pObj->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//	pObj->Render();
	//}

	return true;
}

bool Sample::Release()
{
	m_pDebugCamera->Release();
	delete m_pDebugCamera;

	for (auto fbx : m_FBXList)
	{
		fbx->Release();
	}

	return true; 
}
