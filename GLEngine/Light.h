#pragma once

#include "Transform.h"
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	~Light();

	glm::vec3 GetForwards();
	glm::vec3 GetDirection(glm::vec3& _targetPos);

	Transform m_transform;
	glm::vec3 m_colour;
};
