#include "DEBUG_NEW_LEAK_DETECT.h"
#include "CameraBase.h"

#ifdef _DEBUG
#include <iostream>
#endif
#include "ApplicationBase.h"

//CameraBase* GLE::MAIN_CAM = nullptr;

CameraBase::CameraBase()
	: m_projectionTransform(1)
	, m_FOV(0.25f)
	, m_near(0.1f)
	, m_far(1000.0f)
	, m_transform()
{
	m_aspectRatio = (float)GLE::APP->GetWindowWidth() / (float)GLE::APP->GetWindowHeight();
}

CameraBase::~CameraBase()
{
}

void CameraBase::SetAsMain()
{
	SetAsMain(this);
}

void CameraBase::SetAsMain(CameraBase * _camera)
{
	//if (GLE::MAIN_CAM != nullptr)
	//	GLE::MAIN_CAM->OnLostFocus();

	//GLE::MAIN_CAM = _camera;
	//GLE::MAIN_CAM->OnGainFocus();
}

//void CameraBase::LookAt(const glm::vec3 & _eye, const glm::vec3 & _center, const glm::vec3 & _worldUp)
//{
//	//m_direction = glm::normalize(_center - _eye);
//	// m_viewTransform = glm::lookAt(_eye, _center, _worldUp);
//	// UpdateWorldFromView();
//}
//
//void CameraBase::LookAt(const glm::vec3 & _target)
//{
//	// m_viewTransform = glm::lookAt(m_transform.TransformPoint(m_transform.LocalPosition()), _target, Vector3::up);
//}

void CameraBase::SetPerspective(float _FOV, float _aspectRatio, float _nearDST, float _farDST)
{
	m_FOV = _FOV;
	m_aspectRatio = _aspectRatio;
	m_near = _nearDST;
	m_far = _farDST;
	CalculatePerspective();
}

void CameraBase::SetFOV(const float _FOV)
{
	m_FOV = _FOV;
	CalculatePerspective();
}

void CameraBase::SetAspectRatio(const float _aspectRatio)
{
	m_aspectRatio = _aspectRatio;
	CalculatePerspective();
}

void CameraBase::SetNearFar(const float _nearDST, const float _farDST)
{
	m_near = _nearDST;
	m_far = _farDST;
	CalculatePerspective();
}

void CameraBase::CalculatePerspective()
{
	m_projectionTransform = glm::perspective(m_FOV, m_aspectRatio, m_near, m_far);
}

#ifdef _DEBUG
void CameraBase::PrintMat4(const glm::mat4 & _transform) const
{
	std::cout
		<< " X: "
		<< _transform[0][0] << ','
		<< _transform[0][1] << ','
		<< _transform[0][2] << ','
		<< _transform[0][3] << ','
		<< " Y: "
		<< _transform[1][0] << ','
		<< _transform[1][1] << ','
		<< _transform[1][2] << ','
		<< _transform[1][3] << ','
		<< " Z: "
		<< _transform[2][0] << ','
		<< _transform[2][1] << ','
		<< _transform[2][2] << ','
		<< _transform[2][3] << ','
		<< " W: "
		<< _transform[3][0] << ','
		<< _transform[3][1] << ','
		<< _transform[3][2] << ','
		<< _transform[3][3] << ','
		<< std::endl;
}
#endif
