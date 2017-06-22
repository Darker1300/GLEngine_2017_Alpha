#pragma once
#include <string>

class ParticleSystem;

class Shader
{
public:
	friend class ParticleSystem;
	Shader(const std::string& vertexPath, const std::string& fragPath);
	~Shader();

	Shader(const Shader& _other) = delete;
	Shader(Shader&& _other);

	Shader& operator=(const Shader& _other) = delete;

	unsigned int GetProgramID() const { return m_programID; }

private:
	void				MakeShaderProgram(const std::string& _vertexPath, const std::string& _fragPath);
	static unsigned int	MakeShader(unsigned int _type, const std::string& _path);
	static void			TestCompilation(unsigned int _programID);
	static std::string	LoadText(const std::string& _path);

	unsigned int	m_programID;
};
