#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader
{
private:
	GLuint m_ProgramID;
	std::string m_VsSource, m_FsSource;
	std::unordered_map<std::string, GLint> m_UniformLocationCache;

public:
	Shader(const std::string& t_VsFilename, const std::string& t_FsFilename);
	~Shader();
	void Bind() const;
	void Unbind() const;
	GLuint getID() const { return m_ProgramID; }
	GLint getUniformLocation(const std::string& t_Uname);
	void setUniformMatrix4fv(const std::string& t_Uname, const GLfloat* t_Value);
	void setUniform3fv(const std::string& t_Uname, const GLfloat* t_Value);

private:
	void
	loadShadersFromFile(const std::string& t_VsFilename, const std::string& t_FsFilename);
	GLuint compileShader(const std::string& t_Source, GLenum t_Type);
	GLuint createShader(const std::string& t_VsSource, const std::string& t_FsSource);
};
