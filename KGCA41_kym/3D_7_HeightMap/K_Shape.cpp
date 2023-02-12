#include "K_Shape.h"

void K_Box::CreateVertexData()
{
	// »ó´Ü
  // 5    6
  // 1    2
  // ÇÏ´Ü
  // 4    7
  // 0    3  
  // ¾Õ¸é
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(k_Vector(1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, -1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(k_Vector(1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));


	m_InitVertexList = m_VertexList;
}

void K_Box::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
}

bool K_Box::Frame()
{
	m_vPos.x = m_matWorld._41;
	m_vPos.y = m_matWorld._42;
	m_vPos.z = m_matWorld._43;
	return true;
}

bool K_Box::Render()
{
	PreRender();
	PostRender();
	return true;
}

void K_ShapeLine::CreateVertexData()
{
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_InitVertexList = m_VertexList;
}

void K_ShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList[0] = 0; 	
	m_IndexList[1] = 1; 	
}

bool K_ShapeLine::Frame()
{
	return true;
}

bool K_ShapeLine::Render()
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	PreRender();
	PostRender();
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void K_ShapeDirectionLine::CreateVertexData()
{
	m_VertexList.resize(6);

	// XÃà, ºÓÀº»ö
	m_VertexList[0] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(k_Vector(1.0f, 0.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

	// YÃà, ÃÊ·Ï»ö
	m_VertexList[2] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[3] = PNCT_VERTEX(k_Vector(0.0f, 1.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

	// ZÃà, Çª¸¥»ö
	m_VertexList[4] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 0.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(k_Vector(0.0f, 0.0f, 1.0f), k_Vector(0.0f, 0.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

	m_InitVertexList = m_VertexList;
}

void K_ShapeDirectionLine::CreateIndexData()
{
	m_IndexList.resize(6);

	m_IndexList[0] = 0;
	m_IndexList[1] = 1;

	m_IndexList[2] = 2;
	m_IndexList[3] = 3;

	m_IndexList[4] = 4;
	m_IndexList[5] = 5;
}
