#ifndef CARTESIAN_PLANE_H
#define CARTESIAN_PLANE_H

#include <GL/glew.h>

#include "glm/glm.hpp"

#include "Shader.h"

class CartesianPlane
{
public:

	CartesianPlane(glm::vec3 normal_vector, float horizontal_range, float vertical_range, float spacing, Shader* shader, ShaderColor color);
	~CartesianPlane();

	void draw();

	Shader* cartesian_plane_shader;

private:

	glm::vec3 normal_vector;

	float horizontal_range, vertical_range, spacing;
	int points;

	ShaderColor color;

	GLuint vao, vbo;
	float* plane_position_data;

	void generate_plane_data();
	void set_shader_color(ShaderColor color);

};

#endif