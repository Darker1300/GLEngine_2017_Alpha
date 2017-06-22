#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "Material.h"

#include "Shader.h"
#include "Texture.h"

#include <gl_core_4_4.h>
#include <glm\ext.hpp>

Material::Material(Shader * _shader)
	: m_shader(_shader)
{
}

Material::Material(const Material & _other)
	: m_shader(_other.m_shader)
	, m_textures(_other.m_textures)
{
}

Material & Material::operator=(const Material & _other)
{
	m_shader = _other.m_shader;
	m_textures = _other.m_textures;
	return *this;
}

Material::~Material()
{
}

void Material::Bind() const
{
	glUseProgram(m_shader->GetProgramID());

	// Bind Textures
	auto iter = m_textures.begin();
	for (unsigned int count = 0;
		iter != m_textures.end(); iter++, count++)
	{
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, iter->second->GetID());
	}
}

void Material::Unbind() const
{
	// Unbind Textures
	auto iter = m_textures.begin();
	for (unsigned int count = 0;
		iter != m_textures.end(); iter++, count++)
	{
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glUseProgram(0);
}

void Material::ApplyUniformMat4(const std::string & _name, const glm::mat4 & _matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(_name), 1, false, glm::value_ptr(_matrix));
}

void Material::ApplyUniformFloat(const std::string & _name, const float & _value) const
{
	glUniform1fv(GetUniformLocation(_name), 1, &_value);
}

void Material::ApplyUniformVec2(const std::string & _name, const glm::vec2 & _vec) const
{
	glUniform2fv(GetUniformLocation(_name), 1, glm::value_ptr(_vec));
}

void Material::ApplyUniformVec3(const std::string & _name, const glm::vec3 & _vec) const
{
	glUniform3fv(GetUniformLocation(_name), 1, glm::value_ptr(_vec));
}

void Material::ApplyUniformVec4(const std::string & _name, const glm::vec4 & _vec) const
{
	glUniform4fv(GetUniformLocation(_name), 1, glm::value_ptr(_vec));
}

int Material::GetUniformLocation(const std::string & _name) const
{
	int location = glGetUniformLocation(m_shader->GetProgramID(), _name.c_str());
	if (location < 0) { LOG_ERROR("glGetUniformLocation( ", _name, " ) failed.") }
	return location;
}
