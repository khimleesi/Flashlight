#pragma once
/*******************************************************************************************************************
	Sprite.h, Sprite.cpp
	Created by Kim Kane
	Last updated: 12/04/2018

	A sprite is a 2D quad with a texture.
	
	[Features]
	Supports animations using texture atlases.

	[Upcoming]
	Nothing.

	[Side Notes]
	Nothing at present.

*******************************************************************************************************************/
#include <string>
#include "Texture.h"
#include "Quad.h"

class Sprite {

public:
	Sprite(	const std::string& tag, const std::string& texture,
			bool isAnimated = false, unsigned int rows = Texture::GetDefaultRows());

public:
	~Sprite();

public:
	void Render();
	void Update(float frame);

public:
	Texture* GetTexture();
	bool IsAnimated() const;

public:
	void SetAnimated(bool isAnimated, unsigned int rows);
	void SetAnimationFrame(float frame);

private:
	void Load(unsigned int rows);

private:
	Texture		m_texture;
	Quad		m_quad;

private:
	bool		m_isAnimated;
};