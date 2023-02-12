#include "K_Shape.h"

void K_Box::CreateVertexData()
{
	// 상단
  // 5    6
  // 1    2
  // 하단
  // 4    7
  // 0    3  
  // 앞면
	m_VertexList.resize(24);
	m_VertexList[0] = SimpleVertex(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(k_Vector(1.0f, 1.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[2] = SimpleVertex(k_Vector(1.0f, -1.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[3] = SimpleVertex(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = SimpleVertex(k_Vector(1.0f, 1.0f, 1.0f), k_Vector4D(0.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[6] = SimpleVertex(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[7] = SimpleVertex(k_Vector(1.0f, -1.0f, 1.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = SimpleVertex(k_Vector(1.0f, 1.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[9] = SimpleVertex(k_Vector(1.0f, 1.0f, 1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[10] = SimpleVertex(k_Vector(1.0f, -1.0f, 1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[11] = SimpleVertex(k_Vector(1.0f, -1.0f, -1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = SimpleVertex(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[13] = SimpleVertex(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[14] = SimpleVertex(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[15] = SimpleVertex(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector4D(1.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = SimpleVertex(k_Vector(-1.0f, 1.0f, 1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[17] = SimpleVertex(k_Vector(1.0f, 1.0f, 1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[18] = SimpleVertex(k_Vector(1.0f, 1.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[19] = SimpleVertex(k_Vector(-1.0f, 1.0f, -1.0f), k_Vector4D(1.0f, 0.5f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = SimpleVertex(k_Vector(-1.0f, -1.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[21] = SimpleVertex(k_Vector(1.0f, -1.0f, -1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
	m_VertexList[22] = SimpleVertex(k_Vector(1.0f, -1.0f, 1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 1.0f));
	m_VertexList[23] = SimpleVertex(k_Vector(-1.0f, -1.0f, 1.0f), k_Vector4D(0.0f, 1.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 1.0f));


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
	m_VertexList[0] = SimpleVertex(k_Vector(0.0f, 0.0f, 0.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(k_Vector(0.0f, 0.0f, 1.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));
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

	// X축, 붉은색
	m_VertexList[0] = SimpleVertex(k_Vector(0.0f, 0.0f, 0.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(k_Vector(1.0f, 0.0f, 0.0f), k_Vector4D(1.0f, 0.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

	// Y축, 초록색
	m_VertexList[2] = SimpleVertex(k_Vector(0.0f, 0.0f, 0.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[3] = SimpleVertex(k_Vector(0.0f, 1.0f, 0.0f), k_Vector4D(0.0f, 1.0f, 0.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

	// Z축, 푸른색
	m_VertexList[4] = SimpleVertex(k_Vector(0.0f, 0.0f, 0.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(k_Vector(0.0f, 0.0f, 1.0f), k_Vector4D(0.0f, 0.0f, 1.0f, 1.0f), k_Vector2D(1.0f, 0.0f));

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
