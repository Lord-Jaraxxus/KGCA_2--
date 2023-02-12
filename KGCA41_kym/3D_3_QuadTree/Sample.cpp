#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	m_pDL = new K_ShapeDirectionLine;
	m_pDL->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultColor.txt");
	m_pDL->m_matWorld = m_pZLine->m_matWorld.Scale(100.0f, 100.0f, 100.0f);

	m_pBG = new K_BaseObject;
	m_pBG->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");

	m_pBoxObjA = new K_Box;
	m_pBoxObjA->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld.Translation(0, 0, 2);

	m_pBoxADL = new K_ShapeDirectionLine;
	m_pBoxADL->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultColor.txt");
	m_pBoxADL->m_matWorld = m_pBoxADL->m_matWorld.Scale(100.0f, 100.0f, 100.0f);
	m_pBoxADL->m_matWorld = m_pBoxADL->m_matWorld * m_pBoxObjA->m_matWorld;

	m_pBoxObjB = new K_Box;
	m_pBoxObjB->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	m_pBoxObjB->m_matWorld = m_pBoxObjB->m_matWorld.Translation(2, 0, 4);

	m_pBoxBDL = new K_ShapeDirectionLine;
	m_pBoxBDL->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultColor.txt");
	m_pBoxBDL->m_matWorld = m_pBoxBDL->m_matWorld.Scale(100.0f, 100.0f, 100.0f);
	m_pBoxBDL->m_matWorld = m_pBoxBDL->m_matWorld * m_pBoxObjB->m_matWorld;



	m_pMainCamera = new K_Camera;
	m_pMainCamera->CreateViewMatrix(k_Vector(5, 1, -1.0f), k_Vector(0, 0, 0), k_Vector(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 1000000.0f, 3.141592f * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_pDebugCamera = new K_DebugCamera;
	m_pDebugCamera->CreateViewMatrix(k_Vector(5, 1, -1.0f), k_Vector(0, 0, 0), k_Vector(0, 1, 0));
	m_pDebugCamera->CreateProjMatrix(1.0f, 1000000.0f, 3.141592f * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_pMap = new K_Map;
	// 2*n½Â+1
	m_pMap->Build(4 + 1, 4 + 1);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/map/Tile50.jpg", L"../../data/shader/DefaultObject.txt");

	m_QuadTree.Create(m_pDebugCamera, m_pMap);
	return true; 
}

bool Sample::Frame()
{
	m_pDL->Frame();
	m_pBG->Frame();
	m_pMainCamera->Frame();
	m_pDebugCamera->Frame();
	m_pBoxObjA->Frame(); 
	m_pBoxADL->Frame();
	m_pBoxObjB->Frame();
	m_pBoxBDL->Frame();

	K_Matrix temp = m_pBoxObjA->m_matWorld; 
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld.RotationY(3.141592f * g_fSecondPerFrame);
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld * temp;
	m_pBoxADL->m_matWorld = m_pBoxADL->m_matWorld.Scale(100.0f, 100.0f, 100.0f);
	m_pBoxADL->m_matWorld = m_pBoxADL->m_matWorld * m_pBoxObjA->m_matWorld;

	return true;
}

bool Sample::Render()
{
	m_pDL->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_pDL->Render();

	m_pBG->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_pBG->Render();

	m_pMap->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_QuadTree.Frame();
	m_QuadTree.Render();

	bool bRender = m_pDebugCamera->m_vFrustum.ClassifyPoint(m_pBoxObjA->m_vPos);
	if (bRender)
	{
		m_pBoxObjA->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
		m_pBoxObjA->Render();
		m_pBoxADL->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
		m_pBoxADL->Render();
	}

	bRender = m_pDebugCamera->m_vFrustum.ClassifyPoint(m_pBoxObjB->m_vPos);
	if (bRender)
	{
		m_pBoxObjB->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
		m_pBoxObjB->Render();
		m_pBoxBDL->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
		m_pBoxBDL->Render();
	}

	//m_pBoxObjA->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//m_pBoxObjA->Render();
	//m_pBoxADL->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//m_pBoxADL->Render();

	//m_pBoxObjB->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//m_pBoxObjB->Render();
	//m_pBoxBDL->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	//m_pBoxBDL->Render();

	return true;
}

bool Sample::Release()
{
	m_pDL->Release();
	delete m_pDL;
	//m_pZLine->Release();
	//delete m_pZLine;
	m_pBG->Release();
	delete m_pBG;
	m_pMap->Release();
	delete m_pMap;
	m_pBoxObjA->Release();
	delete m_pBoxObjA;
	m_pBoxObjB->Release();
	delete m_pBoxObjB;
	m_pMainCamera->Release();
	delete m_pMainCamera;
	m_pDebugCamera->Release();
	delete m_pDebugCamera;

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
