#include "SkyboxShader.h"
#include "ScreenManager.h"
#include "Camera.h"
#include "Log.h"
#include "UniformBlocks.h"

/*******************************************************************************************************************
	Default Constructor
*******************************************************************************************************************/
SkyboxShader::SkyboxShader(const std::string& vertex, const std::string& fragment, Camera* camera)
	:	Shader(vertex, fragment, camera),
		m_projection(1.0f)
{
	//--- Check we have a valid program
	if (m_shaderCount != NULL) {

		//--- Get all uniforms within the shader, when an instance of this shader is first created
		GetAllUniforms();

		//--- Set all unchanging data within this shader
		Bind(); SetPermanentAttributes();
	}
}


/*******************************************************************************************************************
	Default Destructor
*******************************************************************************************************************/
SkyboxShader::~SkyboxShader()
{

}


/*******************************************************************************************************************
	A function which get's all uniform locations within the shader
*******************************************************************************************************************/
void SkyboxShader::GetAllUniforms()
{
	//--- Get all uniform locations from the shader
	GetUniform("uniform_skybox_projection");
	GetUniform("uniform_skybox_texture");
	GetUniform("uniform_skybox_tintColor");
	GetUniform("uniform_skybox_tintBegin");
	GetUniform("uniform_skybox_tintEnd");
	GetUniform("uniform_skybox_applyTint");
}


/*******************************************************************************************************************
	A function that set's all the unchanging data within the shader
*******************************************************************************************************************/
void SkyboxShader::SetPermanentAttributes()
{
	using namespace shader_constants;

	SetInteger("uniform_skybox_texture", TEXTURE_SKYBOX);
	SetBool("uniform_skybox_applyTint", SKYBOX_TINTED);
	SetVector3f("uniform_skybox_tintColor", SKYBOX_TINT_COLOR);
	SetFloat("uniform_skybox_tintBegin", SKYBOX_TINT_BEGIN);
	SetFloat("uniform_skybox_tintEnd", SKYBOX_TINT_END);
}


/*******************************************************************************************************************
	A function that set's all the per instance skybox data within the shader (specific to this game)
*******************************************************************************************************************/
void SkyboxShader::SetInstanceData()
{
	//--- Check we have a valid program
	if (m_shaderCount != NULL) {
		SetMatrixData();
	}
}


/*******************************************************************************************************************
	A function that set's the projection matrix of the skybox (should only be done when a change happens)
*******************************************************************************************************************/
bool SkyboxShader::SetMatrixData()
{
	if (!m_camera) { return false; }

	//--- Get the projection matrix and multiply with the camera's rotation matrix
	glm::mat4 projection = Screen::Instance()->GetProjectionMatrix() * m_camera->GetRotationMatrix();

	//--- Only update the projection if the camera rotates
	if (m_projection != projection) {
			
		m_projection = projection;

		SetMatrix("uniform_skybox_projection", m_projection);
	}

	return true;
}