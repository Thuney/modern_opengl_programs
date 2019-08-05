#include "Circle.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PI 3.1415926535

Shader* Circle::circle_shader = nullptr;

unsigned int Circle::instances = 0;
static bool first_circle = true;

float* Circle::circle_data = nullptr;
GLuint Circle::vao = NULL;
GLuint Circle::vbo = NULL;

Circle::Circle(const float x, const float y, const float radius)
{
	this->x = x;
	this->y = y;
	this->radius = radius;

	//Adjust the model matrix to scale and translate our unit circle according to the specified parameters
	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, glm::vec3(x, y, 0));
	this->model_matrix = glm::scale(this->model_matrix, glm::vec3(this->radius, this->radius, 1.0f));

	if (first_circle)
	{
		this->setup_gl();
		first_circle = false;
	}
	this->instances++;
}

Circle::~Circle()
{
	//Circle instance managing. Delete circle data if all circles have been destroyed
	this->instances--;
	if (this->instances == 0)
	{
		delete this->circle_data;
		first_circle = true;
	}

}

void Circle::set_pos(const float x, const float y)
{
	//Set position and adjust model matrix
	this->x = x;
	this->y = y;

	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, glm::vec3(this->x, this->y, 0));
	this->model_matrix = glm::scale(this->model_matrix, glm::vec3(this->radius, this->radius, 1.0f));
}

void Circle::set_radius(const float radius)
{
	//Set radius and adjust model matrix
	this->radius = radius;

	this->model_matrix = glm::mat4(1.0f);
	this->model_matrix = glm::translate(this->model_matrix, glm::vec3(this->x, this->y, 0));
	this->model_matrix = glm::scale(this->model_matrix, glm::vec3(this->radius, this->radius, 1.0f));
}

void Circle::draw_outline(ShaderColor color)
{
	//Draw outline, AKA draw as line loop and omit origin

	this->circle_shader->use();
	this->circle_shader->setColorVec3("color", color);
	this->circle_shader->setMat4("model", this->model_matrix);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glDrawArrays(GL_LINE_LOOP, 1, this->num_vertices - 1);

	glBindVertexArray(GL_NONE);
}

void Circle::draw(ShaderColor color)
{
	//Draw filled circle, AKA draw as a triangle fan

	this->circle_shader->use();
	this->circle_shader->setColorVec3("color", color);
	this->circle_shader->setMat4("model", this->model_matrix);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glDrawArrays(GL_TRIANGLE_FAN, 0, this->num_vertices);

	this->circle_shader->setColorVec3("color", BLACK);
	glDrawArrays(GL_LINE_LOOP, 1, this->num_vertices - 1);

	//Commented code for creating an inner circle for utilities
	//this->circle_shader->setColorVec3("color", GRAY);
	//glm::mat4 shrunk_model_matrix = glm::scale(model_matrix, glm::vec3(0.75, 0.75, 0));
	//this->circle_shader->setMat4("model", shrunk_model_matrix);

	//glDrawArrays(GL_TRIANGLE_FAN, 0, this->num_vertices);

	glBindVertexArray(GL_NONE);

}

void Circle::setup_gl()
{
	//Standard GL setup code for vertex array and buffer objects

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	generate_circle_data();

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * (2 * this->num_vertices)), this->circle_data, GL_STATIC_DRAW);

	int circle_attribute_stride = 2 * sizeof(float);

	GLint circle_xy_data = glGetAttribLocation(this->circle_shader->ID, "position");
	glVertexAttribPointer(circle_xy_data, 2, GL_FLOAT, GL_FALSE, circle_attribute_stride, 0);
	glEnableVertexAttribArray(circle_xy_data);

	glBindVertexArray(GL_NONE);
}

void Circle::generate_circle_data()
{
	//Generate unit circle data with resolution specified by the member variable num_triangles

	this->circle_data = new float[2 * this->num_vertices];
	float* cur_data = this->circle_data;

	//Origin
	*(cur_data) = 0;
	cur_data++;
	*(cur_data) = 0;
	cur_data++;

	float theta_step = (2.0f * PI) / ((float) this->num_triangles);

	//Step around circle
	for (int vertex_number = 0; vertex_number <= this->num_triangles; vertex_number++)
	{
		float angle_radians = ((float)vertex_number) * theta_step;

		*(cur_data) = cos(angle_radians);
		cur_data++;
		*(cur_data) = sin(angle_radians);
		cur_data++;
	}
}
