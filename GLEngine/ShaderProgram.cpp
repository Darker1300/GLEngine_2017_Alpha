//#include "DEBUG_NEW_LEAK_DETECT.h"
//#include "DEBUG_WINDOWS_ERR_MSG.h"
//
//#include "ShaderProgram.h"
//
//#include <gl_core_4_4.h>
//#include <GlFW/glfw3.h>
//#include <iostream>
//
//#include "ShaderCollection.h"
//
//#pragma region IShaderBase
//
//IShaderBase::IShaderBase(
//	unsigned int _shaderType, const char * const _source)
//{
//	m_shaderID = glCreateShader(_shaderType);
//
//	glShaderSource(m_shaderID, 1, &_source, 0);
//	glCompileShader(m_shaderID);
//}
//
//IShaderBase::~IShaderBase() {}
//
//void IShaderBase::AttachLinkShaders(ShaderProgram* _container)
//{
//	glAttachShader(_container->m_programID, m_shaderID);
//}
//
//void IShaderBase::DeleteComponent()
//{
//	glDeleteShader(m_shaderID);
//}
//
//#pragma endregion IShaderBase
//
//#pragma region Vertex Shader
//
//VertexShader::VertexShader(const char * const _source)
//	: IShaderBase(GL_VERTEX_SHADER, _source) {}
//
//VertexShader::~VertexShader() {}
//
//#pragma endregion Vertex Shader
//
//#pragma region Fragment Shader
//
//FragmentShader::FragmentShader(const char * const _source)
//	: IShaderBase(GL_FRAGMENT_SHADER, _source) {}
//
//FragmentShader::~FragmentShader() {}
//
//#pragma endregion Fragment Shader
//
////ShaderProgram::ShaderProgram(IShaderBase ** _shaders, unsigned int _shadersCount, const char** const _uniforms, const unsigned int _uniformsCount)
////{
////	m_shaders = _shaders;
////	m_shadersCount = _shadersCount;
////
////	SetupShaderProgram();
////}
//
//ShaderProgram::ShaderProgram(VertexShader * _vertex, FragmentShader * _fragment, const char* const _uniforms[], const unsigned int _uniformsCount)
//{
//	static const int size = 2;
//	m_shaders = new IShaderBase*[size] { _vertex, _fragment };
//	m_shadersCount = size;
//
//	SetupShaderProgram();
//	//AssignUniforms(_uniforms, _uniformsCount);
//	// DeleteComponents();
//}
//
//ShaderProgram::~ShaderProgram()
//{
//	for (unsigned int i = 0; i < m_shadersCount; i++) {
//		delete m_shaders[i];
//	}
//	delete[] m_shaders;
//	// Delete program
//	glDeleteProgram(m_programID);
//}
//
//void ShaderProgram::UseShader(bool _status)
//{
//	glUseProgram(_status ? m_programID : 0);
//}
//
//void ShaderProgram::SetupShaderProgram()
//{
//	CreateProgramID();
//	AttachLinkShaders();
//#ifdef _DEBUG
//	TestCompilation();
//#endif
//}
//
//void ShaderProgram::AssignUniforms(const char* const _uniforms[], const unsigned int _uniformsCount)
//{
//	for (unsigned int u = 0; u < _uniformsCount; u++)
//	{
//		const char* text = _uniforms[u];
//		if (text == NULL) { LOG_ERROR("_uniformsCount: ", _uniformsCount, " is incorrect.") }
//
//		int location = glGetUniformLocation(m_programID, text);
//		if (location < 0) { LOG_ERROR("glGetUniformLocation( ", text, " ) failed.") }
//
//		m_uniforms[location] = text;
//	}
//}
//
//void ShaderProgram::CreateProgramID()
//{
//	m_programID = glCreateProgram();
//}
//
//void ShaderProgram::AttachLinkShaders()
//{
//	for (unsigned int i = 0; i < m_shadersCount; i++) {
//		m_shaders[i]->AttachLinkShaders(this);
//	}
//	glLinkProgram(m_programID);
//}
//
//void ShaderProgram::TestCompilation()
//{
//	int success = GL_FALSE;
//	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
//	if (success == GL_FALSE)
//	{
//		// DEBUG_LOG
//		int infoLogLength = 0;
//		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
//		char* infoLog = new char[infoLogLength];
//		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
//		printf("Error: Failed to link shader program!\n");
//		printf("%s\n", infoLog);
//		// DEBUG_LOG::ERROR_MSG(std::string("Failed to link shader program " + std::to_string(m_programID) + "."));
//		delete[] infoLog;
//	}
//}
//
//void ShaderProgram::DeleteComponents()
//{
//	for (unsigned int i = 0; i < m_shadersCount; i++) {
//		m_shaders[i]->DeleteComponent();
//	}
//}
