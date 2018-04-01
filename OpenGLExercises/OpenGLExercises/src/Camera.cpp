#include "Camera.h"

Camera::Camera()
{
	m_position = 3.0f * Z_NORM_VECTOR;
	m_frontDirVec = -Z_NORM_VECTOR;
	m_upDirVec = Y_NORM_VECTOR;

	m_yawAngle = -SQUARED_DEGREE;
	m_pitchAngle = 0.0f;
	m_fieldOfView = SQUARED_DEGREE / 2;

	m_cameraSpeed = 0.05f;
	m_nearPlane = 0.1f;
	m_farPlane = 100.0f;
}

glm::mat4 Camera::GetViewMat()
{
	m_viewMat = glm::lookAt(m_position, m_position + m_frontDirVec, m_upDirVec);
	return m_viewMat;
}

glm::mat4 Camera::GetProjectionMat()
{
	m_projectionMat = glm::perspective(glm::radians(m_fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_nearPlane, m_farPlane);
	return m_projectionMat;
}

void Camera::MoveAlongDirection(float distance, glm::vec3 direction)
{
	m_position += distance * direction;
}

glm::vec3 Camera::GetFrontCrossDir()
{
	return glm::normalize(glm::cross(m_frontDirVec, m_upDirVec));
}

void Camera::UpdateAngles(float deltaYaw, float deltaPitch)
{
	m_yawAngle += deltaYaw;
	m_pitchAngle += deltaPitch;
	if (m_pitchAngle > MAX_CAMERA_PITCH)
	{
		m_pitchAngle = MAX_CAMERA_PITCH;
	}
	else if (m_pitchAngle < MIN_CAMERA_PITCH)
	{
		m_pitchAngle = MIN_CAMERA_PITCH;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(m_pitchAngle))  * cos(glm::radians(m_yawAngle));
	front.y = sin(glm::radians(m_pitchAngle));
	front.z = cos(glm::radians(m_pitchAngle)) * sin(glm::radians(m_yawAngle));
	m_frontDirVec = glm::normalize(front);
}

void Camera::UpdateFOV(float deltaAngle)
{
	if (m_fieldOfView >= NARROW_FOV && m_fieldOfView <= LARGEST_FOV)
		m_fieldOfView -= deltaAngle;
	else if (m_fieldOfView < NARROW_FOV)
		m_fieldOfView = NARROW_FOV;
	else if (m_fieldOfView > LARGEST_FOV)
		m_fieldOfView = LARGEST_FOV;
}