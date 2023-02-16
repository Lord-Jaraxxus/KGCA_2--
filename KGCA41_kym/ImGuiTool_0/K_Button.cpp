#include "K_Button.h"
#include "K_Input.h"

bool K_Button::Frame() 
{
	SetState();
	m_pTextureSRV = m_pTextureList[m_BS]->m_pTextureSRV; 
	
	return true;
}

bool K_Button::Release()
{
	K_BaseObject::Release();

	for (auto texture : m_pTextureList) 
	{
		texture->Release();
	}

	return true;
}

bool K_Button::AddTexture(std::wstring filename)
{
	K_Texture* newTexture = I_Tex.Load(filename);

	if (newTexture != nullptr) 
	{
		m_pTextureList.push_back(newTexture);
		return true;
	} 

	return false;
}

bool K_Button::SetState()
{
	ImVec2 vMousePos;
	vMousePos.x = I_Input.m_ptPos.x;
	vMousePos.y = I_Input.m_ptPos.y;	
	
	float mouseNdcX = (vMousePos.x / g_rtClient.right) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((vMousePos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	if (m_bIsDisable) m_BS = DISABLE; //비활성화 

	else if (m_VertexList[0].p.x <= mouseNdcX && m_VertexList[3].p.x >= mouseNdcX &&
			 m_VertexList[0].p.y >= mouseNdcY && m_VertexList[3].p.y <= mouseNdcY)
	{
		if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH || I_Input.GetKey(VK_LBUTTON) == KEY_HOLD) { m_BS = CLICK; }
		else { m_BS = HOVER; }
	}

	else m_BS = NORMAL;

	return true;
}
