#ifndef CAMERA_H
#define CAMERA_H

/*
 * This camera class is adapted from the class provided by the tutorial on learnopengl.com
 *
 * Main change is the ability to change camera modes between grounded and free-floating
 */

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Mode {
	//STATIONARY,
	FREE_FLOATING,
	GROUNDED
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera_Mode Mode;

	// Constructor with vectors
	Camera(Camera_Mode mode = FREE_FLOATING, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(Camera_Mode mode = FREE_FLOATING, float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float yaw = YAW, float pitch = PITCH);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	void set_position(glm::vec3 position);

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void process_keyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void process_mouse_scroll(float yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void update_camera_vectors();
};

#endif