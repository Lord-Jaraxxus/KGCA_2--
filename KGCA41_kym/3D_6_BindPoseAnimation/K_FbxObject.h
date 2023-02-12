#pragma once
#include "fbxsdk.h"
#include "K_BaseObject.h"

struct K_AnimeTrack 
{
	UINT iFrame;
	K_Matrix matAnime;
	K_Matrix matSelfAnime;

	TBASIS_EX::TVector3     s; 
	TBASIS_EX::TQuaternion  r; 
	TBASIS_EX::TVector3     t; 
};

struct K_AnimeScene 
{
	UINT iStartFrame;
	UINT iEndFrame;
	float fTickPerFrame;
	float fFrameSpeed = 30.0f;
	FbxTime::EMode TimeMode;
};

struct K_AnimeCurrentInfo 
{
	float	fAnimeCurrentFrame = 0;
	float	fAnimeSpeed = 1.0f;
	float	fAnimeInverse = 1.0f;
};

class K_FbxObject : public K_BaseObject
{
public:
	const char* m_pObjectName;
	FbxNode* m_pFbxNode = nullptr;
	FbxNode* m_pFbxParentNode = nullptr;
	K_FbxObject* m_pParent = nullptr;
	std::vector<K_FbxObject*> m_pChilds;
	void  SetParent(K_FbxObject* pParent)
	{
		pParent->m_pChilds.push_back(this);
		m_pParent = pParent;
	}

public:
	std::vector<ID3D11Buffer*> m_pVertexBufferList;
	std::vector<K_Texture*>	m_pTextureList;
	std::vector< std::vector<PNCT_VERTEX>>  m_vVertexDataListList;
	std::vector< W_STR >   m_vTextureNameList;

public:
	K_AnimeScene m_AnimeScene;
	float       m_fAnimeFrame = 0;
	float       m_fAnimeInverse = 1.0f;
	float       m_fAnimeSpeed = 1.0f;
	std::vector<K_AnimeTrack>   m_AnimeTracks;
	FbxAMatrix  m_fbxLocalMatrix;

public:
	UINT			   m_iObjectBone;
	TBASIS_EX::TMatrix m_matAnim;
	TBASIS_EX::TMatrix m_matControl;

public:
	void  CreateVertexData(){}
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer() { return S_OK; }; // override
	bool LoadTexture(std::wstring filename);
	bool PostRender();
	bool Release();

	K_Matrix InterPlate(float fTime, K_AnimeScene AnimeScene);
};

