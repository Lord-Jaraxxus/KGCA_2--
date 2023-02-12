#include "Sample.h"

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
	m_2Dgamecore.Init();
	m_2Dgamecore.player2D.SetDevice(m_pd3dDevice, m_pImmediateContext);
	m_2Dgamecore.player2D.Init();

	if (!m_2Dgamecore.AllObjectList2D.empty())
	{
		for (int iObj = 0; iObj < m_2Dgamecore.AllObjectList2D.size(); iObj++)
		{
			m_2Dgamecore.AllObjectList2D[iObj]->SetDevice(m_pd3dDevice, m_pImmediateContext);
			m_2Dgamecore.AllObjectList2D[iObj]->Init();
		}
	}

	return true; 
}

bool Sample::Frame()
{
	m_2Dgamecore.Frame(m_ElapseTimer, m_fGameTimer);
	return true;
}

bool Sample::Render()
{
	m_2Dgamecore.Render();
	return true;
}

bool Sample::Release()
{
	m_2Dgamecore.Release();
	return true; 
}



GAME_RUN(OctreeDraw, 1024, 768)