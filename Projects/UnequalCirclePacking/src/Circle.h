#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glew.h>

#include "Shader.h"

class Circle
{
public:

	//A shared shader instance used to color all circles
	static Shader* circle_shader;

	//Circle parameters
	float x, y, radius;

	Circle(const float x = 0.0f, const float y = 0.0f, const float radius = 1.0f);
	~Circle();

	//Setter functions
	//These adjust both circle parameters and model_matrix
	void set_pos(const float x, const float y);
	void set_radius(const float radius);

	//Options for drawing the circle outlined or filled
	void draw_outline(const ShaderColor color);
	void draw(const ShaderColor color);

private:
	//OpenGL components
	static GLuint vao, vbo;

	static unsigned int instances;

	glm::mat4 model_matrix;

	//Shared circle vertex data
	//Stores data for vertices of a circle with radius 1 (unit circle)
	static float* circle_data;

	static const int num_triangles = 32;
	static const int num_vertices = num_triangles + 2;

	void setup_gl();
	void generate_circle_data();

};

#endif