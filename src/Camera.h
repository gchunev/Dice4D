#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	enum ECameraMovementType
	{
		E_CMT_FREE_LOOK,
		E_CMT_FLY_THROUGH
	};

public:
	Camera();

	const glm::mat4x4*	getViewMatrix();
	const glm::mat4x4*	getProjMatrix();

	float				getNearClipDistance();
	float				getFarClipDistance();

	glm::vec3 			getPosition();

	void				resetCameraFrame();
	void				translateCamera(float dx, float dy, float dz);
	void				setMovementRightDirection(const glm::vec3& rightVec);
	const glm::vec3&	getMovementRightDirection();
	void				addPitchRotation(float deg);
	void				setMovementUpDirection(const glm::vec3& upVec);
	const glm::vec3&	getMovementUpDirection();
	void				addYawRotation(float deg);

	void				setProjectionParams(float FoV, float aspectRatio);
	void				setCameraMovementType(ECameraMovementType camType);

	void				setFollowTarget(const glm::vec3& pos);

private:
	ECameraMovementType		m_movementType;
	// yaw and pitch angles
	float		m_yaw;
	float		m_pitch;

	float		m_FoV;
	float		m_aspectRatio;

	float		m_nearClipDistance;
	float		m_farClipDistance;
	
	glm::vec3	m_position;

	glm::mat4x4	m_frameMatrix; // Stores as colums |F U R 0| the OpenGL forward, up, and right vectors.
	glm::vec3	m_movementRight; // Local space right
	glm::vec3	m_movementUp; // World space up

	glm::mat4x4	m_viewMatrix;
	glm::mat4x4	m_projMatrix;

	glm::vec3 m_FollowTarget;
};

#endif