#include "Player.h"

Player::Player(glm::vec3 position, float height, Shader* shader)
{
	this->position = position;
	this->height = height;
	this->eye_height = ((163.0/174.0)*height);

	this->camera = new Camera(GROUNDED, glm::vec3(position.x, position.y + this->eye_height, position.z));
	this->shader = shader;

	this->create_bounding_box_data();
}

Player::~Player()
{
	glDeleteVertexArrays(1, &this->avatar_vao);
	glDeleteBuffers(1, &this->avatar_vbo);
}

void Player::draw()
{
	this->shader->use();
	this->shader->setColorVec3("color", BLACK);

	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->position.x, this->position.y, this->position.z));
	this->model = glm::rotate(this->model, glm::radians(-this->camera->Yaw), glm::vec3(0.0f, 1.0f, 0.0f));

	this->shader->setMat4("model", this->model);

	glBindVertexArray(this->avatar_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->avatar_vbo);

	glDrawArrays(GL_LINES, 0, this->num_vertices);

	glBindVertexArray(GL_NONE);
}

glm::mat4 Player::getViewMatrix()
{
	return this->camera->getViewMatrix();
}

void Player::process_keyboard(Camera_Movement direction, float deltaTime)
{
	this->camera->process_keyboard(direction, deltaTime);

	this->position = glm::vec3(this->camera->Position.x, this->camera->Position.y - this->eye_height, this->camera->Position.z);
}

void Player::process_mouse_movement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	this->camera->process_mouse_movement(xoffset, yoffset, constrainPitch);
}

void Player::process_mouse_scroll(float yoffset)
{
	this->camera->process_mouse_scroll(yoffset);
}

void add_line(float** current_element, float x1, float y1, float z1, float x2, float y2, float z2)
{
	*((*current_element)++) = x1;
	*((*current_element)++) = y1;
	*((*current_element)++) = z1;

	*((*current_element)++) = x2;
	*((*current_element)++) = y2;
	*((*current_element)++) = z2;
}

void Player::create_bounding_box_data()
{
	glGenVertexArrays(1, &this->avatar_vao);
	glBindVertexArray(this->avatar_vao);

	glGenBuffers(1, &this->avatar_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->avatar_vbo);

	this->num_vertices = 2 * 12;
	int num_elements = 3 * this->num_vertices;
	int elements_size = sizeof(float) * num_elements;

	this->bounding_box_data = new float[(num_elements)];
	float* cur_element = this->bounding_box_data;

	float radius = width / 2;

	add_line(&cur_element, -radius, 0.0f, radius, radius, 0.0f, radius);
	add_line(&cur_element, radius, 0.0f, radius, radius, 0.0f, -radius);
	add_line(&cur_element, radius, 0.0f, -radius, -radius, 0.0f, -radius);
	add_line(&cur_element, -radius, 0.0f, -radius, -radius, 0.0f, radius);

	add_line(&cur_element, -radius, this->height, radius, radius, this->height, radius);
	add_line(&cur_element, radius, this->height, radius, radius, this->height, -radius);
	add_line(&cur_element, radius, this->height, -radius, -radius, this->height, -radius);
	add_line(&cur_element, -radius, this->height, -radius, -radius, this->height, radius);

	add_line(&cur_element, -radius, 0.0f, radius, - radius, this->height, radius);
	add_line(&cur_element, radius, 0.0f, radius, radius, this->height, radius);
	add_line(&cur_element, radius, 0.0f, -radius, radius, this->height, -radius);
	add_line(&cur_element, -radius, 0.0f, -radius, -radius, this->height, -radius);

	glBufferData(GL_ARRAY_BUFFER, elements_size, this->bounding_box_data, GL_STATIC_DRAW);

	int player_attribute_stride = 3 * sizeof(float);

	GLint bounding_box_position_data = glGetAttribLocation(this->shader->ID, "position");
	glVertexAttribPointer(bounding_box_position_data, 3, GL_FLOAT, GL_FALSE, player_attribute_stride, 0);
	glEnableVertexAttribArray(bounding_box_position_data);

	glBindVertexArray(GL_NONE);
}
