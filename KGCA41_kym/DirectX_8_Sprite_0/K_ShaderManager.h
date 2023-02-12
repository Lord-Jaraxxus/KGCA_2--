#pragma once
#include "K_Shader.h"

class K_ShaderManager : public K_Singleton<K_ShaderManager>
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();


private:
	friend class K_Singleton<K_ShaderManager>;
	std::map<std::wstring, K_Shader*> m_List;
	UINT m_iIndex;
private:
	K_ShaderManager();
	~K_ShaderManager();

public:
	K_Shader* Load(std::wstring name);

public:
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	ID3D11Device* m_pd3dDevice = nullptr; // 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr; // 콘텍스트, 생성된 걸 컨트롤하는 용도
};

#define I_Shader K_ShaderManager::GetInstance()