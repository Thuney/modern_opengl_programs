#include "CartesianPlane.h"
#include "glm/gtc/quaternion.hpp"

CartesianPlane::CartesianPlane(glm::vec3 normal_vector, float horizontal_range, float vertical_range, float spacing, Shader* shader, ShaderColor color)
{
	this->cartesian_plane_shader = shader;

	this->normal_vector = glm::normalize(normal_vector);	

	this->horizontal_range = horizontal_range;
	this->vertical_range = vertical_range;

	this->spacing = spacing;

	this->color = color;

	generate_plane_data();
}

CartesianPlane::~CartesianPlane()
{
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

void CartesianPlane::draw()
{
	set_shader_color(color);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glDrawArrays(GL_LINES, 0, this->points);

	glBindVertexArray(GL_NONE);
}

void add_vertical_line(float** cur_vertex_element, float x, float y_extent)
{
	*((*cur_vertex_element)++) = x;
	*((*cur_vertex_element)++) = 0.0f;
	*((*cur_vertex_element)++) = (y_extent);

	*((*cur_vertex_element)++) = x;
	*((*cur_vertex_element)++) = 0.0f;
	*((*cur_vertex_element)++) = (-y_extent);
}

void add_horizontal_line(float** cur_vertex_element, float y, float x_extent)
{
	*((*cur_vertex_element)++) = (x_extent);
	*((*cur_vertex_element)++) = 0.0f;
	*((*cur_vertex_element)++) = y;

	*((*cur_vertex_element)++) = (-x_extent);
	*((*cur_vertex_element)++) = 0.0f;
	*((*cur_vertex_element)++) = y;
}

void CartesianPlane::generate_plane_data()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	int num_v_lines = (2 * (this->vertical_range)) / (this->spacing);
	int num_h_lines = (2 * (this->horizontal_range)) / (this->spacing);
	int num_lines = num_v_lines + num_h_lines + 2;

	this->points = 2 * num_lines;

	//3 elements per point, 2 points per line
	int num_elements = 3 * 2 * (num_lines);
	int plane_data_size  = sizeof(float) * (num_elements);

	this->plane_position_data = new float[(num_elements)];
	float* cur_vertex_element = this->plane_position_data;

	for (float x = -this->horizontal_range; x <= this->horizontal_range; x += spacing)
	{
		add_vertical_line(&cur_vertex_element, x, this->vertical_range);
	}

	for (float y = -this->vertical_range; y <= this->vertical_range; y += spacing)
	{
		add_horizontal_line(&cur_vertex_element, y, this->horizontal_range);
	}

	glm::vec3 starting_normal(0.0f, 1.0f, 0.0f);
	//this->normal_vector;

	glm::vec3 rotation_axis = glm::cross(starting_normal, this->normal_vector);

	float rotation_angle_rad = glm::acos(glm::dot(starting_normal, this->normal_vector));

	glm::quat quatRot = glm::angleAxis(rotation_angle_rad, rotation_axis);
	glm::mat4x4 matRot = glm::mat4_cast(quatRot);

	for (int i = 0; i < 2 * num_lines; i++)
	{
		float x = *(this->plane_position_data + (3 * i));
		float y = *(this->plane_position_data + (3 * i + 1));
		float z = *(this->plane_position_data + (3 * i + 2));
		glm::vec3 point(x, y, z);

		glm::vec3 rotated_point = glm::vec3(matRot * glm::vec4(point, 1.0f));

		*(this->plane_position_data + (3 * i)) = rotated_point.x;
		*(this->plane_position_data + (3 * i + 1)) = rotated_point.y;
		*(this->plane_position_data + (3 * i + 2)) = rotated_point.z;
	}

	glBufferData(GL_ARRAY_BUFFER, plane_data_size, this->plane_position_data, GL_STATIC_DRAW);

	int plane_attribute_stride = 3 * sizeof(float);

	GLint plane_position_data = glGetAttribLocation(this->cartesian_plane_shader->ID, "position");
	glVertexAttribPointer(plane_position_data, 3, GL_FLOAT, GL_FALSE, plane_attribute_stride, 0);
	glEnableVertexAttribArray(plane_position_data);

	glBindVertexArray(GL_NONE);
}

void CartesianPlane::set_shader_color(ShaderColor color)
{
	this->cartesian_plane_shader->setColorVec3("color", color);
}
