#include "Camera.h"

#include "Transform.h"
#include <glm\gtx\transform.hpp>
#include <glfw\glfw3.h>

Camera* GLE::MAIN_CAM = nullptr;

Camera::Camera()
	: forward(), position()
	, m_FOV(0.25f), m_aspectRatio(16.0f / 9.0f), m_near(0.1f), m_far(100.0f)
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::GetRotationMatrix()
{
	glm::mat4 rotMat = glm::mat4();
	// rotate around to a given bearing: yaw
	rotMat = glm::rotate(rotMat, yaw, Vector3::up);
	// rotate around to a given bearing: pitch
	rotMat = glm::rotate(rotMat, pitch, Vector3::right);
	// rotate around to a given bearing: roll
	rotMat = glm::rotate(rotMat, roll, Vector3::forward);
	return rotMat;
}

glm::mat4 Camera::GetLocalMatrix()
{
	glm::mat4 local = glm::mat4();
	// translate
	local = glm::translate(local, position);
	// rotate
	local = local * GetRotationMatrix();
	return local;
}

glm::mat4 Camera::GetViewMatrix()
{
	// get the view matrix by taking the transformation inverse
	return glm::inverse(GetLocalMatrix());
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(
		glm::pi<float>() * m_FOV, m_aspectRatio, m_near, m_far);
}

Camera::Frustum Camera::GetFrustum()
{
	Camera::Frustum frustum = Camera::Frustum();
	CalcFrustumPlanes(GetProjectionViewMatrix(), frustum);
	return frustum;
}

void Camera::SetClipping(float _near, float _far)
{
	m_near = _near;
	m_far = _far;
}

void Camera::UpdateFly(GLFWwindow* _window, float _deltaTime, float _moveSpeed, float _rotSpeed)
{
	glm::vec3 move = glm::vec3(0);
	glm::vec3 rotate = glm::vec3(0);

#pragma region Movement
	// Forward
	if (glfwGetKey(_window, GLFW_KEY_W)) {
		move -= Vector3::forward;
	}
	// Backwards
	if (glfwGetKey(_window, GLFW_KEY_S)) {
		move -= Vector3::backward;
	}
	// Left
	if (glfwGetKey(_window, GLFW_KEY_A)) {
		move += Vector3::left;
	}
	// Right
	if (glfwGetKey(_window, GLFW_KEY_D)) {
		move += Vector3::right;
	}
	// Up
	if (glfwGetKey(_window, GLFW_KEY_E)) {
		move += Vector3::up;
	}
	// Down
	if (glfwGetKey(_window, GLFW_KEY_Q)) {
		move += Vector3::down;
	}
#pragma endregion

#pragma region Rotation
	// Left
	if (glfwGetKey(_window, GLFW_KEY_KP_4)) {
		rotate -= Vector3::left;
	}
	// Right
	if (glfwGetKey(_window, GLFW_KEY_KP_6)) {
		rotate -= Vector3::right;
	}
	// Up
	if (glfwGetKey(_window, GLFW_KEY_KP_8)) {
		rotate += Vector3::up;
	}
	// Down
	if (glfwGetKey(_window, GLFW_KEY_KP_5)) {
		rotate += Vector3::down;
	}
	// Roll left
	if (glfwGetKey(_window, GLFW_KEY_KP_7)) {
		rotate += Vector3::forward;
	}
	// Roll right
	if (glfwGetKey(_window, GLFW_KEY_KP_9)) {
		rotate += Vector3::backward;
	}

#pragma endregion

	bool moved = glm::length(move) != 0.0f;
	bool rotated = glm::length(rotate) != 0.0f;
	// Apply
	if (moved || rotated) {
		if (moved) {
			glm::mat4 orientation = GetRotationMatrix();
			glm::vec3 localMove = glm::vec3(orientation * glm::vec4(move, 0));
			position += localMove * _deltaTime * _moveSpeed;
		}
		if (rotated) {
			yaw = ClampRadian(yaw + rotate[0] * _deltaTime * _rotSpeed);
			pitch = ClampRadian(pitch + rotate[1] * _deltaTime * _rotSpeed);
			roll = ClampRadian(roll + rotate[2] * _deltaTime * _rotSpeed);
		}
	}
}

float Camera::ClampRadian(const float & _value)
{
	return fmodf(_value, glm::two_pi<float>());
}

void Camera::CalcFrustumPlanes(const glm::mat4 & _projectionView, Camera::Frustum & _frustum)
{
	// right side
	_frustum[0] = glm::vec4(_projectionView[0][3] - _projectionView[0][0],
		_projectionView[1][3] - _projectionView[1][0],
		_projectionView[2][3] - _projectionView[2][0],
		_projectionView[3][3] - _projectionView[3][0]);
	// left side
	_frustum[1] = glm::vec4(_projectionView[0][3] + _projectionView[0][0],
		_projectionView[1][3] + _projectionView[1][0],
		_projectionView[2][3] + _projectionView[2][0],
		_projectionView[3][3] + _projectionView[3][0]);
	// top
	_frustum[2] = glm::vec4(_projectionView[0][3] - _projectionView[0][1],
		_projectionView[1][3] - _projectionView[1][1],
		_projectionView[2][3] - _projectionView[2][1],
		_projectionView[3][3] - _projectionView[3][1]);
	// bottom
	_frustum[3] = glm::vec4(_projectionView[0][3] + _projectionView[0][1],
		_projectionView[1][3] + _projectionView[1][1],
		_projectionView[2][3] + _projectionView[2][1],
		_projectionView[3][3] + _projectionView[3][1]);
	// far
	_frustum[4] = glm::vec4(_projectionView[0][3] - _projectionView[0][2],
		_projectionView[1][3] - _projectionView[1][2],
		_projectionView[2][3] - _projectionView[2][2],
		_projectionView[3][3] - _projectionView[3][2]);
	// near
	_frustum[5] = glm::vec4(_projectionView[0][3] + _projectionView[0][2],
		_projectionView[1][3] + _projectionView[1][2],
		_projectionView[2][3] + _projectionView[2][2],
		_projectionView[3][3] + _projectionView[3][2]);
	// plane normalisation, based on length of normal
	for (int i = 0; i < 6; i++) {
		float d = glm::length(glm::vec3(_frustum[i]));
		_frustum[i] /= d;
	}
}
