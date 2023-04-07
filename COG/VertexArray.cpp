#include "VertexArray.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
VertexArray::VertexArray()
	:	m_arrayObject(0)
{
	GenerateArrayObject();
}


/*******************************************************************************************************************
	Default destructor
*******************************************************************************************************************/
VertexArray::~VertexArray()
{
	FL_GLCALL(glDeleteVertexArrays(1, &m_arrayObject));

	FL_LOG("[VERTEX ARRAY] Vertex array object destroyed: ", m_arrayObject, LOG_MEMORY);
}


/*******************************************************************************************************************
	Binds the vertex array object ID & makes it the active array object
*******************************************************************************************************************/
void VertexArray::Bind() const
{
	FL_GLCALL(glBindVertexArray(m_arrayObject));
}


/*******************************************************************************************************************
	Unbinds the vertex array object ID & makes it disactive
*******************************************************************************************************************/
void VertexArray::Unbind() const
{
	FL_GLCALL(glBindVertexArray(0));
}


/*******************************************************************************************************************
	Generate the vertex array objects ID
*******************************************************************************************************************/
void VertexArray::GenerateArrayObject()
{
	FL_GLCALL(glGenVertexArrays(1, &m_arrayObject));

	FL_LOG("[VERTEX ARRAY] Vertex array object created: ", m_arrayObject, LOG_MEMORY);
}