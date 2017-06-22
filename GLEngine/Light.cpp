#include "Light.h"



Light::Light()
	: m_transform()
	, m_colour(1)
{
}


Light::~Light()
{
}

glm::vec3 Light::GetForwards()
{
	glm::mat4& matrix = m_transform.GetLocalMatrix();
	return glm::normalize(glm::vec3(matrix[2]));
}

glm::vec3 Light::GetDirection(glm::vec3& _targetPos)
{
	return glm::normalize(_targetPos - m_transform.position);
}
