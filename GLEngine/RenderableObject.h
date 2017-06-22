#pragma once

class Material;
class RenderData;

#include "Transform.h"
#include <vector>
#include "Camera.h"

class RenderableObject
{
public:
	RenderableObject(Material* _material, std::vector<RenderData*>& _renderShapes);

	// TODO COPY/MOVE

	~RenderableObject();

	void Bind() const;
	void Render();
	void Unbind() const;
	bool FrustrumCollision(const Camera::Frustum& _frustum) const;

	Material* m_material;
	std::vector<RenderData*> m_renderShapes;
	Transform m_transform;
};

