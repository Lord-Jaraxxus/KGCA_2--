#pragma once
#include "K_2dObject.h"

class K_BackGround : public K_2dObject
{
public:
	virtual bool	Frame() override;
	virtual void	SetPosition(k_Vector2D pos) override;
};

class K_Npc : public K_2dObject
{
public:
	virtual bool	Frame() override;
	virtual void	SetPosition(k_Vector2D pos) override;
	int				m_iMouseFlag = 0;
	int				m_iDragFlag = 0;
	float			m_fZoom = 1.0f;
	k_Vector2D		m_vMousePos;
};

class K_Player : public K_2dObject
{
	virtual bool	Frame() override;
};

class K_Button : public K_2dObject
{
public:
	virtual bool	Frame() override;
	virtual void	SetPosition(k_Vector2D pos) override;
	virtual void	UpdateVertexBuffer() override;

public:
	int				m_iMouseFlag = 0;
	bool			m_bButtonPushed = false;
	float			m_fZoom = 1.2f;
	float			m_fSize = 1.0f;
	float			m_fColor = 1.0f;
};

class K_Map : public K_2dObject 
{
};

class K_MapObject : public K_2dObject 
{
public:
	virtual bool	Frame() override;
	virtual void	UpdateVertexBuffer() override;
	virtual void	SetPosition(k_Vector2D pos) override;

public:
	int				m_iMouseFlag = 0;
	bool			m_bButtonPushed = false;
	float			m_fZoom = 1.0f;
	float			m_fColor = 0.4f;
	int				m_iStage = 0;
	bool			m_bStageDone = false;
};

class K_Card : public K_2dObject 
{
public:
	virtual bool	Frame() override;
	virtual void	SetPosition(k_Vector2D pos) override;

public:
	k_Vector2D		m_vMousePos;
	k_Vector2D		m_vPosOrigin;
	int				m_iMouseFlag = 0;
	int				m_iDragFlag = 0;
	float			m_fZoom = 1.0f;
	bool			m_bUsed = false;
	bool			m_bAvailable = true;

public:
	int				m_iCardNum;
	int				m_iCardMana = 1;
};

class K_Character : public K_2dObject
{
public:
	virtual void	SetPosition(k_Vector2D pos) override;
	void			SetDrawPos();
	k_Vector2D		m_vPosOrgin;
	std::vector<K_Texture*>	m_pCharacterTextureList;
};

class K_EnemyIntent : public K_2dObject // K_Effect로 나중에 이름 바꿔야하는디
{
public:
	std::vector<K_Texture*>	m_pEnemyIntentTextureList;

public:
	bool	m_bEffectOn = false;
	float	m_fEffectTimer = 0.0f;
};

