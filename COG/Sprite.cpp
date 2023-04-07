#include "Sprite.h"
#include "Shader.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Sprite::Sprite(const std::string& tag, const std::string& texture, bool isAnimated, unsigned int rows)
	:	m_texture(texture, Shader::GetTextureUnit(Shader::TEXTURE_INTERFACE)),
		m_quad(tag),
		m_isAnimated(isAnimated)
{
	Load(rows);
}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
Sprite::~Sprite()
{

}


/*******************************************************************************************************************
	Default loading settings for all sprites - these can be adjusted to suit
*******************************************************************************************************************/
void Sprite::Load(unsigned int rows)
{
	m_quad.Create();
	m_texture.SetMirrored(true);

	if (m_isAnimated) { m_texture.SetRows(rows); }
}


/*******************************************************************************************************************
	A function that renders a sprite to the screen
*******************************************************************************************************************/
void Sprite::Render()
{
	m_texture.Bind();
		m_quad.Render();
	m_texture.Unbind();
}


/*******************************************************************************************************************
	A function that updates a sprite
*******************************************************************************************************************/
void Sprite::Update(float frame)
{
	//--- Only update the texture if it is animated
	if (m_isAnimated) { SetAnimationFrame(frame); }
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
Texture* Sprite::GetTexture()	{ return &m_texture; }
bool Sprite::IsAnimated() const { return m_isAnimated; }


/*******************************************************************************************************************
	Modifier methods
*******************************************************************************************************************/
void Sprite::SetAnimationFrame(float frame)						{ m_texture.SetIndex((int)frame); }
void Sprite::SetAnimated(bool isAnimated, unsigned int rows)	{ m_isAnimated = isAnimated; m_texture.SetRows(rows); }