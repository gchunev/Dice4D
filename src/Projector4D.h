#ifndef PROJECTOR_4D_H
#define PROJECTOR_4D_H

#include <glm/glm.hpp>

class Projector4D
{
public:
	enum EPrincipalViewDirection
	{
		E_PVD_POSITIVE_X,
		E_PVD_NEGATIVE_X,
		E_PVD_POSITIVE_Y,
		E_PVD_NEGATIVE_Y,
		E_PVD_POSITIVE_Z,
		E_PVD_NEGATIVE_Z,
		E_PVD_POSITIVE_W,
		E_PVD_NEGATIVE_W,
	};

public:
	Projector4D();

	void				resetProjectorFrame();

	void				setProjectionParams(glm::vec3 parallaxDirection, float parallaxAngle);
	void				setPrincipalViewDirection(EPrincipalViewDirection viewDir);

	glm::vec4			GetViewDirection();
	const glm::mat4&	GetFrameMatrix();

	glm::vec4			Project(glm::vec4 vector);
	glm::vec4			UnProject(glm::vec4 vector);

private:
	EPrincipalViewDirection	m_principalViewDir;

	glm::vec3	m_parallaxDirection;
	float		m_parallaxAngle;

	glm::mat4	m_frameMatrix;

	glm::mat4	m_viewMatrix;
};

#endif