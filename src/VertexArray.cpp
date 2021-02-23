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

void VertexArray::addBuffer(const VertexBuffer& t_Vb, const Layout& t_Layout)
{
	Bind();
	t_Vb.Bind();
	glVertexAttribPointer(t_Layout.index, 3, GL_FLOAT, GL_FALSE, t_Layout.stride, t_Layout.offset);
	glEnableVertexAttribArray(t_Layout.index);
}
