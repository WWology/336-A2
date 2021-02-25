// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "IndexBuffer.h"

/**
 * @brief Construct a new Index Buffer object
 *
 * @param t_Data The data to be passed onto the index buffer
 * @param t_Count The number of indices for the index buffer
 */
IndexBuffer::IndexBuffer(const GLvoid* t_Data, GLuint t_Count)
	: m_Count(t_Count)
{
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_Count, t_Data, GL_STATIC_DRAW);
}

/**
 * @brief Destroy the Index Buffer object
 *
 */
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}

/**
 * @brief Bind the IBO object
 *
 */
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

/**
 * @brief Unbind the IBO object
 *
 */
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}