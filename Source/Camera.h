#pragma once
#include <iostream>
#include <vector>
#include "Vec4f.h"
#include "Mat4x4f.h"
#define DEG_TO_RAD 0.01745329251f

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	inline Camera(const Vec4f& position = Vec4f(0.0f, 0.0f, 0.0f, 0.0f), const Vec4f& up = Vec4f(0.0f, 1.0f, 0.0f, 0.0f), const float& yaw = YAW, const float& pitch = PITCH) {
		pos = position;
		worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
		firstTime = true;
	}
	inline Mat4x4f getViewMatrix() const{ return camView; };
	inline Vec4f getPosition() const { return pos; };
	inline float getYaw() const { return yaw; }
	inline float getPitch() const { return pitch; }
	inline void setPosition(Vec4f newPos) { pos = newPos; };
	inline void setYaw(float newYaw) { yaw = newYaw; }
	inline void setPitch(float newPitch) { pitch = newPitch; }
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float x, float y, float deltaTime, bool constrainPitch = true);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);


	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
	static Camera& getInstance() { return cameraInstance; }

	void reset();
	void update();

private:
	Mat4x4f camView;
	// Camera Attributes
	Vec4f pos;
	Vec4f front=Vec4f(0.0f, 0.0f,-1.0f,0.0f);
	Vec4f up;
	Vec4f right;
	Vec4f worldUp;

	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed= SPEED;
	float mouseSensitivity=SENSITIVITY;
	float zoom= ZOOM;


	float lastX;
	float lastY;
	bool firstTime;

	static Camera cameraInstance;
};