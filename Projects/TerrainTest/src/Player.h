#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"

#include <GL/glew.h>

#include "Camera.h"
#include "Shader.h"

class Player
{
public:

	Player(glm::vec3 position, float height, Shader* shader);
	~Player();

	void draw();
	glm::mat4 getViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void process_keyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void process_mouse_scroll(float yoffset);

private:

	float width = 15.0f;

	glm::mat4 model = glm::mat4(1.0f);

	Camera* camera;
	Shader* shader;

	glm::vec3 position;
	float height, eye_height;

	GLuint avatar_vao, avatar_vbo;
	float* bounding_box_data;
	int num_vertices;

	void create_bounding_box_data();
};

#endif