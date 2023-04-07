#include "Texture.h"
#include "Log.h"
#include "ResourceManager.h"
#include "ScreenManager.h"

/*******************************************************************************************************************
	[Texture] Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Texture::Texture(const std::string& texture, int slot, bool hasTransparency, bool hasFakeLighting)
	:	m_tag(texture),
		m_data({ 0, slot, GL_TEXTURE_2D }),
		m_width(0),
		m_height(0),
		m_index(s_defaultIndex),
		m_rows(s_defaultRows),
		m_offset(0.0f),
		m_hasTransparency(hasTransparency),
		m_hasFakeLighting(hasFakeLighting),
		m_isMirrored(false)
{
	Load();
}


/*******************************************************************************************************************
	[Cubemap] Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Texture::Texture(const std::string& tag, const std::vector<std::string>& textures, int slot)
	:	m_tag(tag),
		m_data({ 0, slot, GL_TEXTURE_CUBE_MAP }),
		m_width(0),
		m_height(0),
		m_index(s_defaultIndex),
		m_rows(s_defaultRows),
		m_offset(0.0f),
		m_hasTransparency(false),
		m_hasFakeLighting(false),
		m_isMirrored(false)
{
	Load(textures);
}


/*******************************************************************************************************************
	[Render Target] Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Texture::Texture(int attachment, int width, int height, int slot, const std::string& tag)
	:	m_tag(tag),
		m_data({ 0, slot, GL_TEXTURE_2D }),
		m_width(width),
		m_height(height),
		m_index(s_defaultIndex),
		m_rows(s_defaultRows),
		m_offset(0.0f),
		m_hasTransparency(false),
		m_hasFakeLighting(false),
		m_isMirrored(false)
{
	Load(attachment);
}


/*******************************************************************************************************************
	Default Constructor
*******************************************************************************************************************/
Texture::Texture()
	:	m_tag(""),
		m_data({ 0, 0, GL_TEXTURE_2D }),
		m_width(0),
		m_height(0),
		m_index(s_defaultIndex),
		m_rows(s_defaultRows),
		m_offset(0.0f),
		m_hasTransparency(false),
		m_hasFakeLighting(false),
		m_isMirrored(false)
{

}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
Texture::~Texture()
{

}


/*******************************************************************************************************************
	Function that generates an OpenGL texture object for a render target; allowing us to save the scene to a texture
*******************************************************************************************************************/
bool Texture::Load(int attachment)
{
	if (m_tag.empty()) { return false; }
	
	//--- If the texture exists already, just set the ID to the previous texture ID and return
	if (Resource::Instance()->FindTexture(m_tag)) {
		m_data.ID = Resource::Instance()->GetTexture(m_tag);
		FL_LOG("[TEXTURE] Texture ID already exists for tag: ", m_tag.c_str(), LOG_RESOURCE); return false;
	}

	//--- Otherwise generate a new texture ID and add this texture to our resource cache
	GenerateTexture();
	Bind();

	//--- Distinguish what kind of texture to create, based on the attachment
	if (attachment == GL_COLOR_ATTACHMENT0) {
		FL_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	else if (attachment == GL_DEPTH_ATTACHMENT) {
		FL_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	}

	//--- Set the filters and create the frame buffer texture using the ID we just generated
	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	FL_GLCALL(glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_data.ID, 0));
	
	FL_LOG("[TEXTURE] Generated framebuffer texture: ", m_tag.c_str(), LOG_RESOURCE);

	//--- NOTE
	// We don't unbind the texture, as it will be created within an FBO object
	// Similar to how VBO/VAO's work
	//---

	return true;
}


