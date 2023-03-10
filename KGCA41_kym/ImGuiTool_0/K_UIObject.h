#pragma once
#include "K_BaseObject.h"
#include "K_Input.h"
#include <fstream>

enum UIType 
{
	IMAGE,
	BUTTON,
	SPRITE,
}; 

struct CI // Cut Information
{
	ImVec2 pxWH; // pixel Width Height

	ImVec2 uvTL; // UV Top & Left
	ImVec2 uvBR; // UV Bottom & Right

	std::wstring tn; // Texture name
	std::wstring sn; // Shader name

	K_Texture* tc; // Texture class

	int cn;	// cut number
};

class K_UIObject : public K_BaseObject
{
public:
	virtual bool	Frame() override;
	virtual bool	Release() override;

public:
	virtual bool	SetUV(ImVec2 uvTL, ImVec2 uvBR);
	virtual bool	SetPosition(ImVec2 orginPos, ImVec2 widthHeight, float depth);
	virtual bool	SetAlpha(float alpha);
	static ImVec2	PtoN(ImVec2 pxWH); // Pixel to NDC, pixle Width Height
	static ImVec2	PtoN_Pos(ImVec2 pxWH);

	virtual bool	Save(std::ofstream& outfile);
	virtual bool	Load();

	virtual bool	AddCut(ImVec2 pxWH, ImVec2 uvTL, ImVec2 uvBR, std::wstring tn, std::wstring sn);
	virtual bool	AddCut(CI newCut);

	virtual void	Drag();

public:
	UIType			m_Type;
	int				m_ID;
	std::wstring	m_szObjName;

	ImVec2 m_OrginPos;
	ImVec2 m_WidthHeight; // �ϴ� NDC
	float  m_fDepth = 0.0f;
	float  m_fAlpha = 1.0f;

	// ����� ���̽� ������Ʈ�� �̹� ����
	//std::wstring	m_szTextureName;
	//std::wstring	m_szShaderName;

	std::vector<CI*> m_pCutInfoList;
	int m_iCurrentCutNum = 0;

	bool m_bDraggable = true; // ���߿� ���� �巡�װ��� ��ȣ�ۿ� �����ɲ��ϰ� �⺻ false�� �ؾ��ҵ�..
	bool m_bIsClicked = false;
};

