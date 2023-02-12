#pragma once
#include "K_BaseObject.h"

class K_Object3D : public K_BaseObject
{
public:
	K_Object3D() {};
	virtual ~K_Object3D() {};
};

class K_Shape : public K_Object3D
{
public:
	K_Shape() {};
	virtual ~K_Shape() {};
};

class K_ShapeLine : public K_Shape 
{
public:
	virtual void CreateVertexData() override;
	virtual void CreateIndexData() override;
	virtual bool Frame() override;
	virtual bool Render() override;

public:
	K_ShapeLine() {};
	virtual ~K_ShapeLine() {};
};

class K_ShapeDirectionLine : public K_ShapeLine
{
public:
	virtual void CreateVertexData() override;
	virtual void CreateIndexData() override;
public:
	K_ShapeDirectionLine() {};
	virtual ~K_ShapeDirectionLine() {};
};

class K_Box : public K_Object3D
{
public:
	virtual void		CreateVertexData() override;
	virtual void		CreateIndexData() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	K_Box() {};
	virtual ~K_Box() {};
};