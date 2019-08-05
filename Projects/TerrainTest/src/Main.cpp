#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Player.h"
#include "Shader.h"

#include "PerlinTerrainMesh.h"
#include "CartesianPlane.h"

void calculate_frame_rate(GLdouble& lastTime);

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//Prototype for the callback function to handle mouse input
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
//Prototype for the callback function to handle mouse scroll wheel input
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void process_input(GLFWwindow* window);

int window_width = 1600;
int window_height = 1200;

GLdouble lastTime = glfwGetTime();
GLdouble deltaTime = 0.0;

glm::mat4 projection_matrix(1.0f);
glm::mat4 model_matrix(1.0f);

Player* player;

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
	glfwSetScrollCallback(window, scroll_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	Shader terrain_shader("../res/terrain_vertex_shader.vert", "../res/terrain_fragment_shader.frag");
	Shader plane_shader("../res/cartesian_plane_vertex_shader.vert", "../res/cartesian_plane_fragment_shader.frag");

	player = new Player(glm::vec3(0.0f, 0.0f, 50.0f), 65.0f, &plane_shader);

	////Collision detection testing settings
	//float terrain_width = 500.0f;
	//float terrain_height = 500.0f;
	//float terrain_cell_size = 10.0f;
	//float scale_factor = 40.0f;

	//float terrain_width = 50.0f;
	//float terrain_height = 50.0f;
	//float terrain_cell_size = 0.5f;
	//float scale_factor = 5.0f;

	float terrain_width = 50.0f;
	float terrain_height = 50.0f;
	float terrain_cell_size = 2.0f;
	float scale_factor = 6.0f;


	PerlinTerrainMesh terrain(terrain_width, terrain_height, terrain_cell_size, scale_factor, &terrain_shader);
	
	CartesianPlane xz_plane(glm::vec3(0.0f, 1.0f, 0.0f), 200, 200, 5, &plane_shader, RED);
	//CartesianPlane xy_plane(glm::vec3(0.0f, 0.0f, 1.0f), 5.0f, 5.0f, 1.0f, &plane_shader, GREEN);
	//CartesianPlane yz_plane(glm::vec3(1.0f, 0.0f, 0.0f), 200, 200, 5, &plane_shader, BLUE);

	CartesianPlane angled_plane(glm::vec3(0.0f, 1.0f, 1.0f), 50, 50, 10, &plane_shader, YELLOW);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Rendering loop; breaks on window close
	while (!glfwWindowShouldClose(window))
	{
		GLdouble currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

#if _DEBUG
		calculate_frame_rate(currentTime);
#endif

		process_input(window);

		//projection_matrix = glm::perspective(glm::radians(scene_camera.Zoom), (float)(window_width / window_height), 0.1f, 100.0f);
		projection_matrix = glm::perspective(glm::radians(115.0f), (155.0f/115.0f), 0.1f, 1000.0f);

		model_matrix = glm::mat4(1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		plane_shader.use();
		plane_shader.setMat4("view", player->getViewMatrix());
		plane_shader.setMat4("projection", projection_matrix);

		model_matrix = glm::mat4(1.0f);
		plane_shader.setMat4("model", model_matrix);

		xz_plane.draw();
		//xy_plane.draw();
		//yz_plane.draw();

		angled_plane.draw();

		terrain.mesh_shader->use();
		terrain.mesh_shader->setMat4("view", player->getViewMatrix());
		terrain.mesh_shader->setMat4("projection", projection_matrix);

		glm::mat4 local_transform_matrix = glm::mat4(1.0f);
		local_transform_matrix = glm::translate(local_transform_matrix, glm::vec3((float)(-(terrain_width / 2)), (float)-(terrain_height / 2), 0.0f));
		terrain.mesh_shader->setMat4("local_transform", local_transform_matrix);

		model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 48.0f, -12.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		terrain.mesh_shader->setMat4("model", model_matrix);

		terrain.draw();

		player->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete player;

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

	player->process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Y input is simple mouse scroll input
	player->process_mouse_scroll(yoffset);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Camera Control
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player->process_keyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player->process_keyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player->process_keyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player->process_keyboard(RIGHT, deltaTime);
	}
}