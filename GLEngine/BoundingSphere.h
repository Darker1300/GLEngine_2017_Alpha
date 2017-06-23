#pragma once

#include <glm\glm.hpp>
#include <vector>
#include "Camera.h"

class Transform;

class BoundingSphere {
public:

	BoundingSphere() : centre(0), radius(0) { }
	~BoundingSphere() {}

	template <typename T>
	void Fit(T* _first, unsigned _count);


	bool FrustumCollision(const Camera::Frustum& _frustum, const Transform& _transform) const;

	glm::vec3 centre;
	float radius;
};

template<typename T>
inline void BoundingSphere::Fit(T * _first, unsigned _count) {
	glm::vec3 min(1e37f), max(-1e37f);

	for (int i = 0; i < (int)_count; i++) {
		glm::vec4& p = _first[i].position;
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}
	centre = (min + max) * 0.5f;
	radius = glm::distance(min, centre);
}
