#include "BoundingSphere.h"

bool BoundingSphere::FrustumCollision(const Camera::Frustum & _frustum) const
{
	for (int i = 0; i < 6; i++) {
		float d = glm::dot(glm::vec3(_frustum[i]), centre) +
			_frustum[i].w;
		if (d < -radius) {
			// Behind, don't render it
			return false;
		}
		else if (d < radius) {
			// Touching, we still need to render it
		}
		else {
			// Front, fully visible so render it
		}
	}
	return true;
}
