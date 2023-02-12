#include "Sample.h"
#include "K_TextureManager.h"

bool Sample::Init() 
{
	m_pBoxObjA = new K_Box;
	m_pBoxObjA->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld.Translation(100, 0, 10);


	m_pMainCamera = new K_Camera;
	m_pMainCamera->CreateViewMatrix(k_Vector(5, 1, -1.0f), k_Vector(0, 0, 0), k_Vector(0, 1, 0));
	m_pMainCamera->CreateProjMatrix(1.0f, 1000000.0f, 3.141592f * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_pDebugCamera = new K_DebugCamera;
	m_pDebugCamera->CreateViewMatrix(k_Vector(0, 100, -10.0f), k_Vector(0, 0, 1), k_Vector(0, 1, 0));
	m_pDebugCamera->CreateProjMatrix(1.0f, 1000000.0f, 3.141592f * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);


	m_pBG = new K_BaseObject;
	m_pBG->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/img/map.jpg", L"../../data/shader/DefaultObject.txt");

	m_pMap = new K_Map;
	m_pMap->LoadHeightMap(m_pd3dDevice, m_pImmediateContext, L"../../data/map/heightMap513.bmp");
	m_pMap->Build(m_pMap->m_iNumCols, m_pMap->m_iNumRows);
	//m_pMap->Build(5, 5);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/map/Tile50.jpg", L"../../data/shader/DefaultObject.txt");

	m_QuadTree.Create(m_pDebugCamera, m_pMap);

	return true; 
}

bool Sample::Frame()
{
	m_pMainCamera->Frame();
	m_pDebugCamera->Frame();

	m_pBoxObjA->m_matWorld._41 = m_pDebugCamera->m_matWorld._41;
	m_pBoxObjA->m_matWorld._43 = m_pDebugCamera->m_matWorld._43 + 10;
	//if (I_Input.GetKey('4') == KEY_HOLD) { m_pBoxObjA->m_matWorld._41 += g_fSecondPerFrame * 10.0f; }
	//if (I_Input.GetKey('6') == KEY_HOLD) { m_pBoxObjA->m_matWorld._41 -= g_fSecondPerFrame * 10.0f; }
	float fMapHeight = m_pMap->GetHeight(m_pBoxObjA->m_matWorld._41, m_pBoxObjA->m_matWorld._43);
	m_pBoxObjA->m_matWorld._42 = fMapHeight;
	m_pBoxObjA->Frame();

	K_Matrix temp = m_pBoxObjA->m_matWorld; 
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld.RotationY(3.141592f * g_fSecondPerFrame);
	m_pBoxObjA->m_matWorld = m_pBoxObjA->m_matWorld * temp;

	return true;
}

bool Sample::Render()
{
	m_pMap->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_QuadTree.Frame();
	m_QuadTree.Render();

	m_pBoxObjA->SetMatrix(nullptr, &m_pDebugCamera->m_matView, &m_pDebugCamera->m_matProj);
	m_pBoxObjA->Render();
	

	return true;
}

bool Sample::Release()
{
	m_pMap->Release();
	delete m_pMap;
	m_pBoxObjA->Release();
	delete m_pBoxObjA;

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