/*******************************************************************************************************************
	Function that loads in a texture file and generates an OpenGL texture object for the texture
*******************************************************************************************************************/
bool Texture::Load()
{
	if (m_tag.empty()) { return false; }
	
	//--- If a texture with this tag is already in memory, use pre-existing data stored for the texture
	//--- And exit the function
	if (Resource::Instance()->FindTexture(m_tag)) {
		m_data.ID = Resource::Instance()->GetTexture(m_tag);
		FL_LOG("[TEXTURE] Texture ID already exists for tag: ", m_tag.c_str(), LOG_RESOURCE); return false;
	}

	std::string src = "Assets\\Textures\\" + m_tag;

	//--- Load the texture and store it into our texture data variable
	 SDL_Surface* textureData = IMG_Load(src.c_str());

	//--- If there was a problem loading discontinue and free the SDL surface before exiting out of the function (precautionary)
	if (!textureData) { 
		FL_LOG("[TEXTURE] Error loading texture file: ", src.c_str(), LOG_ERROR);
		SDL_FreeSurface(textureData); 
		return false;
	}

	//--- Otherwise, generate OpenGL texture object, set the filters for the texture and add this new texture
	//--- to the map of texture ID's, so we can bind it later
	GenerateTexture();

	FL_LOG("[TEXTURE] Texture filters being set...", FL_LOG_EMPTY, LOG_MESSAGE);

	//--- Similar to how a VAO works, bind the ID for use and then declare what filters we want set within this texture ID
	Bind();
	
	unsigned char* pixels	= (unsigned char*)textureData->pixels;
	unsigned int depth		= textureData->format->BytesPerPixel;
	unsigned int format		= ((depth == 4) ? GL_RGBA : GL_RGB);
	m_width					= textureData->w;
	m_height				= textureData->h;

	FL_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixels));

	//--- Make sure we generate the mipmaps after we create the texture and before we set the filters below.
	FL_GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

	//--- If anisotropic filtering is supported then use it, otherwise use LOD_BIAS
	if (Screen::Instance()->IsAnisotropySupported())
	{
		FL_GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, Screen::Instance()->GetAnisotropy()));
	}
	else {
		//--- Makes the texture detail level slightly higher (less blurred), even after mipmap generation.
		//--- This number shouldn't be too high, or we lose the performance gain of using mipmaps.
		FL_GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f));
	}

	//--- Free (delete) the SDL surface we created, as OpenGL now has the data
	SDL_FreeSurface(textureData);

	//--- Then unbind the texture ID, now the above data is stored in this ID
	Unbind();
	
	FL_LOG("[TEXTURE] Generated new texture: ", src.c_str(), LOG_RESOURCE);

	//--- NOTE
	// We need to make sure we generate the OpenGL texture ID after the file has been successfully loaded in
	// Otherwise, we generate an OpenGL ID, which never get's bound and becomes useless and takes up memory.
	//---

	return true;
}


/*******************************************************************************************************************
	Function that loads in a cube map file and generates an OpenGL texture object for the cube map
*******************************************************************************************************************/
bool Texture::Load(const std::vector<std::string>& textures)
{
	if (textures.empty()) { return false; }

	//--- If a texture with this tag is already in memory, use pre-existing data stored for the texture
	//--- And exit the function
	if (Resource::Instance()->FindTexture(m_tag)) {
		m_data.ID = Resource::Instance()->GetTexture(m_tag);
		FL_LOG("[TEXTURE] Cube map texture ID already exists for tag: ", m_tag.c_str(), LOG_RESOURCE); return false;
	}

	bool initialized			= false;
	SDL_Surface* textureData	= nullptr;

	for (unsigned int i = 0; i < textures.size(); i++) {

		std::string src = "Assets\\Textures\\Skybox\\" + textures[i] + ".png";

		//--- Load the texture and store it into our texture data variable
		textureData = IMG_Load(src.c_str());

		//--- If there was a problem loading discontinue and free the SDL surface before exiting out of the function (precautionary)
		if (!textureData) { FL_LOG("[TEXTURE] Error loading cube map file: ", src.c_str(), LOG_ERROR); SDL_FreeSurface(textureData); return false; }

		//--- If we haven't initialized a texture ID, generate just 1 texture ID for our cube map, and bind the cube map.
		//--- Proceed to generate the texture data for each cube map face.
		if (!initialized) {
				 
			GenerateTexture(); 
			Bind();
			initialized = true;
		}

		unsigned char* pixels	= (unsigned char*)textureData->pixels;
		unsigned int depth		= textureData->format->BytesPerPixel;
		unsigned int format		= ((depth == 4) ? GL_RGBA : GL_RGB);
		m_width					= textureData->w;
		m_height				= textureData->h;

		//--- Pass the image data we obtained from SDL surface to OpenGL
		FL_GLCALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixels));

		//--- Free (delete) the SDL surface we created, as OpenGL now has the data
		SDL_FreeSurface(textureData);
			
		FL_LOG("[TEXTURE] New cube map texture created: ", src.c_str(), LOG_RESOURCE);
	}

	//--- Only done once per cube map, not to be done per cube map face
	//--- No mipmap generation, as cube map never get's further from the camera. We want it to look high def.
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	
	//--- As we have no mipmaps, we set the max levels to 0 (it is 1000 by default)
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0));
	FL_GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0));

	//--- Unbind now OpenGL has all the texture data for our cube map
	Unbind();
	
	//--- NOTE
	// We need to make sure we generate the OpenGL texture ID after the file has been successfully loaded in
	// Otherwise, we generate an OpenGL ID, which never get's bound and becomes useless and takes up memory.
	//---

	return true;
}


