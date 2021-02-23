#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader
{
private:
	GLuint m_ProgramID;
	std::string m_VsSource, m_FsSource;
	std::unordered_map<std::string, GLuint> m_UniformLocationCache;

public:
	Shader(const std::string& t_VsFilename, const std::string& t_FsFilename);
	~Shader();
	void Bind() const;
	void Unbind() const;
	GLuint getID() const { return m_ProgramID; }
	GLuint getUniformLocation(const std::string& t_Uname);

private:
	void
	loadShadersFromFile(const std::string& t_VsFilename, const std::string& t_FsFilename);
	GLuint compileShader(const std::string& t_Source, GLenum t_Type);
	GLuint createShader(const std::string& t_VsSource, const std::string& t_FsSource);
};
