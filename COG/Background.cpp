#include "Background.h"
#include "InterfaceShader.h"
#include "Tools.h"
#include "FileManager.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Background::Background(const std::string& tag, const std::string& texture, const Transform& transform)
	:	Interface(tag + ".background", transform),
		m_sprite(m_tag, texture)
{

}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
Background::~Background()
{

}


/*******************************************************************************************************************
	A static function which creates a background instance from data read in from a file
*******************************************************************************************************************/
Background* Background::Create(const std::string& tag)
{
	File::Instance()->OpenForReading("Assets\\Files\\interfaceObjects.config");

	std::map<std::string, std::map<std::string, std::string>> data;

	File::Instance()->GetObjectData(tag, data);

	glm::vec2 position	= glm::vec2(StringToFloat(data[tag]["transform.x"]), StringToFloat(data[tag]["transform.y"]));
	glm::vec2 dimension	= glm::vec2(StringToFloat(data[tag]["width"]), StringToFloat(data[tag]["height"]));

	//--- NOTE
	// This function was made for ease of use/quickness and is not optimized at all, so please ignore (I know it's ugly!)
	// Later I plan on tidying up file handling and building a proper file parsing system (I never got round to this!)
	//---

	return new Background(data[tag]["tag"], data[tag]["sprite"], Transform(position, dimension));
}


/*******************************************************************************************************************
	A function that renders the button to the screen
*******************************************************************************************************************/
void Background::Render(Shader* shader)
{
	if (InterfaceShader* interfaceShader = Downcast<InterfaceShader>(shader)) {

		interfaceShader->SetInstanceData(&m_transform, m_sprite.GetTexture());

		m_sprite.Render();
	}
}


/*******************************************************************************************************************
	A function that updates the background
*******************************************************************************************************************/
void Background::Update()
{
	m_transform.Update();
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
Sprite* Background::GetSprite() { return &m_sprite; }