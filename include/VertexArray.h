#pragma once

#include "VertexBuffer.h"

#include <GL/glew.h>

struct Layout
{
	GLuint index;
	GLsizei stride;
	GLvoid* offset;
};

#include "GL/glew.h"
class VertexArray
{
private:
	GLuint m_ArrayID;

public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void addBuffer(const VertexBuffer& t_Vb, const Layout& t_Layout);
};