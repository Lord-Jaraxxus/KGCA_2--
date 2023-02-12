#pragma once
#include "K_Std.h"
#include "fbxsdk.h"
#include "K_FbxObject.h"
#include "K_FbxObjectSkinning.h"
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

	std::vector< K_FbxObjectSkinning* > m_pObjectListS;
	std::vector< K_FbxObjectSkinning* > m_pDrawObjListS;
	std::map<FbxNode*, K_FbxObjectSkinning* > m_pObjectMap;
	std::map<FbxNode*, UINT > m_pObjectIDMap;

public:
	K_AnimeScene  m_AnimeScene;
	float		  m_fAnimeFrame = 0;
	float		  m_fAnimeInverse = 1.0f;
	float		  m_fAnimeSpeed = 1.0f;

	VS_CONSTANT_BONE_BUFFER  m_cbDataBone;
	ID3D11Buffer* m_pConstantBufferBone;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool Load(C_STR filename);
	void PreProcess(FbxNode* pFbxNode);
	void PreProcessS(FbxNode* pFbxNode);
	void ParseMesh(FbxMesh* pFbxMesh, K_FbxObject* pObject);
	void ParseMeshS(FbxMesh* pFbxMesh, K_FbxObjectSkinning* pObject);
	FbxVector2 ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxColor ReadColor(FbxMesh* pFbxMesh, FbxLayerElementVertexColor* VertexColorSet, int vertexIndex, int colorIndex);
	FbxVector4 ReadNormal(FbxMesh* pFbxMesh, FbxLayerElementNormal* VertexNormalSet, int vertexIndex, int normalIndex);
	int GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* MaterialSet);

	void InitAnimation();
	void LoadAnimation(K_FbxObject* pObject);
	void LoadAnimationS(FbxLongLong t, FbxTime time);
	K_Matrix ConvertMatrix(FbxMatrix fbxMatrix);
	HRESULT	CreateConstantBuffer(ID3D11Device* pDevice);
	bool UpdateFrame(ID3D11DeviceContext* pContext);
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, K_FbxObjectSkinning* pObject);
};