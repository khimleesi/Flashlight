#include "TexturePack.h"
#include "Log.h"
#include "Tools.h"

/*******************************************************************************************************************
	[Textures] Constructor called when object is created
*******************************************************************************************************************/
TexturePack::TexturePack(const std::string& baseTexture,
						 const std::string& rTexture,
						 const std::string& gTexture,
						 const std::string& bTexture,
						 const std::string& blendMap)
{
	Load(baseTexture, rTexture, gTexture, bTexture, blendMap);
}


/*******************************************************************************************************************
	[Normal Maps] Constructor called when object is created
*******************************************************************************************************************/
TexturePack::TexturePack(const std::string& baseTextureNormal,
						 const std::string& rTextureNormal,
						 const std::string& gTextureNormal,
						 const std::string& bTextureNormal)
{
	Load(baseTextureNormal, rTextureNormal, gTextureNormal, bTextureNormal);
}


/*******************************************************************************************************************
	Function that loads all startup procedures
*******************************************************************************************************************/
void TexturePack::Load(	const std::string& baseTexture,
						const std::string& rTexture,
						const std::string& gTexture,
						const std::string& bTexture,
						const std::string& blendMap)
{
	//--- Create the textures and store them into our m_textures map so we can bind them later
	AddTexture(Shader::TEXTURE_BASE, baseTexture);
	AddTexture(Shader::TEXTURE_RED, rTexture);
	AddTexture(Shader::TEXTURE_GREEN, gTexture);
	AddTexture(Shader::TEXTURE_BLUE, bTexture);
	AddTexture(Shader::TEXTURE_BLENDMAP, blendMap);
}


/*******************************************************************************************************************
	Function that loads all startup procedures
*******************************************************************************************************************/
void TexturePack::Load( const std::string& baseTextureNormal,
						const std::string& rTextureNormal,
						const std::string& gTextureNormal,
						const std::string& bTextureNormal)
{
	//--- Create the textures and store them into our m_textures map so we can bind them later
	AddTexture(Shader::TEXTURE_BASE_NORMAL, baseTextureNormal);
	AddTexture(Shader::TEXTURE_RED_NORMAL, rTextureNormal);
	AddTexture(Shader::TEXTURE_GREEN_NORMAL, gTextureNormal);
	AddTexture(Shader::TEXTURE_BLUE_NORMAL, bTextureNormal);
}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
TexturePack::~TexturePack()
{

}


/*******************************************************************************************************************
	Function that checks if a texture string is empty and adds it to the m_textures map if not
*******************************************************************************************************************/
bool TexturePack::AddTexture(Shader::TextureUnit unit, const std::string& texture)
{
	if (texture.empty())	{ return false; }
	else					{ m_textures.try_emplace(unit, texture, Shader::GetTextureUnit(unit)); }

	return true;
}


/*******************************************************************************************************************
	Function that binds all the textures within the m_textures array
*******************************************************************************************************************/
void TexturePack::Bind() const
{
	for (auto& texture : m_textures) { GetValue(texture).Bind(); }
}


/*******************************************************************************************************************
	Function that unbinds all the textures within the m_textures array
*******************************************************************************************************************/
void TexturePack::Unbind() const
{
	for (auto& texture : m_textures) { GetValue(texture).Unbind(); }
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
Texture* TexturePack::GetBlendMap() { return &m_textures[Shader::TEXTURE_BLENDMAP]; }