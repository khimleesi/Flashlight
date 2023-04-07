#pragma once

/*******************************************************************************************************************
	TexturePack.h, TexturePack.cpp
	Created by Kim Kane
	Last updated: 02/04/2018

	A simple class that loads in multiple textures at once and stores them into an array.
	Prime usage would be for terrain multi-textures.

	[Features]
	Nothing fancy.

	[Upcoming]
	Nothing.

	[Side Notes]
	Not much to say here!

*******************************************************************************************************************/

#include <map>
#include <string>
#include "Texture.h"
#include "Shader.h"

class TexturePack {

public:
	TexturePack(const std::string& baseTexture,
				const std::string& rTexture,
				const std::string& gTexture,
				const std::string& bTexture,
				const std::string& blendMap);

	TexturePack(const std::string& baseTextureNormal,
				const std::string& rTextureNormal,
				const std::string& gTextureNormal,
				const std::string& bTextureNormal);

public:
	~TexturePack();

public:
	void Bind() const;
	void Unbind() const;

public:
	Texture* GetBlendMap();

private:
	void Load(const std::string& baseTexture,
			  const std::string& rTexture,
			  const std::string& gTexture,
			  const std::string& bTexture,
			  const std::string& blendMap);

	void Load(const std::string& baseTextureNormal,
			  const std::string& rTextureNormal,
			  const std::string& gTextureNormal,
			  const std::string& bTextureNormal);

private:
	bool AddTexture(Shader::TextureUnit unit, const std::string& texture);

private:
	std::map<Shader::TextureUnit, Texture> m_textures;
};