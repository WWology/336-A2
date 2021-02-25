#pragma once

#include <GL/glew.h>
class IndexBuffer
{
private:
	GLuint m_BufferID;
	GLuint m_Count;

public:
	IndexBuffer(const GLvoid* t_Data, GLuint t_Count);
	~IndexBuffer();

	GLuint getID() const { return m_BufferID; }
	GLuint getCount() const { return m_Count; }

	void Bind() const;
	void Unbind() const;
};