#include "2dObjects.h"
#include "K_Input.h"

bool K_BackGround::Frame()
{
	UpdateVertexBuffer();
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_BackGround::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;

	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	m_vDrawSize.x = 2.0f;
	m_vDrawSize.y = 2.0f;
 
	UpdateVertexBuffer();
}

bool K_Npc::Frame()
{
	//k_Vector2D vPos = m_vPos;
	//vPos.y += 0.2f * g_fSecondPerFrame * 100.0f;
	//SetPosition(vPos);

	k_Vector2D vMousePosMove;
	vMousePosMove.x = I_Input.m_ptPos.x - m_vMousePos.x;
	vMousePosMove.y = I_Input.m_ptPos.y - m_vMousePos.y;

	m_vMousePos.x = I_Input.m_ptPos.x;
	m_vMousePos.y = I_Input.m_ptPos.y;
	float mouseNdcX = (m_vMousePos.x / g_rtClient.right) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((m_vMousePos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	if (m_VertexList[0].p.x <= mouseNdcX && m_VertexList[5].p.x >= mouseNdcX &&
		m_VertexList[0].p.y >= mouseNdcY && m_VertexList[5].p.y <= mouseNdcY)
	{
		m_iMouseFlag = 1;

		if (I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)	m_iDragFlag = 1;
		else m_iDragFlag = 0;
	}
	else m_iMouseFlag = 0;

	if (m_iMouseFlag == 1) m_fZoom = 1.2f;
	else m_fZoom = 1.0f;

	if (m_iDragFlag == 1)
	{
		m_vPos += vMousePosMove;
	}

	SetPosition(m_vPos);
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

void K_Npc::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	//m_vDrawPos.x = m_vDrawPos.x - m_vDrawSize.x / 2;
	//m_vDrawPos.y = m_vDrawPos.y + m_vDrawSize.y / 2;

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = m_rtInit.z / g_rtClient.right * m_fZoom;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom * m_fZoom;

	// 클라이언트 크기 바꾸면 찌그러지도록
	//m_vDrawSize.x = m_rtInit.z / m_ptImageSize.x;
	//m_vDrawSize.y = m_rtInit.w / m_ptImageSize.y;

	UpdateVertexBuffer();
}

bool K_Player::Frame()
{
	k_Vector2D vPos = m_vPos;
	if (I_Input.GetKey('W')) vPos.y += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('A')) vPos.x += -1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('S')) vPos.y += 1.0f * g_fSecondPerFrame * 100.0f;
	if (I_Input.GetKey('D')) vPos.x += 1.0f * g_fSecondPerFrame * 100.0f;
	SetPosition(vPos);

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

bool K_Button::Frame()
{
	k_Vector2D vMousePos;
	vMousePos.x = I_Input.m_ptPos.x;
	vMousePos.y = I_Input.m_ptPos.y;

	float mouseNdcX = (vMousePos.x / 1585.0f) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((vMousePos.y / 860.0f) * 2.0f - 1.0f); // 마우스 좌표가 좀 이상해서 일단 고정값으로 넣음

	if (m_rtCollision.x <= mouseNdcX &&  m_rtCollision.z >= mouseNdcX &&
		m_rtCollision.y >= mouseNdcY &&  m_rtCollision.w <= mouseNdcY)
	{
		m_iMouseFlag = 1;

		if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH) 
			m_bButtonPushed = true;
	}
	else m_iMouseFlag = 0;

	if (m_iMouseFlag == 1) m_fZoom = 1.5f;
	else m_fZoom = 1.2f;

	SetPosition(m_vPos);
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_Button::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = m_rtInit.z / g_rtClient.right * m_fSize * m_fZoom;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom * m_fSize * m_fZoom;

	// 중앙 기준으로 뿌리도록
	m_vDrawPos.x -= m_vDrawSize.x / 2.0f;
	m_vDrawPos.y += m_vDrawSize.x / 2.0f;

	UpdateVertexBuffer();

	m_rtCollision = { m_VertexList[0].p.x, m_VertexList[0].p.y, m_VertexList[5].p.x, m_VertexList[5].p.y };
}

void K_Button::UpdateVertexBuffer()
{
	m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
	m_VertexList[0].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[0].t = { m_rtUV.x, m_rtUV.y };

	m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.0f };
	m_VertexList[1].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[1].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y };

	m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[2].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[2].t = { m_rtUV.x, m_rtUV.y + m_rtUV.w };

	m_VertexList[3].p = m_VertexList[2].p;
	m_VertexList[3].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[3].t = m_VertexList[2].t;

	m_VertexList[4].p = m_VertexList[1].p;
	m_VertexList[4].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[4].t = m_VertexList[1].t;

	m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[5].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[5].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y + m_rtUV.w };
}

bool K_MapObject::Frame()
{
	k_Vector2D vMousePos;
	vMousePos.x = I_Input.m_ptPos.x;
	vMousePos.y = I_Input.m_ptPos.y;

	float mouseNdcX = (vMousePos.x / 1585.0f) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((vMousePos.y / 860.0f) * 2.0f - 1.0f); // 마우스 좌표가 좀 이상해서 일단 고정값으로 넣음

	if (m_rtCollision.x <= mouseNdcX && m_rtCollision.z >= mouseNdcX &&
		m_rtCollision.y >= mouseNdcY && m_rtCollision.w <= mouseNdcY)
	{
		m_iMouseFlag = 1;

		if (I_Input.GetKey(VK_LBUTTON) == KEY_HOLD) m_bButtonPushed = true;
		else m_bButtonPushed = false;
	}
	else m_iMouseFlag = 0;

	if (m_iMouseFlag == 1) 
	{ 
		m_fZoom = 1.2f;
		m_fColor = 0.0f;
	}
	else 
	{ 
		m_fZoom = 1.0f; 
		m_fColor = 0.4f;
	}

	if (m_bStageDone == true) 
	{
		m_fZoom = 1.2f;
		m_fColor = 0.0f;
	}

	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return true;
}

