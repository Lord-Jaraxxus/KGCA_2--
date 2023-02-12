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
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr; // ���̴� ���ҽ� ��, ��¥ �ؽ�ó�� �갡 �ؽ�ó
	ID3D11Resource* m_pResource = nullptr;

}; 

