#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	//if (m_FBXLoader.Init())
	//{
	//	m_FBXLoader.Load("../../data/fbx/box.fbx");
	//}

	//for (int iObj = 0; iObj < m_FBXLoader.m_pDrawObjList.size(); iObj++)
	//{
	//	K_BaseObject* pObj = m_FBXLoader.m_pDrawObjList[iObj];
	//	pObj->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultColor.txt");
	//}

	//K_FBXLoader* pFbxLoaderC = new K_FBXLoader;
	//if (pFbxLoaderC->Init())
	//{
	//	pFbxLoaderC->Load("../../data/fbx/MultiCameras.fbx");
	//}
	//m_FBXList.push_back(pFbxLoaderC);

	//K_FBXLoader* pFbxLoaderA = new K_FBXLoader;
	//if (pFbxLoaderA->Init())
	//{
	//	//if (pFbxLoaderA->Load("../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx")) 
	//	if (pFbxLoaderA->Load("../../data/fbx/Man.fbx"))
	//	{
	//		pFbxLoaderA->CreateConstantBuffer(m_pd3dDevice);
	//	}
	//}
	//m_FBXList.push_back(pFbxLoaderA);

	//K_FBXLoader* pFbxLoaderB = new K_FBXLoader;
	//if (pFbxLoaderB->Init())
	//{
	//	pFbxLoaderB->Load("../../data/fbx/sm_rock.fbx");
	//}
	//m_FBXList.push_back(pFbxLoaderB);



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
		fbx->UpdateFrame(m_pImmediateContext);
	}

	return true;
}

bool Sample::Render()
{
	//k_Vector vLight(0, 0, 1);
	//K_Matrix matRotation;
	//matRotation = matRotation.RotationY(g_fGameTimer);
	//vLight = vLight * matRotation;
	//vLight.Normalize();

	//for (int iModel = 0; iModel < m_FBXList.size(); iModel++)
	//{
	//	for (int iObj = 0; iObj < m_FBXList[iModel]->m_pDrawObjList.size(); iObj++)
	//	{
	//		K_FbxObject* pObj = m_FBXList[iModel]->m_pDrawObjList[iObj];
	//		pObj->m_cbData.x = vLight.x;
	//		pObj->m_cbData.y = vLight.y;
	//		pObj->m_cbData.z = vLight.z;
	//		pObj->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//		pObj->Render();
	//	}
	//}

	for (int iFbxFile = 0; iFbxFile < m_FBXList.size(); iFbxFile++)
	{
		m_FBXList[iFbxFile]->m_fAnimeSpeed = 1.0f;
		//m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_FBXList[iFbxFile]->m_pConstantBufferBone);
		for (int iObj = 0; iObj < m_FBXList[iFbxFile]->m_pDrawObjListS.size(); iObj++)
		{
			K_FbxObjectSkinning* pObj = m_FBXList[iFbxFile]->m_pDrawObjListS[iObj];

			if (pObj->m_bSkinned == true)
			{
				m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_FBXList[iFbxFile]->m_pDrawObjListS[iObj]->m_pConstantBufferBone);
			}
			else 
			{
				m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_FBXList[iFbxFile]->m_pConstantBufferBone);
			}

			pObj->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
			pObj->Render();
		}
	}

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


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow) 
//{
//    Sample demo;
//    demo.SetWindow(hInstance);
//    demo.Run();
//    return 1;
//}  

GAME_RUN(CreateObject, 800, 900)
