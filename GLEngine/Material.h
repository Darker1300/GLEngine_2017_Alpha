#pragma once
#include <string>
#include <vector>
#include <map>
#include <glm\glm.hpp>

class Shader;
class Texture;

class Material
{
public:
	Material(Shader* _shader = nullptr);
	Material(const Material& _other);
	Material& operator=(const Material& _other);
	~Material();

	Shader* m_shader;
	std::map<std::string, Texture*> m_textures;

	void Bind() const;
	void Unbind() const;

	void ApplyUniformMat4(const std::string& _name, const glm::mat4& _matrix) const;
	void ApplyUniformFloat(const std::string& _name, const float& _value) const;
	void ApplyUniformVec2(const std::string& _name, const glm::vec2& _vec) const;
	void ApplyUniformVec3(const std::string& _name, const glm::vec3& _vec) const;
	void ApplyUniformVec4(const std::string& _name, const glm::vec4& _vec) const;

	int GetUniformLocation(const std::string& _name) const;
};

