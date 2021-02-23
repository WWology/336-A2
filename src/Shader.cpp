// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string& t_VsFilename, const std::string& t_FsFilename)
	: m_ProgramID(0)
{
	loadShadersFromFile(t_VsFilename, t_FsFilename);
	m_ProgramID = createShader(m_VsSource, m_FsSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_ProgramID);
}

void Shader::Bind() const
{
	glUseProgram(m_ProgramID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

GLuint Shader::getUniformLocation(const std::string& t_Uname)
{
	if (m_UniformLocationCache.find(t_Uname) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[t_Uname];
	}

	GLint u_LocationIdx = glGetUniformLocation(m_ProgramID, t_Uname.c_str());
	if (u_LocationIdx != -1)
	{
		std::cout << "Warning: Uniform " << t_Uname << " doesn't exist!" << std::endl;
	}

	m_UniformLocationCache[t_Uname] = u_LocationIdx;
	return u_LocationIdx;
}

void Shader::loadShadersFromFile(const std::string& t_VsFilename, const std::string& t_FsFilename)
{
	std::ifstream vsFile(t_VsFilename, std::ios::in); // open file stream

	if (vsFile)
	{
		std::string line = "";
		while (std::getline(vsFile, line))
			m_VsSource += line + "\n";

		vsFile.close();
	}
	else
	{
		std::cerr << "Failed to open vertex shader file - " << t_VsFilename << std::endl;
		exit(EXIT_FAILURE);
	}

	std::ifstream fsFile(t_FsFilename, std::ios::in);

	if (fsFile)
	{
		std::string line = "";
		while (std::getline(fsFile, line))
			m_FsSource += line + "\n";

		fsFile.close();
	}
	else
	{
		std::cerr << "Failed to open fragment shader file - " << t_FsFilename << std::endl;
		exit(EXIT_FAILURE);
	}
}

GLuint Shader::compileShader(const std::string& t_Source, GLenum t_Type)
{
	GLuint shaderID = glCreateShader(t_Type);
	const GLchar* source = t_Source.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);

	glCompileShader(shaderID);

	GLint status = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		// output error message
		std::cerr << "Failed to compile vertex shader" << std::endl;

		// output error information
		int infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, errorMessage);
		std::cerr << errorMessage << std::endl;
		glDeleteShader(shaderID);
		delete[] errorMessage;

		exit(EXIT_FAILURE);
	}

	return shaderID;
}

GLuint Shader::createShader(const std::string& t_vsSource, const std::string& t_fsSource)
{
	GLuint progID = glCreateProgram();

	GLuint vsID = compileShader(t_vsSource, GL_VERTEX_SHADER);
	GLuint fsID = compileShader(t_fsSource, GL_FRAGMENT_SHADER);

	glAttachShader(progID, vsID);
	glAttachShader(progID, fsID);
	glLinkProgram(progID);
	glValidateProgram(progID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return progID;
}
