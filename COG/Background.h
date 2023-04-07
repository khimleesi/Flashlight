#pragma once
/*******************************************************************************************************************
	Background.h, Background.cpp
	Created by Kim Kane
	Last updated: 08/04/2018

	A derived class of base class Interface. Creates a 2D background image.
	
	[Features]

	[Upcoming]
	Nothing.

	[Side Notes]
	Nothing.

*******************************************************************************************************************/
#include "Interface.h"
#include "Sprite.h"

class Background : public Interface {

public:
	Background(const std::string& tag, const std::string& texture, const Transform& transform);
	virtual ~Background();

public:
	virtual void Render(Shader* shader) override;
	virtual void Update()				override;

public:
	static Background* Create(const std::string& tag);

public:
	Sprite* GetSprite();

private:
	Sprite m_sprite;
};