#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "GeometryHelper.h"

#include <gl_core_4_4.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "RenderData.h"
#include "tiny_obj_loader.h"

namespace GeometryHelper {
	RenderData * CreatePlane(const unsigned int _rows, const unsigned int _cols, const float _width, const float _height)
	{
		// Vertices
		unsigned int verticesSize = _rows * _cols;
		std::vector<OBJVertex> vertices = std::vector<OBJVertex>();// new OBJVertex[verticesSize];
		vertices.reserve(verticesSize);

		float colSpacing = _width / (_cols - 1);
		float rowSpacing = _height / (_rows - 1);

		float uvFactorX = 1.0f / (_cols - 1);
		float uvFactorY = 1.0f / (_rows - 1);

		// Vertices Generation
		for (unsigned int r = 0; r < _rows; r++)
		{
			for (unsigned int c = 0; c < _cols; c++)
			{
				vertices.push_back(OBJVertex());
				OBJVertex& vert = vertices[r * _cols + c];
				// Pos
				vert.position = glm::vec4(
					-(_width / 2) + (colSpacing * c),
					-(_height / 2) + (rowSpacing * r),
					0, 1);
				// UVs
				vert.uv = glm::vec2(
					c * uvFactorX,
					r * uvFactorY);
			}
		}
		// Indices
		unsigned int indicesSize = (_rows - 1) * (_cols - 1) * 6;
		std::vector<unsigned int> indices = std::vector<unsigned int>();
		indices.reserve(indicesSize);

		// Indices Generation
		unsigned int index = 0;
		for (unsigned int r = 0; r < (_rows - 1); ++r)
		{
			for (unsigned int c = 0; c < (_cols - 1); ++c)
			{
				// Triangle 1
				indices.push_back(r * _cols + c);
				indices.push_back((r + 1) * _cols + c);
				indices.push_back((r + 1) * _cols + (c + 1));
				// Triangle 2
				indices.push_back(r * _cols + c);
				indices.push_back((r + 1) * _cols + (c + 1));
				indices.push_back(r * _cols + (c + 1));
			}
		}
		// Calculate Tangents
		CalculateTangents(vertices, indices);

		// Store on GPU
		RenderData* renderData = new RenderData();
		renderData->GenerateBuffers(true);

		renderData->Bind();
		renderData->FillIndexBuffer(&indices[0], indicesSize);
		renderData->FillVertexBuffer(&vertices[0], verticesSize);
		renderData->SetFloatAttributePointer(0, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::position));
		renderData->SetFloatAttributePointer(1, 2, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::uv));
		renderData->SetFloatAttributePointer(2, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::normal));
		renderData->SetFloatAttributePointer(3, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::tangent));
		renderData->SetFloatAttributePointer(4, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::bitangent));
		renderData->Unbind();

		// Configure RenderData
		renderData->SetIndicesSize(indicesSize);
		renderData->SetPrimitiveType(GL_TRIANGLES);

		// Delete cpu-side data
		vertices.clear();
		indices.clear();

		return renderData;
	}

	RenderData * CreateQuad()
	{
		return GeometryHelper::CreatePlane(2, 2, 1, 1);
	}

	RenderData * CreateFullscreenQuad(const float _windowWidth, const float _windowHeight)
	{
		std::vector<OBJVertex> vertexData;
		vertexData.reserve(6);
		for (int i = 0; i < 6; i++) vertexData.push_back(OBJVertex());

		vertexData[0].position = glm::vec4(-1, -1, 0, 1);
		vertexData[1].position = glm::vec4(1, 1, 0, 1);
		vertexData[2].position = glm::vec4(-1, 1, 0, 1);
		vertexData[3].position = glm::vec4(-1, -1, 0, 1);
		vertexData[4].position = glm::vec4(1, -1, 0, 1);
		vertexData[5].position = glm::vec4(1, 1, 0, 1);

		glm::vec2 halfTexel = 1.0f / glm::vec2(_windowWidth, _windowHeight) * 0.5f;
		vertexData[0].uv = glm::vec2(halfTexel.x, halfTexel.y);
		vertexData[1].uv = glm::vec2(1 - halfTexel.x, 1 - halfTexel.y);
		vertexData[2].uv = glm::vec2(halfTexel.x, 1 - halfTexel.y);
		vertexData[3].uv = glm::vec2(halfTexel.x, halfTexel.y);
		vertexData[4].uv = glm::vec2(1 - halfTexel.x, halfTexel.y);
		vertexData[5].uv = glm::vec2(1 - halfTexel.x, 1 - halfTexel.y);

		RenderData* renderData = new RenderData();
		renderData->GenerateBuffers(false);

		CalculateTangents(vertexData);

		renderData->FillVertexBuffer(&vertexData[0], 6);
		renderData->SetFloatAttributePointer(0, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::position));
		renderData->SetFloatAttributePointer(1, 2, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::uv));
		renderData->SetFloatAttributePointer(2, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::normal));
		renderData->SetFloatAttributePointer(3, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::tangent));
		renderData->SetFloatAttributePointer(4, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::bitangent));
		renderData->Unbind();

		return renderData;
	}

	RenderData * CreateSphere(const float _radius, const unsigned int _rings, const unsigned int _sectors)
	{
		std::vector<OBJVertex> vertices;
		std::vector<unsigned int> indices;

#pragma region Generate Mesh

		float const R = 1.f / (float)(_rings - 1);
		float const S = 1.f / (float)(_sectors - 1);
		float const pi = glm::pi<float>();
		float const piHalf = glm::half_pi<float>();
		int r, s;

		vertices.resize(_rings * _sectors * 3);
		std::vector<OBJVertex>::iterator v = vertices.begin();
		for (r = 0; r < (int)_rings; r++) for (s = 0; s < (int)_sectors; s++) {
			float const y = sin(-piHalf + pi * r * R);
			float const x = cos(2 * pi * s * S) * sin(pi * r * R);
			float const z = sin(2 * pi * s * S) * sin(pi * r * R);

			// Position
			glm::vec4 position = glm::vec4(x * _radius, y * _radius, z * _radius, 1);
			v->position = position;

			// Texture Co-ord
			v->uv = glm::vec2(s*S, r*R);

			// Normal
			v->normal = glm::vec4(x, y, z, 0);
			v++;
		}

		// Indices
		indices.resize(_rings * _sectors * 4);
		std::vector<unsigned int>::iterator i = indices.begin();
		for (r = 0; r < (int)_rings - 1; r++) for (s = 0; s < (int)_sectors - 1; s++) {
			*i++ = r * _sectors + s;
			*i++ = r * _sectors + (s + 1);
			*i++ = (r + 1) * _sectors + (s + 1);
			*i++ = (r + 1) * _sectors + s;
		}

#pragma endregion Generate Mesh

		RenderData* renderData = new RenderData();
		renderData->GenerateBuffers(true);

		CalculateTangents(vertices);

		renderData->FillIndexBuffer(&indices[0], indices.size());
		renderData->FillVertexBuffer(&vertices[0], vertices.size());

		renderData->SetFloatAttributePointer(0, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::position));
		renderData->SetFloatAttributePointer(1, 2, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::uv));
		renderData->SetFloatAttributePointer(2, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::normal));
		renderData->SetFloatAttributePointer(3, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::tangent));
		renderData->SetFloatAttributePointer(4, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::bitangent));

		renderData->SetPrimitiveType(GL_QUADS);

		renderData->Unbind();

		return renderData;
	}

	std::vector<RenderData*> LoadOBJFromDisk(const std::string& _path)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		// Load
		bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, _path.c_str());
		if (!success) { LOG_ERROR("Could not load OBJ correctly. Path: ", _path, ".") }

		// Parse
		std::vector<RenderData*> modelRenderData;
		modelRenderData.reserve(shapes.size());

		// grab each shape
		int shapeIndex = 0;
		for (auto& shape : shapes) {

			// collect triangle vertices
			std::vector<OBJVertex> vertices;
			int index = 0;
			for (auto face : shape.mesh.num_face_vertices)
			{
				for (int i = 0; i < 3; ++i)
				{
					tinyobj::index_t idx = shape.mesh.indices[index + i];
					OBJVertex v = { {},{},{} };
					// positions
					v.position.x = attribs.vertices[3 * idx.vertex_index + 0];
					v.position.y = attribs.vertices[3 * idx.vertex_index + 1];
					v.position.z = attribs.vertices[3 * idx.vertex_index + 2];
					v.position.w = 1;
					// normals
					if (attribs.normals.size() > 0)
					{
						v.normal.x = attribs.normals[3 * idx.normal_index + 0];
						v.normal.y = attribs.normals[3 * idx.normal_index + 1];
						v.normal.z = attribs.normals[3 * idx.normal_index + 2];
						v.normal.w = 0;
					} // texture coordinates
					if (attribs.texcoords.size() > 0)
					{
						v.uv.s = attribs.texcoords[2 * idx.texcoord_index + 0];
						v.uv.t = attribs.texcoords[2 * idx.texcoord_index + 1];
					}
					vertices.push_back(v);
				}
				index += face;
			}
			// Calculate Tangents
			CalculateTangents(vertices);

			// Parse shape to GPU
			RenderData* renderData = new RenderData();
			modelRenderData.push_back(renderData);
			// Set indices size (in this case == vertices size)
			renderData->GenerateBuffers(false);

			renderData->Bind();
			renderData->SetIndicesSize((UINT)vertices.size());
			renderData->FillVertexBuffer(&vertices[0], vertices.size());
			renderData->SetFloatAttributePointer(0, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::position));
			renderData->SetFloatAttributePointer(1, 2, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::uv));
			renderData->SetFloatAttributePointer(2, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::normal));
			renderData->SetFloatAttributePointer(3, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::tangent));
			renderData->SetFloatAttributePointer(4, 4, sizeof(OBJVertex), offsetof(OBJVertex, OBJVertex::bitangent));
			renderData->Unbind();
		}

		return modelRenderData;
	}

	void CalculateTangents(std::vector<OBJVertex>& vertices)
	{
		unsigned int vertexCount = (unsigned int)vertices.size();
		glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
		glm::vec4* tan2 = tan1 + vertexCount;
		memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

		for (unsigned int a = 0; a < (unsigned int)vertices.size(); a += 3) {
			long i1 = a;
			long i2 = a + 1;
			long i3 = a + 2;

			const glm::vec4& v1 = vertices[i1].position;
			const glm::vec4& v2 = vertices[i2].position;
			const glm::vec4& v3 = vertices[i3].position;

			const glm::vec2& w1 = vertices[i1].uv;
			const glm::vec2& w2 = vertices[i2].uv;
			const glm::vec2& w3 = vertices[i3].uv;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r, 0);
			glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r, 0);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (unsigned int a = 0; a < vertexCount; a++) {
			const glm::vec3& n = glm::vec3(vertices[a].normal);
			const glm::vec3& t = glm::vec3(tan1[a]);

			// Gram-Schmidt orthogonalize
			vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

			// Calculate handedness (direction of bitangent)
			vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;

			// calculate bitangent (ignoring for Vertex)
			vertices[a].bitangent = glm::vec4(glm::cross(glm::vec3(vertices[a].normal), glm::vec3(vertices[a].tangent)) * vertices[a].tangent.w, 0);
			vertices[a].tangent.w = 0;
		}

		delete[] tan1;
	}

	void CalculateTangents(std::vector<OBJVertex>& vertices, std::vector<unsigned int>& indices)
	{
		unsigned int vertexCount = (unsigned int)indices.size();
		glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
		glm::vec4* tan2 = tan1 + vertexCount;
		memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

		for (unsigned int a = 0; a < (unsigned int)indices.size(); a += 3) {
			long i1 = indices[a];
			long i2 = indices[a + 1];
			long i3 = indices[a + 2];

			const glm::vec4& v1 = vertices[i1].position;
			const glm::vec4& v2 = vertices[i2].position;
			const glm::vec4& v3 = vertices[i3].position;

			const glm::vec2& w1 = vertices[i1].uv;
			const glm::vec2& w2 = vertices[i2].uv;
			const glm::vec2& w3 = vertices[i3].uv;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r, 0);
			glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r, 0);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (unsigned int a = 0; a < vertexCount; a++) {
			const glm::vec3& n = glm::vec3(vertices[indices[a]].normal);
			const glm::vec3& t = glm::vec3(tan1[indices[a]]);

			// Gram-Schmidt orthogonalize
			vertices[indices[a]].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

			// Calculate handedness (direction of bitangent)
			vertices[indices[a]].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[indices[a]])) < 0.0F) ? 1.0F : -1.0F;

			// calculate bitangent (ignoring for Vertex)
			vertices[indices[a]].bitangent = glm::vec4(glm::cross(glm::vec3(vertices[indices[a]].normal), glm::vec3(vertices[indices[a]].tangent)) * vertices[indices[a]].tangent.w, 0);
			vertices[indices[a]].tangent.w = 0;
		}

		delete[] tan1;
	}
}
