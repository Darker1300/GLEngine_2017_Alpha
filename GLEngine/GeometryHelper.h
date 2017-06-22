#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

class RenderData;

namespace GeometryHelper
{
	struct OBJVertex {
		// x, y, z, w
		glm::vec4 position;
		// u, v
		glm::vec2 uv;
		// nx, ny, nz, nw
		glm::vec4 normal;
		// tx, ty, tz, tw
		glm::vec4 tangent;
		// btx, bty, btz, btw
		glm::vec4 bitangent;
	};

	RenderData* CreatePlane(const unsigned int _rows, const unsigned int _cols, const float _width, const float _height);
	RenderData* CreateQuad();
	RenderData* CreateFullscreenQuad(const float _windowWidth, const float _windowHeight);
	RenderData* CreateSphere(const float _radius, const unsigned int _rings, const unsigned int _sectors);

	std::vector<RenderData*> LoadOBJFromDisk(const std::string& _path);
	void CalculateTangents(std::vector<OBJVertex>& vertices);
	void CalculateTangents(std::vector<OBJVertex>& vertices, std::vector<unsigned int>& indices);
};

