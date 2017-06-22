#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "Transform.h"

#include <glm\gtx\transform.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm\ext.hpp>

static const glm::vec3 Vector3::forward = glm::vec3(0, 0, 1);
static const glm::vec3 Vector3::backward = glm::vec3(0, 0, -1);
static const glm::vec3 Vector3::up = glm::vec3(0, 1, 0);
static const glm::vec3 Vector3::down = glm::vec3(0, -1, 0);
static const glm::vec3 Vector3::right = glm::vec3(1, 0, 0);
static const glm::vec3 Vector3::left = glm::vec3(-1, 0, 0);
static const glm::vec3 Vector3::one = glm::vec3(1, 1, 1);
static const glm::vec3 Vector3::zero = glm::vec3(0, 0, 0);

Transform::Transform()
	: scale(1), forward(), position()
{
}

Transform::~Transform()
{
}

glm::mat4 Transform::GetRotationMatrix()
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

glm::mat4 Transform::GetLocalMatrix()
{
	glm::mat4 local = glm::mat4();
	// translate
	local = glm::translate(local, position);
	// rotate
	local = local * GetRotationMatrix();
	// scale
	local = glm::scale(local, scale);
	return local;
}
