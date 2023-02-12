#pragma once
#include "K_Device.h"

class Sample : public K_Device
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // 쉐이더 리소스 뷰, 진짜 텍스처는 얘가 텍스처
	ID3D11Resource* m_pResource = nullptr;

}; 

