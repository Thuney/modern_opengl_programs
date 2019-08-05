#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

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

Camera scene_camera(GROUNDED, glm::vec3(0.0f, 1.5f, 3.0f));

GLfloat model_pitch = 0.0f;
GLfloat model_yaw = 0.0f;
GLfloat model_roll = 0.0f;

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
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL", nullptr, nullptr); //Windowed
	glfwMakeContextCurrent(window);
	glViewport(0, 0, window_width, window_height);

	//Hide mouse cursor and capture its input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Register callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	const float cube_attributes[] =
	{
		//Front Face
		
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f,
		
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f,

		//Left Face

		0.0f, 0.0f, 1.0f, 0.25f, 0.33f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.33f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.66f,

		0.0f, 0.0f, 1.0f, 0.25f, 0.33f,
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.66f,

		//Right Face

		1.0f, 0.0f, 0.0f, 0.75f, 0.33f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f,
		
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f,
		1.0f, 1.0f, 0.0f, 0.75f, 0.66f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f,

		//Top Face
		
		0.0f, 1.0f, 0.0f, 0.25f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.25f, 0.66f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f,

		0.0f, 1.0f, 0.0f, 0.25f, 1.0f,
		1.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.5f, 0.66f,

		//Bottom Face

		0.0f, 0.0f, 0.0f, 0.25f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.25f, 0.33f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f,

		0.0f, 0.0f, 0.0f, 0.25f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.5f, 0.33f,

		//Back Face

		0.0f, 1.0f, 0.0f, 1.0f, 0.66f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.33f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f,

		0.0f, 1.0f, 0.0f, 1.0f, 0.66f,
		1.0f, 1.0f, 0.0f, 0.75f, 0.66f,
		1.0f, 0.0f, 0.0f, 0.75f, 0.33f
	};

	Shader cubeShaderProgram("../res/cube_vertex_shader.vert", "../res/cube_fragment_shader.frag");

	Shader basisShaderProgram("../res/basis_vertex_shader.vert", "../res/basis_fragment_shader.frag");

	//Cube formation
	Texture cube_texture("../res/MinecraftGrassBlock.png", true);
	//Texture cube_texture("CubeTexture.png", true);
	glBindTexture(GL_TEXTURE_2D, cube_texture.ID);

	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	GLuint cube_vbo;
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_attributes), cube_attributes, GL_STATIC_DRAW);

	int cube_attribute_stride = 3 * sizeof(float) + 2 * sizeof(float);

	GLint cube_position_attribute = glGetAttribLocation(cubeShaderProgram.ID, "position");
	glVertexAttribPointer(cube_position_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, 0);
	glEnableVertexAttribArray(cube_position_attribute);

	GLint cube_texture_coord_attribute = glGetAttribLocation(cubeShaderProgram.ID, "tex_coords");
	glVertexAttribPointer(cube_texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, cube_attribute_stride, (void*) (3*sizeof(float)));
	glEnableVertexAttribArray(cube_texture_coord_attribute);

	/*
	 * Specify how the texture will wrap when texture coordinates are out of the normal range
	 * In this case, the texture will 'mirrored repeat' for S and T texture coordinates
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	/*
	 * Set the modes for texture filtering
	 * In this case, the 'minimizing' filter (for when we downscale textures) is set to the GL_NEAREST option
	 * and the 'magnifying' filter (for when we upscale textures) is set to the GL_LINEAR option
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const float basis_attributes[]
	{
		//X Axis
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		//Y Axis
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		//Z Axis
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f
	};

	//Axis structure formation
	GLuint basis_vao;
	glGenVertexArrays(1, &basis_vao);
	glBindVertexArray(basis_vao);

	GLuint basis_vbo;
	glGenBuffers(1, &basis_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, basis_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(basis_attributes), basis_attributes, GL_STATIC_DRAW);

	int basis_attribute_stride = 3 * sizeof(float) + 3 * sizeof(float);

	GLint basis_position_attribute = glGetAttribLocation(basisShaderProgram.ID, "position");
	glVertexAttribPointer(basis_position_attribute, 3, GL_FLOAT, GL_FALSE, basis_attribute_stride, 0);
	glEnableVertexAttribArray(basis_position_attribute);

	GLint basis_color_attribute = glGetAttribLocation(basisShaderProgram.ID, "color");
	glVertexAttribPointer(basis_color_attribute, 3, GL_FLOAT, GL_FALSE, basis_attribute_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(basis_color_attribute);
	
	glBindVertexArray(GL_NONE);

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

		projection_matrix = glm::perspective(glm::radians(scene_camera.Zoom), (float)(window_width / window_height), 0.1f, 100.0f);

		//Perform Euler angle rotations in the order x, y, z
		model_matrix = glm::mat4(1.0f);

		/*
		 * These commented portions are from me trying to figure out how to actually produce Euler angle rotation matrices from the provided rotation
		 * functions. I don't quite understand how just producing the individual rotation matrices about the original basis vectors and multiplying them 
		 * in the gimbal ordering doesn't produce the same result as the experimental Euler angle GLM library
		 * (although it should be much slower since matrix multiplication is obviously slower than raw trigonometric calculations)
		 */
		//glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(model_roll), glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(model_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(model_pitch), glm::vec3(1.0f, 0.0f, 0.0f));

		//model_matrix = glm::eulerAngleXYZ(model_pitch, model_yaw, model_roll);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(basis_vao);
		basisShaderProgram.use();

		basisShaderProgram.setMat4("view", scene_camera.getViewMatrix());
		basisShaderProgram.setMat4("projection", projection_matrix);
		
		model_matrix = glm::eulerAngleX(model_pitch);
		basisShaderProgram.setMat4("model", model_matrix);
		glDrawArrays(GL_LINES, 0, 2);

		model_matrix = glm::eulerAngleXY(model_pitch, model_yaw);
		basisShaderProgram.setMat4("model", model_matrix);
		glDrawArrays(GL_LINES, 2, 2);

		model_matrix = glm::eulerAngleXYZ(model_pitch, model_yaw, model_roll);
		basisShaderProgram.setMat4("model", model_matrix);
		glDrawArrays(GL_LINES, 4, 2);

		//model_matrix = rotX;
		//basisShaderProgram.setMat4("model", model_matrix);
		//glDrawArrays(GL_LINES, 0, 2);

		//model_matrix = rotY * rotX;
		//basisShaderProgram.setMat4("model", model_matrix);
		//glDrawArrays(GL_LINES, 2, 2);

		//model_matrix = rotZ * rotY * rotX;
		//basisShaderProgram.setMat4("model", model_matrix);
		//glDrawArrays(GL_LINES, 4, 2);

		//model_matrix = rotZ * rotY * rotX;
		//basisShaderProgram.setMat4("model", model_matrix);
		//glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(cube_vao);
		cubeShaderProgram.use();

		cubeShaderProgram.setMat4("model", model_matrix);
		
		glm::mat4 local_transform(1.0f);
		local_transform = glm::translate(local_transform, glm::vec3(-0.5f, -0.5f, -0.5f));
		cubeShaderProgram.setMat4("local_transform", local_transform);

		cubeShaderProgram.setMat4("model", model_matrix);
		cubeShaderProgram.setMat4("view", scene_camera.getViewMatrix());
		cubeShaderProgram.setMat4("projection", projection_matrix);

		//Draw commands
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cube_vao);
	glDeleteBuffers(1, &cube_vbo);

	glDeleteVertexArrays(1, &basis_vao);
	glDeleteBuffers(1, &basis_vbo);

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
		printf("%f ms/frame\n", 1000.0 / ((double) numberOfFrames));
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Y input is simple mouse scroll input
	scene_camera.process_mouse_scroll(yoffset);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float rotation_speed = 2.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		model_pitch -= rotation_speed;
		//model_pitch = glm::mod(model_pitch, 360.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		model_pitch += rotation_speed;
		//model_pitch = glm::mod(model_pitch, 360.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			model_roll += rotation_speed;
			//model_roll = glm::mod(model_roll, 360.0f);
		}
		else
		{
			model_yaw += rotation_speed;
			//model_yaw = glm::mod(model_yaw, 360.0f);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			model_roll -= rotation_speed;
			//model_roll = glm::mod(model_roll, 360.0f);
		}
		else
		{
			model_yaw -= rotation_speed;
			//model_yaw = glm::mod(model_yaw, 360.0f);
		}
	}

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

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		model_pitch = 0.0f;
		model_yaw = 0.0f;
		model_roll = 0.0f;
	}
}