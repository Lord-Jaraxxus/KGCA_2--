#pragma once
#include "K_Texture.h"

class K_TextureManager : public K_Singleton<K_TextureManager>
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();


private:
	friend class K_Singleton<K_TextureManager>;
	std::map<std::wstring, K_Texture*> m_List;
	UINT m_iIndex;
private:
	K_TextureManager();
	~K_TextureManager();

public:
	K_Texture*	Load(std::wstring name);
	K_Texture*	Find(std::wstring name);
	W_STR		GetSplitName(std::wstring fullpath);
	W_STR		GetSplitName(std::string fullpath);

public:
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	ID3D11Device* m_pd3dDevice = nullptr; // ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // ���ؽ�Ʈ, ������ �� ��Ʈ���ϴ� �뵵
};

#define I_Tex K_TextureManager::GetInstance()