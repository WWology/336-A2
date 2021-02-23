#pragma once

#include <GL/glew.h>
class VertexBuffer
{
private:
	GLuint m_BufferID;

public:
	VertexBuffer(const GLvoid* t_Data, GLuint t_Size);
	~VertexBuffer();

	GLuint getID() const { return m_BufferID; }

	void Bind() const;
	void Unbind() const;
};