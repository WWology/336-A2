// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "VertexBuffer.h"

/**
 * @brief Construct a new Vertex Buffer object
 *
 * @param t_Data Data to be stored in the Vertex Buffer
 * @param t_Size The Size of the Data in the vertex Buffer
 */
VertexBuffer::VertexBuffer(const GLvoid* t_Data, GLuint t_Size)
{
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ARRAY_BUFFER, t_Size, t_Data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}