/*******************************************************************************************************************
	Generate a texture object ID and insert the new ID into the static map of texture ID's
*******************************************************************************************************************/
void Texture::GenerateTexture()
{
	FL_GLCALL(glGenTextures(1, &m_data.ID));

	//--- Insert this texture ID into our resource cache, and assign it the tag as the key
	Resource::Instance()->AddTexture(m_tag, m_data.ID);
	
	FL_LOG("[TEXTURE] Generated OpenGL texture object: " + m_tag + ", OpenGL ID: ", m_data.ID, LOG_MEMORY);
}


/*******************************************************************************************************************
	Bind the texture ID stored within the texture map to the relevant texture type & make it the active texture
*******************************************************************************************************************/
void Texture::Bind() const
{	
	FL_GLCALL(glActiveTexture(m_data.slot));
	FL_GLCALL(glBindTexture(m_data.type, m_data.ID));

	//--- NOTE
	// As we are using multi-textures for the terrain plus diffuse/specular, etc. textures for every object
	// we need to make sure we activate textures at the correct texture slot every frame, for all objects.
	// You can choose to only do this once before drawing all objects, if you are sure you will only ever
	// use one texture slot for all your objects in the scene.
	// However, that wouldn't allow you to switch between texture slots depending on the objects being drawn,
	// which is why I have set it up in this generic way.
	//--- 
}


/*******************************************************************************************************************
	Unbinds the texture object ID to the relevant texture type & makes it disactive
*******************************************************************************************************************/
void Texture::Unbind() const
{
	FL_GLCALL(glBindTexture(m_data.type, 0));
}


/*******************************************************************************************************************
	Function that calculates the current texture offset and returns the value if any changes have occurred
*******************************************************************************************************************/
const glm::vec2& Texture::GetOffset() {

	//--- NOTE
	// This is only used for texture atlases. It allows textures to animate easily without updating any buffers
	// as the offset & texture rows are passed to the shader. I'm not currently using it in this game
	//---

	glm::vec2 offset = glm::vec2(0.0f);

	//--- Determine the row of where the texture is located
	offset.x = (float)(m_index % m_rows);

	//--- Determine the column of where the texture is located
	offset.y = (float)(m_index / m_rows);

	//--- Update the texture offset if a change has happened
	if (m_offset != offset) { m_offset = offset /= (float)m_rows; }

	return m_offset;
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
int Texture::GetWidth() const	{ return m_width; }
int Texture::GetHeight() const	{ return m_height; }
int Texture::GetRows() const	{ return m_rows; }

bool Texture::HasTransparency()	const	{ return m_hasTransparency; }
bool Texture::HasFakeLighting()	const	{ return m_hasFakeLighting; }
bool Texture::IsMirrored()	const 		{ return m_isMirrored; }

const Texture::OpenGLTextureData* Texture::GetData() const		{ return &m_data; }
const std::string& Texture::GetTag() const						{ return m_tag; }


/*******************************************************************************************************************
	Modifier methods
*******************************************************************************************************************/
void Texture::SetIndex(int index)					{ m_index = index; }
void Texture::SetRows(int rows)						{ m_rows = rows; }
void Texture::SetMirrored(bool isMirrored)			{ m_isMirrored = isMirrored; }
void Texture::SetFakeLighting(bool hasFakeLighting) { m_hasFakeLighting = hasFakeLighting; }
void Texture::SetTransparency(bool hasTransparency) { m_hasTransparency = hasTransparency; }


/*******************************************************************************************************************
	Static variables and functions
*******************************************************************************************************************/
unsigned int Texture::s_defaultIndex	= 0;
unsigned int Texture::s_defaultRows		= 1;

unsigned int Texture::GetDefaultRows() { return s_defaultRows; }