void K_MapObject::UpdateVertexBuffer()
{
	m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
	m_VertexList[0].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[0].t = { m_rtUV.x, m_rtUV.y };

	m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.0f };
	m_VertexList[1].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[1].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y };

	m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[2].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[2].t = { m_rtUV.x, m_rtUV.y + m_rtUV.w };

	m_VertexList[3].p = m_VertexList[2].p;
	m_VertexList[3].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[3].t = m_VertexList[2].t;

	m_VertexList[4].p = m_VertexList[1].p;
	m_VertexList[4].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[4].t = m_VertexList[1].t;

	m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
	m_VertexList[5].c = { m_fColor, m_fColor, m_fColor, 0.0f };
	m_VertexList[5].t = { m_rtUV.x + m_rtUV.z, m_rtUV.y + m_rtUV.w };
}

void K_MapObject::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;
	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = m_rtInit.z / g_rtClient.right * 2.0f * m_fZoom;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom * 2.0f * m_fZoom;

	// 중앙 기준으로 뿌리도록
	m_vDrawPos.x -= m_vDrawSize.x / 2.0f;
	m_vDrawPos.y += m_vDrawSize.y / 2.0f;

	UpdateVertexBuffer();

	m_rtCollision = { m_VertexList[0].p.x, m_VertexList[0].p.y, m_VertexList[5].p.x, m_VertexList[5].p.y };
}

bool K_Card::Frame()
{
	k_Vector2D vMousePosMove;
	vMousePosMove.x = I_Input.m_ptPos.x - m_vMousePos.x;
	vMousePosMove.y = I_Input.m_ptPos.y - m_vMousePos.y;

	m_vMousePos.x = I_Input.m_ptPos.x;
	m_vMousePos.y = I_Input.m_ptPos.y;
	float mouseNdcX = (m_vMousePos.x / 1585.0f) * 2.0f - 1.0f; // 마우스 x좌표를 NDC좌표계로 변환
	float mouseNdcY = -((m_vMousePos.y / 860.0f) * 2.0f - 1.0f);

	if (m_rtCollision.x <= mouseNdcX && m_rtCollision.z >= mouseNdcX &&
		m_rtCollision.y >= mouseNdcY && m_rtCollision.w <= mouseNdcY)
	{
		m_iMouseFlag = 1;

		if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH)	m_iDragFlag = 1;
		else if (I_Input.GetKey(VK_LBUTTON) == KEY_UP) m_iDragFlag = 0;
	}
	else m_iMouseFlag = 0;

	if (m_iMouseFlag == 1) m_fZoom = 1.2f;
	else m_fZoom = 1.0f;

	if (m_iDragFlag == 1)
	{
		m_vPos += vMousePosMove;
	}
	else 
	{
		m_vPos = m_vPosOrigin;
	}

	// 일정 높이 이상으로 카드를 끌어올리면 사용한 판정
	if (m_vPos.y < 250.0f) { 
		m_bUsed = true; }

	SetPosition(m_vPos);
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return true;
}

void K_Card::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;
	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;
	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	m_vDrawSize.x = m_rtInit.z / g_rtClient.right * 1.5f * m_fZoom;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom * 1.5f * m_fZoom;

	// 중앙 기준으로 뿌리도록
	m_vDrawPos.x -= m_vDrawSize.x / 2.0f;
	m_vDrawPos.y += m_vDrawSize.y / 2.0f;

	UpdateVertexBuffer();

	m_rtCollision = { m_vDrawPos.x, m_vDrawPos.y, 
						m_vDrawPos.x + m_vDrawSize.x / m_fZoom, m_vDrawPos.y - m_vDrawSize.y / m_fZoom };
}

void K_Character::SetPosition(k_Vector2D pos)
{
	m_vPos = pos;

	pos.x -= m_vCameraPos.x;
	pos.y -= m_vCameraPos.y;

	m_vDrawPos.x = (pos.x / g_rtClient.right) * 2.0f - 1.0f;
	m_vDrawPos.y = -((pos.y / g_rtClient.bottom) * 2.0f - 1.0f);

	// 클라이언트 크기 바꿔도 원본 이쁘게 유지, 해상도가 몇이던 간에 항상 일정한 픽셀만큼 잡아먹음
	m_vDrawSize.x = m_rtInit.z / g_rtClient.right * 2.0f;
	m_vDrawSize.y = m_rtInit.w / g_rtClient.bottom * 2.0f;


	UpdateVertexBuffer();

	m_rtCollision = { m_VertexList[0].p.x, m_VertexList[0].p.y, m_VertexList[5].p.x, m_VertexList[5].p.y };
}

void K_Character::SetDrawPos()
{
	// 좌하단 점을 기준으로 뿌리도록
	m_vDrawPos.y += m_vDrawSize.y;
	UpdateVertexBuffer();

	m_rtCollision = { m_VertexList[0].p.x, m_VertexList[0].p.y, m_VertexList[5].p.x, m_VertexList[5].p.y };
}
