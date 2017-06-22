#include "DEBUG_NEW_LEAK_DETECT.h"

// #define USE_CONSOLE_LOG 1
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "Shader.h"
#include <gl_core_4_4.h>
#include <GlFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragPath)
	: m_programID(-1)
{
	MakeShaderProgram(vertexPath, fragPath);
}

Shader::~Shader()
{
	if (m_programID != -1) glDeleteProgram(m_programID);
}

Shader::Shader(Shader && _other)
{
	if (m_programID != -1) glDeleteProgram(m_programID);

	m_programID = _other.m_programID;
	_other.m_programID = -1;
}

void Shader::MakeShaderProgram(const std::string & _vertexPath, const std::string & _fragPath)
{
	unsigned int m_vertexShaderID = MakeShader(GL_VERTEX_SHADER, _vertexPath);
	unsigned int m_fragmentShaderID = MakeShader(GL_FRAGMENT_SHADER, _fragPath);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);
	glLinkProgram(m_programID);

	TestCompilation(m_programID);
}

unsigned int Shader::MakeShader(unsigned int _type, const std::string & _path)
{
	std::string text = LoadText(_path);
	const char * shaderCode = text.c_str();
	unsigned int shaderID = glCreateShader(_type);

	glShaderSource(shaderID, 1, (const char**)&shaderCode, NULL);
	glCompileShader(shaderID);

	return shaderID;
}

void Shader::TestCompilation(unsigned int _programID)
{
	int success = GL_FALSE;
	glGetProgramiv(_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		// DEBUG_LOG
		int infoLogLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_programID, infoLogLength, 0, infoLog);
		// Log error
		LOG_ERROR("Failed to link shader program ", _programID, ".\n", infoLog);

		delete[] infoLog;
	}
}

std::string Shader::LoadText(const std::string & _path)
{
	std::ifstream file;
	file.open(_path.c_str(), std::ifstream::in);

	if (!file.good()) {
		LOG_ERROR("Failed to load file. ", _path);
		return std::string("ERROR");
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	return ss.str();
}
