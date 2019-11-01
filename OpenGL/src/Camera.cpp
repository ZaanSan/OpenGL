#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Position(position),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	WorldUp(up),
	Yaw(yaw),
	Pitch(pitch),
	MovementSpeed(CAM_MOV_SPEED),
	RotationSpeed(CAM_ROT_SPEED),
	Zoom(ZOOM)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Position(glm::vec3(posX, posY, posZ)),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	WorldUp(glm::vec3(upX, upY, upZ)),
	Yaw(yaw),
	Pitch(pitch),
	MovementSpeed(CAM_MOV_SPEED),
	RotationSpeed(CAM_ROT_SPEED),
	Zoom(ZOOM)
{
	updateCameraVectors();
}

Camera::~Camera() = default;

static glm::mat4 LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	glm::vec3 zAxis = glm::normalize(position - target);
	glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);

	glm::mat4 rotation(1.0f);
	glm::mat4 translation(1.0f);

	// In glm we access elements as mat[col][row] due to column-major layout
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;

	rotation[0][0] = xAxis.x;
	rotation[1][0] = xAxis.y;
	rotation[2][0] = xAxis.z;
	rotation[0][1] = yAxis.x;
	rotation[1][1] = yAxis.y;
	rotation[2][1] = yAxis.z;
	rotation[0][2] = zAxis.x;
	rotation[1][2] = zAxis.y;
	rotation[2][2] = zAxis.z;

	return rotation * translation;
}

glm::mat4 Camera::GetViewMatrix() const
{
	//return glm::lookAt(Position, Position + Front, WorldUp);
	return LookAt(Position, Position + Front, WorldUp);
}

void Camera::ProcessMovement(CameraMovement movement, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	switch (movement)
	{
	case FORWARD:
		Position += Front * velocity;
		break;
	case BACKWARD:
		Position -= Front * velocity;
		break;
	case RIGHT:
		Position += Right * velocity;
		break;
	case LEFT:
		Position -= Right * velocity;
		break;
	case PITCH_UP:
		Pitch += RotationSpeed * deltaTime;
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		break;
	case PITCH_DOWN:
		Pitch -= RotationSpeed * deltaTime;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		break;
	case YAW_RIGHT:
		Yaw += RotationSpeed * deltaTime;
		break;
	case YAW_LEFT:
		Yaw -= RotationSpeed * deltaTime;
		break;
	}
	updateCameraVectors();
}

void Camera::ProcessZoom(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	else if (Zoom >= 45.0f)
		Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
