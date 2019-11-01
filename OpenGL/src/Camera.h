#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	PITCH_UP,
	PITCH_DOWN,
	YAW_LEFT,
	YAW_RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float CAM_MOV_SPEED = 10.0f;
const float CAM_ROT_SPEED = 100.0f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// Camera attributes
	glm::vec3	Position;
	glm::vec3	Front;
	glm::vec3	Up;
	glm::vec3	Right;
	glm::vec3	WorldUp;

	// Euler angles
	float		Yaw;
	float		Pitch;

	// Camera settings
	float		MovementSpeed;
	float		RotationSpeed;
	float		Zoom;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

	glm::mat4	GetViewMatrix() const;
	void		ProcessMovement(CameraMovement direction, float deltaTime);
	void		ProcessZoom(float yoffset);

private:
	void updateCameraVectors();
};

