#include "K_Button.h"


bool K_Button::Frame() 
{
	m_CollisionBox[0] = { m_VertexList[0].p.x, m_VertexList[0].p.y };
	m_CollisionBox[1] = { m_VertexList[3].p.x, m_VertexList[3].p.y };

	Drag();

	SetState();

	if (m_pCutInfoList.size() == 0) return false;
	m_pTextureSRV = m_pCutInfoList[m_BS]->tc->GetSRV();
	
	SetPosition(m_OrginPos, PtoN(m_pCutInfoList[m_BS]->pxWH), m_fDepth);
	SetUV(m_pCutInfoList[m_BS]->uvTL, m_pCutInfoList[m_BS]->uvBR);
	UpdateVertexBuffer();

	return true;
}


bool K_Button::SetState()
{
	ImVec2 vMousePos;
	vMousePos.x = I_Input.m_ptPos.x;
	vMousePos.y = I_Input.m_ptPos.y;	
	
	float mouseNdcX = (vMousePos.x / g_rtClient.right) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((vMousePos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	if (m_bIsDisable) m_BS = DISABLE; //비활성화 
	//else if (m_VertexList[0].p.x <= mouseNdcX && m_VertexList[3].p.x >= mouseNdcX && m_VertexList[0].p.y >= mouseNdcY && m_VertexList[3].p.y <= mouseNdcY) -> 이건 이미지 크기따라 충돌처리
	else if (m_CollisionBox[0].x <= mouseNdcX && m_CollisionBox[1].x >= mouseNdcX &&
			 m_CollisionBox[0].y >= mouseNdcY && m_CollisionBox[1].y <= mouseNdcY)
	{
		if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH || I_Input.GetKey(VK_LBUTTON) == KEY_HOLD) { m_BS = CLICK; }
		else { m_BS = HOVER; }
	}

	else m_BS = NORMAL;

	return true;
}
