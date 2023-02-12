#pragma once
#include "K_Std.h"
#include "fbxsdk.h"
#include "K_FbxObject.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")


class K_FBXLoader
{
public:
	ID3D11DeviceContext* m_pContext = nullptr;

	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	FbxNode* m_pRootNode;

	std::vector< K_FbxObject* > m_pObjectList;
	std::vector< K_FbxObject* > m_pDrawObjList;
	std::vector< FbxMesh* > m_pFbxMeshList;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	bool Load(C_STR filename);
	void PreProcess(FbxNode* pFbxNode);
	void ParseMesh(FbxMesh* pFbxMesh, K_FbxObject* pObject);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh, FbxLayerElementVertexColor* VertexColorSet, int vertexIndex, int colorIndex);
	FbxVector4 ReadNormal(FbxMesh* pFbxMesh, FbxLayerElementNormal* VertexNormalSet, int vertexIndex, int normalIndex);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* MaterialSet);
	void LoadAnimation(K_FbxObject* pObject);
	K_Matrix ConvertMatrix(FbxMatrix fbxMatrix);
};