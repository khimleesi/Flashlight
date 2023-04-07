#include "FrameBuffer.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
FrameBuffer::FrameBuffer()
	:	m_frameBufferObject(0)
{
	GenerateBufferObject();
}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
FrameBuffer::~FrameBuffer()
{
	FL_GLCALL(glDeleteFramebuffers(1, &m_frameBufferObject));

	FL_LOG("[FRAME BUFFER] Frame buffer object destroyed: ", m_frameBufferObject, LOG_MEMORY);
}


/*******************************************************************************************************************
	Generate the buffer objects ID
*******************************************************************************************************************/
void FrameBuffer::GenerateBufferObject()
{
	FL_GLCALL(glGenFramebuffers(1, &m_frameBufferObject));

	FL_LOG("[FRAME BUFFER] Frame buffer object created: ", m_frameBufferObject, LOG_MEMORY);
}


/*******************************************************************************************************************
	Binds the frame buffer object ID & makes it the active buffer object
*******************************************************************************************************************/
void FrameBuffer::Bind() const
{
	FL_GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject));
}


/*******************************************************************************************************************
	Unbinds the frame buffer object ID & makes it disactive
*******************************************************************************************************************/
void FrameBuffer::Unbind() const
{
	FL_GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


/*******************************************************************************************************************
	Function that sets the attachment of the FBO
*******************************************************************************************************************/
void FrameBuffer::SetAttachment(int attachment)
{
	FL_GLCALL(glDrawBuffer(attachment));
}