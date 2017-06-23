#pragma once

#include "Transform.h"
#include <glm\glm.hpp>

class Light
{
public:
	Light(Transform& _transform);
	~Light();

	glm::vec3 GetForwards() const;
	glm::vec3 GetDirection(glm::vec3& _targetPos) const;

	Transform* m_transform;
	glm::vec3 m_colour;
};
