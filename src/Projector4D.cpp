
#include "Projector4D.h"

#include <glm/gtc/constants.hpp>
#include <glm/ext/vector_transform.hpp>

#include <math.h>

Projector4D::Projector4D()
	: m_principalViewDir(E_PVD_NEGATIVE_W)
{
	resetProjectorFrame();
}

void Projector4D::resetProjectorFrame()
{
	m_parallaxDirection = glm::vec3(0.0);
	m_parallaxAngle = 0.0f;
	
	// By default, we project along the w axis, looking down -w
	m_frameMatrix = glm::mat4(1.0f); //Load Identity
	switch (m_principalViewDir)
	{
	// These cases correspond to rotating -w to the target view vector:
	case E_PVD_POSITIVE_X:
		m_frameMatrix[0] = -m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		break;
	case E_PVD_NEGATIVE_X:
		m_frameMatrix[0] = m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
		break;
	case E_PVD_POSITIVE_Y:
		m_frameMatrix[1] = -m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		break;
	case E_PVD_NEGATIVE_Y:
		m_frameMatrix[1] = m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
		break;
	case E_PVD_POSITIVE_Z:
		m_frameMatrix[2] = -m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		break;
	case E_PVD_NEGATIVE_Z:
		m_frameMatrix[2] = m_frameMatrix[3];
		m_frameMatrix[3] = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		break;
	// This case correspond to rotating -w to -x and then rotating -x to +w:
	case E_PVD_POSITIVE_W:
		m_frameMatrix[0] = -m_frameMatrix[0];
		m_frameMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);
		break;
	case E_PVD_NEGATIVE_W:
	default:
		break;
	}
	m_frameMatrix = glm::transpose(m_frameMatrix);

	m_viewMatrix = glm::mat4(1.0f); //Load Identity
}

void Projector4D::setProjectionParams(glm::vec3 parallaxDirection, float parallaxAngle)
{
	m_parallaxDirection = parallaxDirection;
	m_parallaxAngle = parallaxAngle;
}

void Projector4D::setPrincipalViewDirection(EPrincipalViewDirection projDir)
{
	if (projDir != m_principalViewDir)
	{
		m_principalViewDir = projDir;
		resetProjectorFrame();
	}
}

const glm::mat4& Projector4D::GetFrameMatrix()
{
	return m_frameMatrix;
}

glm::vec4 Projector4D::GetViewDirection()
{
	return glm::vec4(m_frameMatrix[0][3], m_frameMatrix[1][3], m_frameMatrix[2][3], m_frameMatrix[3][3]);
}

glm::vec4 Projector4D::Project(glm::vec4 vector)
{
	return m_frameMatrix * vector;
}

glm::vec4 Projector4D::UnProject(glm::vec4 vector)
{
	return vector * m_frameMatrix;
}