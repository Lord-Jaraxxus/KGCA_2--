#include "Sample.h"
#include "K_TextureManager.h"
#include "K_ShaderManager.h"

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

bool Sample::Init() 
{                                                                                                                                        
	I_Tex.SetDevice(m_pd3dDevice, m_pImmediateContext);
	I_Shader.SetDevice(m_pd3dDevice, m_pImmediateContext);

	I_Tex.Load(L"../../data/img/1.png");
	I_Tex.Load(L"../../data/img/2.png");
	I_Tex.Load(L"../../data/img/3.png");
	I_Tex.Load(L"../../data/img/4.png");
	I_Tex.Load(L"../../data/img/5.png");

	for (int iObj = 0; iObj < 10; iObj++) 
	{
		K_BaseObject* pObject = new K_BaseObject;
		pObject->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../data/img/2.png",
			L"../../data/shader/DefaultShape.txt");
		m_pObjectList.push_back(pObject);
	} 
	return true; 
}

bool Sample::Frame()
{
	for (auto obj : m_pObjectList) { obj->Frame(); }
	return true;
}

bool Sample::Render()
{
	static float fTime = 0.0f;
	fTime += m_fElapseTimer;
	if (fTime > 1.0f) 
	{
		m_iTexIndex++;
		if (m_iTexIndex > 5) m_iTexIndex = 1;
		fTime = fTime - 1.0f;
	}

	std::wstring name = L"../../data/img/";
	name += std::to_wstring(m_iTexIndex);
	name += L".png";

	m_pObjectList[0]->m_pTexture = I_Tex.Find(name);
	m_pObjectList[0]->Render();

	//for (auto obj : m_pObjectList) { obj->Render(); }

	return true;
}

bool Sample::Release()
{
	for (auto obj : m_pObjectList) 
	{
		obj->Release(); 
		delete obj;
	}
	return true; 
}



GAME_RUN(CreateObject, 1024, 768)