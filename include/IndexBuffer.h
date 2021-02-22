#pragma once

#include <GL/glew.h>
class IndexBuffer
{
private:
	GLuint m_BufferID;

public:
	IndexBuffer(const GLvoid* data);
	~IndexBuffer();

	GLuint getID() const { return m_BufferID; }

	void Bind() const;
	void Unbind() const;
};