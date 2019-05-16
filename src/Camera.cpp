
#include "Camera.h"

#include <glm/gtc/constants.hpp>

#include <math.h>

Camera::Camera()
	: m_movementType(E_CMT_FLY_THROUGH)
	, m_movementRight(1.0f, 0.0f, 0.0f)
	, m_movementUp(0.0f, 1.0f, 0.0f)
	, m_FollowTarget(0.f)
{
	resetCameraFrame();
}

void Camera::resetCameraFrame()
{
	m_yaw = 0.0;
	m_pitch = 0.0;
	m_FoV = 45.0f;
	m_nearClipDistance = 0.1f;
	m_farClipDistance = 200.0f;
	m_aspectRatio = 1.0f;
	m_position = glm::vec3(0.0f);

	//Local Right Direction:
	m_frameMatrix[0][0] = 1.0f;
	m_frameMatrix[0][1] = 0.0f;
	m_frameMatrix[0][2] = 0.0f;
	m_frameMatrix[0][3] = 0.0f;

	//Local Up Direction:
	m_frameMatrix[1][0] = 0.0f;
	m_frameMatrix[1][1] = 1.0f;
	m_frameMatrix[1][2] = 0.0f;
	m_frameMatrix[1][3] = 0.0f;

	//Local Forward Direction:
	m_frameMatrix[2][0] = 0.0f;
	m_frameMatrix[2][1] = 0.0f;
	m_frameMatrix[2][2] = -1.0f;
	m_frameMatrix[2][3] = 0.0f;

	//Unused:
	m_frameMatrix[3][0] = 0.0f;
	m_frameMatrix[3][1] = 0.0f;
	m_frameMatrix[3][2] = 0.0f;
	m_frameMatrix[3][3] = 1.0f;

	m_viewMatrix = glm::mat4(1.0f); //Load Identity
	m_projMatrix = glm::perspective(m_FoV, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}

void Camera::setProjectionParams(float FoV, float aspectRatio)
{
	m_FoV = FoV;
	m_aspectRatio = aspectRatio;
	m_projMatrix = glm::perspective(m_FoV, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}

void Camera::setCameraMovementType(ECameraMovementType camType)
{
	m_movementType = camType;
}

void Camera::setFollowTarget(const glm::vec3& pos)
{
	m_FollowTarget = pos;
}

const glm::mat4x4* Camera::getViewMatrix()
{
	glm::mat4 rotMat(1.0f); //Load Identity
	glm::mat4 rotMatX = glm::rotate(glm::mat4(1.0f), m_pitch, m_movementRight);
	glm::mat4 rotMatY = glm::rotate(glm::mat4(1.0f), m_yaw, m_movementUp);
	rotMat = rotMatY * rotMatX;
	
	glm::mat4 cameraMatrix = rotMat * m_frameMatrix;
	
	if (m_FollowTarget == glm::vec3(0.f))
	{
		m_viewMatrix = glm::lookAt(m_position,
			glm::vec3(m_position.x + cameraMatrix[2][0], m_position.y + cameraMatrix[2][1], m_position.z + cameraMatrix[2][2]),
			glm::vec3(cameraMatrix[1][0], cameraMatrix[1][1], cameraMatrix[1][2]));
	}
	else
	{
		m_viewMatrix = glm::lookAt(m_position,
			m_FollowTarget,
			glm::vec3(cameraMatrix[1][0], cameraMatrix[1][1], cameraMatrix[1][2]));
	}
	
	return &m_viewMatrix;
}

const glm::mat4x4* Camera::getProjMatrix()
{
	return &m_projMatrix;
}

glm::vec3 Camera::getPosition()
{
	return m_position;
}

float Camera::getNearClipDistance()
{
	return m_nearClipDistance;
}

float Camera::getFarClipDistance()
{
	return m_farClipDistance;
}

void Camera::translateCamera(float dx, float dy, float dz)
{
	glm::vec4 dPos(dx, dy, dz, 0.0f);

	glm::mat4 rotMat;
	switch (m_movementType)
	{
	case E_CMT_FLY_THROUGH:
		{
			glm::mat4 rotMatX = glm::rotate(glm::mat4(1.0f), m_pitch, m_movementRight);
			glm::mat4 rotMatY = glm::rotate(glm::mat4(1.0f), m_yaw, m_movementUp);
			rotMat = rotMatY * rotMatX;
		}
		break;
	case E_CMT_FREE_LOOK:
		{
			glm::mat4 rotMatY = glm::rotate(glm::mat4(1.0f), m_yaw, m_movementUp);
			rotMat = rotMatY;
		}
		break;
	default:
		rotMat = glm::mat4(1.0f);
	}
	dPos = rotMat * dPos;

	m_position += glm::vec3(dPos);
}

void Camera::setMovementRightDirection(const glm::vec3& rightVec)
{
	m_movementRight = rightVec;
}

const glm::vec3& Camera::getMovementRightDirection()
{
	return m_movementRight;
}

void Camera::setMovementUpDirection(const glm::vec3& upVec)
{
	m_movementUp = upVec;
}

const glm::vec3& Camera::getMovementUpDirection()
{
	return m_movementUp;
}

void Camera::addPitchRotation(float deg)
{
	m_pitch =  fmod( m_pitch + deg, 360.0f );
}

void Camera::addYawRotation(float deg)
{
	m_yaw =  fmod( m_yaw + deg, 360.0f );
}
