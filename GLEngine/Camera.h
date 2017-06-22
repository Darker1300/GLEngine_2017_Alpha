#pragma once

#include <glm\glm.hpp>

struct GLFWwindow;

class Camera;
namespace GLE { extern Camera* MAIN_CAM; }

class Camera
{
public:
	struct Frustum {
		Frustum() : Planes() {}
		union {
			glm::vec4 Planes[6];
			struct { glm::vec4 Right, Left, Top, Bottom, Far, Near; };
		};
		glm::vec4& operator[] (const int index) { return Planes[index]; }
		const glm::vec4& operator[] (const int index) const { return Planes[index]; }
	};

	Camera();
	~Camera();

	// X:Yaw, Y:Pitch, Z:Roll. Rotated in following order: X, Y, Z
	glm::mat4 GetRotationMatrix();
	glm::mat4 GetLocalMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetProjectionViewMatrix() { return GetProjectionMatrix() * GetViewMatrix(); }
	Camera::Frustum GetFrustum();

	void SetClipping(float _near, float _far);
	void UpdateFly(GLFWwindow* _window, float _deltaTime, float _moveSpeed = 1.0f, float _rotSpeed = 1.0f);

	inline static float ClampRadian(const float& _value);

	union {
		// Radians. X:Yaw, Y:Pitch, Z:Roll. Rotated in following order: X, Y, Z
		glm::vec3 forward;
		struct { float yaw, pitch, roll; };
	};
	glm::vec3 position;
	float m_FOV, m_aspectRatio, m_near, m_far;

private:
	void CalcFrustumPlanes(const glm::mat4 & _projectionView, Camera::Frustum & _frustum);
};
