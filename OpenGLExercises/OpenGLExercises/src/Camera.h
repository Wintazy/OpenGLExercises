#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Define.h"

const float CAMERA_SPEED = 2.0f;
const float MAX_CAMERA_PITCH = 89.9f;
const float MIN_CAMERA_PITCH = -MAX_CAMERA_PITCH;
const float LARGEST_FOV = 45.0f;
const float NARROW_FOV = 1.0f;

class Camera
{
public:
	Camera();
	glm::mat4 GetViewMat();
	glm::mat4 GetProjectionMat();
	glm::vec3 GetFrontCrossDir();
	glm::vec3 GetFrontDir() { return m_frontDirVec; }
	void MoveAlongDirection(float distance, glm::vec3 direction);
	void UpdateAngles(float deltaYaw, float deltaPitch);
	void UpdateFOV(float deltaAngle);
private:
	float m_yawAngle, m_pitchAngle, m_fieldOfView;
	float m_cameraSpeed;
	float m_nearPlane, m_farPlane;
	glm::vec3 m_position, m_frontDirVec, m_upDirVec;
	glm::mat4 m_viewMat, m_projectionMat;
};
#endif