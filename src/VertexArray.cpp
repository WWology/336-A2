// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ArrayID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

/**
 * @brief Add a Vertex Buffer Object
 *
 * @param t_Vb The Vertex Buffer Object to be added
 * @param t_Layout Layout specification for the Vertex Buffer Data
 */
void VertexArray::addBuffer(const VertexBuffer& t_Vb, const Layout& t_Layout)
{
	Bind();
	t_Vb.Bind();
	glVertexAttribPointer(t_Layout.index, 3, GL_FLOAT, GL_FALSE, t_Layout.stride, t_Layout.offset);
	glEnableVertexAttribArray(t_Layout.index);
}
