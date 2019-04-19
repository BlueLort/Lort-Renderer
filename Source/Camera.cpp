#include "Camera.h"


Camera Camera::cameraInstance = Camera();

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		pos -= front * velocity;
	if (direction == BACKWARD)
		pos += front * velocity;
	if (direction == LEFT)
		pos -= right * velocity;
	if (direction == RIGHT)
		pos += right * velocity;

	updateCameraVectors();
	

}

void Camera::ProcessMouseMovement(float x, float y, float deltaTime, bool constrainPitch)
{
	if (firstTime)
	{
		lastX = x;
		lastY = y;
		firstTime = false;
	}

	float xoffset = (x - lastX) * mouseSensitivity;
	float yoffset = (lastY - y) * mouseSensitivity;

	yaw += xoffset * deltaTime;
	pitch += yoffset * deltaTime;

	lastX = x;
	lastY = y;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw -= xoffset;
	pitch -= yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	Vec4f Front;
	float radYaw = yaw * DEG_TO_RAD;
	float radPitch = pitch * DEG_TO_RAD;
	Front.insert(cos(radYaw) * cos(radPitch),0);
	Front.insert(sin(radPitch),1);
	Front.insert(sin(radYaw) * cos(radPitch),2);
	front = Front.normalized();

	right = cross(front, worldUp).normalized();
	up = cross(right, front).normalized();

	update();
}

void Camera::reset()
{
	firstTime = true;
}

void Camera::update()
{
	camView = Mat4x4f::getLookAt(pos, pos + front, up);
}
