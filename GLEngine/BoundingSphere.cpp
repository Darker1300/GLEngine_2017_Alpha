#include "BoundingSphere.h"
#include "Transform.h"

bool BoundingSphere::FrustumCollision(const Camera::Frustum & _frustum, const Transform& _transform) const
{
	glm::vec3 transformedCentre = glm::vec3(glm::vec4(centre + _transform.position, 1.0f)/* * _transform.GetLocalMatrix()*/);
	float largestRadius = 0;

	// Calculate radius
	for (int i = 0; i < 3; i++) {
		if (_transform.scale[i] > largestRadius)
			largestRadius = _transform.scale[i];
	}
	largestRadius = (radius * largestRadius);

	// Collision Test
	for (int i = 0; i < 6; i++) {
		float d = glm::dot(glm::vec3(_frustum[i]), transformedCentre) +
			_frustum[i].w;

		if (d < -largestRadius) {
			// Behind, don't render it
			return false;
		}
		else if (d < largestRadius) {
			// Touching, we still need to render it
			continue;
		}
		else {
			// Front, fully visible so render it
			continue;
		}
	}
	return true;
}
