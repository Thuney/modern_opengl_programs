#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

void calculate_frame_rate(GLdouble& lastTime);

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Prototype for the callback function to handle mouse input
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);

void process_input(GLFWwindow* window);

int window_width = 1600;
int window_height = 1200;

GLdouble lastTime = glfwGetTime();
GLdouble deltaTime = 0.0;

glm::mat4 projection_matrix(1.0f);

//Camera scene_camera(GROUNDED, glm::vec3(0.0f, 2.0f, 3.0f));
Camera scene_camera(FREE_FLOATING, glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	glfwInit();

	//Set some GLFW settings such as GL context version, modern core profile for the context, etc.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Create our window and bind a context to it by making it current
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	glViewport(0, 0, window_width, window_height);

	//Hide mouse cursor and capture its input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	const float cube_attributes[] =
	{
		//Front Face

		0.0f, 0.0f, 1.0f, 0.25f, 0.33f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f, 0.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 0.5f, 0.66f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f, 0.0f, 0.0f, 1.0f,

		//Left Face

		0.0f, 0.0f, 0.0f, 0.0f, 0.33f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f, -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.66f, -1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 1.0f, 0.25f, 0.66f, -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.66f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f, -1.0f, 0.0f, 0.0f,

		//Right Face

		1.0f, 0.0f, 1.0f, 0.5f, 0.33f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.75f, 0.66f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f, 1.0f, 0.0f, 0.0f,

		//Top Face

		0.0f, 1.0f, 1.0f, 0.25f, 0.66f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f, 0.0f, 1.0f, 0.0f,

		//Bottom Face

		0.0f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f, 0.0f, -1.0f, 0.0f,

		1.0f, 0.0f, 1.0f, 0.5f, 0.33f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f,

		//Back Face

		0.0f, 0.0f, 0.0f, 1.0f, 0.33f, 0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 0.66f, 0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f, 0.0f, 0.0f, -1.0f,

		1.0f, 1.0f, 0.0f, 0.75f, 0.66f, 0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f, 0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 0.66f, 0.0f, 0.0f, -1.0f
	};

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Shader cubeShaderProgram("../res/cube_vertex_shader.vert", "../res/cube_fragment_shader.frag");
	//Shader lightSourceShaderProgram("../res/light_source_vertex_shader.vert", "../res/light_source_fragment_shader.frag");

	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	GLuint cube_vbo;
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_attributes), cube_attributes, GL_STATIC_DRAW);

	int cube_attribute_stride = 3 * sizeof(float) + 2 * sizeof(float) + 3 * sizeof(float);

	GLint cube_position_attribute = glGetAttribLocation(cubeShaderProgram.ID, "position");
	glVertexAttribPointer(cube_position_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, 0);
	glEnableVertexAttribArray(cube_position_attribute);

	GLint cube_texture_coords_attribute = glGetAttribLocation(cubeShaderProgram.ID, "texture_coords");
	glVertexAttribPointer(cube_texture_coords_attribute, 2, GL_FLOAT, GL_FALSE, cube_attribute_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(cube_texture_coords_attribute);

	GLint cube_normal_attribute = glGetAttribLocation(cubeShaderProgram.ID, "normal");
	glVertexAttribPointer(cube_normal_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, (void*) (5*sizeof(float)) );
	glEnableVertexAttribArray(cube_normal_attribute);

	Texture cube_texture("../res/MinecraftGrassBlock.png", true);

	projection_matrix = glm::perspective(glm::radians(45.0f), (float) (window_width / window_height), 0.1f, 100.0f);

	//Rendering loop; breaks on window close
	while (!glfwWindowShouldClose(window))
	{
		//Frame time calculations
		GLdouble currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		#if _DEBUG
				calculate_frame_rate(currentTime);
		#endif

		process_input(window);

		//Rendering

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 local_transform(1.0f);
		local_transform = glm::translate(local_transform, glm::vec3(-0.5f));

		glm::mat4 model(1.0f);

		float light_path_radius = 2.0f;
		glm::vec3 light_position(light_path_radius*sin(currentTime), 1.5f, light_path_radius*cos(currentTime));

		glm::vec3 light_color(1.0f, 1.0f, 1.0f);

		cubeShaderProgram.use();
		cubeShaderProgram.setMat4("local_transform", local_transform);
		cubeShaderProgram.setMat4("projection", projection_matrix);
		cubeShaderProgram.setMat4("view", scene_camera.getViewMatrix());

		cubeShaderProgram.setVec3("cameraPosition", scene_camera.Position);

		cubeShaderProgram.setVec3("light.position", scene_camera.Position);
		cubeShaderProgram.setVec3("light.direction", scene_camera.Front);
		cubeShaderProgram.setFloat("light.cos_inner_cutoff_angle", cos(glm::radians(12.5f)));
		cubeShaderProgram.setFloat("light.cos_outer_cutoff_angle", cos(glm::radians(17.5f)));

		//cubeShaderProgram.setVec3("light.position", light_position);

		cubeShaderProgram.setVec3("light.color", light_color);
		cubeShaderProgram.setFloat("light.ambient_intensity", 0.2f);
		cubeShaderProgram.setFloat("light.diffuse_intensity", 0.5f);
		cubeShaderProgram.setFloat("light.specular_intensity", 1.0f);

		//Attenuation values for a distance of ~50 unit lengths
		cubeShaderProgram.setFloat("light.attenuation_constant", 1.0f);
		cubeShaderProgram.setFloat("light.attenuation_linear", 0.09f);
		cubeShaderProgram.setFloat("light.attenuation_quadratic", 0.032f);

		//Box 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture.ID);
		cubeShaderProgram.setInt("material.diffuse_sampler", 0);
		cubeShaderProgram.setVec3("material.specular_color", glm::vec3(0.0f));
		cubeShaderProgram.setFloat("material.shininess", 32.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		cubeShaderProgram.setMat4("model", model);
		cubeShaderProgram.setMat3("normal_matrix", glm::mat3(transpose(inverse(model * local_transform))));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		////Render light source
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, light_position);
		//model = glm::scale(model, glm::vec3(0.15f));

		//lightSourceShaderProgram.use();
		//lightSourceShaderProgram.setMat4("model", model);
		//lightSourceShaderProgram.setMat4("projection", projection_matrix);
		//lightSourceShaderProgram.setMat4("view", scene_camera.getViewMatrix());

		//lightSourceShaderProgram.setVec3("lightColor", light_color);

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cube_vao);
	glDeleteBuffers(1, &cube_vbo);

	glDeleteProgram(cubeShaderProgram.ID);
	//glDeleteProgram(lightSourceShaderProgram.ID);

	//Clean up GLFW
	glfwTerminate();

	return 0;
}

void calculate_frame_rate(GLdouble& currentTime)
{
	//Measure ms per frame
	static GLdouble referenceSecond = currentTime;
	static int numberOfFrames = 0;

	numberOfFrames++;
	if (currentTime - referenceSecond >= 1.0)
	{
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(numberOfFrames));
		numberOfFrames = 0;
		referenceSecond += 1.0;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos)
{
	static GLdouble lastX = ((GLdouble)window_width) / 2.0;
	static GLdouble lastY = ((GLdouble)window_height) / 2.0;

	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLdouble xoffset = xpos - lastX;
	GLdouble yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	scene_camera.process_mouse_movement(xoffset, yoffset);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Camera Control
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		scene_camera.process_keyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		scene_camera.process_keyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		scene_camera.process_keyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		scene_camera.process_keyboard(RIGHT, deltaTime);
	}
}