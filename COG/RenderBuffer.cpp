#include "RenderBuffer.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
RenderBuffer::RenderBuffer()
	:	m_renderBufferObject(0)
{
	
}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
RenderBuffer::~RenderBuffer()
{
	FL_GLCALL(glDeleteRenderbuffers(1, &m_renderBufferObject));

	FL_LOG("[RENDER BUFFER] Render buffer object destroyed: ", m_renderBufferObject, LOG_MEMORY);
}


/*******************************************************************************************************************
	Binds the render buffer object ID & makes it the active buffer
*******************************************************************************************************************/
void RenderBuffer::Bind() const
{
	FL_GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject));
}


/*******************************************************************************************************************
	Unbinds the render buffer object ID & makes it disactive
*******************************************************************************************************************/
void RenderBuffer::Unbind() const
{
	FL_GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}


/*******************************************************************************************************************
	Generates a new RBO and creates the necessary storage for use within an FBO
*******************************************************************************************************************/
void RenderBuffer::CreateStorage(int width, int height)
{
	GenerateBufferObject();
	Bind();
	FL_GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	FL_GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject));
}


/*******************************************************************************************************************
	Generate the buffer objects ID
*******************************************************************************************************************/
void RenderBuffer::GenerateBufferObject()
{
	FL_GLCALL(glGenRenderbuffers(1, &m_renderBufferObject));

	FL_LOG("[RENDER BUFFER] Render buffer object created: ", m_renderBufferObject, LOG_MEMORY);
}