#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
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
	
	//float cube_vertices[]
	//{
	//	//Back Face
	//	0.0f, 0.0f, 0.0f,	//0
	//	0.0f, 1.0f, 0.0f,	//1
	//	1.0f, 1.0f, 0.0f,	//2
	//	1.0f, 0.0f, 0.0f,	//3
	//	//Front Face
	//	0.0f, 0.0f, 1.0f,	//4
	//	0.0f, 1.0f, 1.0f,	//5
	//	1.0f, 1.0f, 1.0f,	//6
	//	1.0f, 0.0f, 1.0f	//7
	//};

	//unsigned int cube_elements[]
	//{
	//	5, 4, 7,
	//	5, 6, 7,

	//	4, 0, 1,
	//	4, 5, 1,

	//	3, 7, 6,
	//	3, 2, 6,

	//	1, 5, 6,
	//	1, 2, 6,

	//	0, 4, 7,
	//	0, 3, 7,

	//	1, 0, 3,
	//	1, 2, 3
	//};

	float cube_vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	Shader cubeShaderProgram("../res/cube_phong_vertex_shader.vert", "../res/cube_phong_fragment_shader.frag");
	//Shader cubeShaderProgram("cube_gouraud_vertex_shader.vert", "cube_gouraud_fragment_shader.frag");
	Shader lightSourceShaderProgram("../res/light_source_vertex_shader.vert", "../res/light_source_fragment_shader.frag");

	GLuint cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	GLuint cube_vbo;
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	//GLuint cube_ebo;
	//glGenBuffers(1, &cube_ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	//int cube_attribute_stride = 3 * sizeof(float);

	//GLint cube_position_attribute = glGetAttribLocation(cubeShaderProgram.ID, "position");
	//glVertexAttribPointer(cube_position_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, 0);
	//glEnableVertexAttribArray(cube_position_attribute);

	int cube_attribute_stride = 3 * sizeof(float) + 3 * sizeof(float);

	GLint cube_position_attribute = glGetAttribLocation(cubeShaderProgram.ID, "position");
	glVertexAttribPointer(cube_position_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, 0);
	glEnableVertexAttribArray(cube_position_attribute);

	GLint cube_normal_attribute = glGetAttribLocation(cubeShaderProgram.ID, "normal");
	glVertexAttribPointer(cube_normal_attribute, 3, GL_FLOAT, GL_FALSE, cube_attribute_stride, (void*) (3*sizeof(float)) );
	glEnableVertexAttribArray(cube_normal_attribute);

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
		glm::mat4 model(1.0f);

		float light_path_radius = 4.0f;
		glm::vec3 light_position(light_path_radius*sin(currentTime), 2.0f, 3.0f);

		//glm::vec3 light_position(light_path_radius*sin(currentTime), 0.2f, light_path_radius*cos(currentTime));
		//glm::vec3 light_position(2.0f, 0.2f, 4.0f);

		//glm::vec3 light_color;
		//light_color.x = sin(glfwGetTime() * 2.0f);
		//light_color.y = sin(glfwGetTime() * 0.7f);
		//light_color.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 light_color(1.0f);

		cubeShaderProgram.use();
		cubeShaderProgram.setMat4("local_transform", local_transform);
		cubeShaderProgram.setMat4("projection", projection_matrix);
		cubeShaderProgram.setMat4("view", scene_camera.getViewMatrix());
		//local_transform = glm::translate(local_transform, glm::vec3(-0.5f, -0.5f, -0.5f));

		cubeShaderProgram.setVec3("cameraPosition", scene_camera.Position);

		cubeShaderProgram.setVec3("light.position", light_position);
		cubeShaderProgram.setVec3("light.color", light_color);
		//cubeShaderProgram.setFloat("light.ambient_intensity", 0.2f);
		//cubeShaderProgram.setFloat("light.diffuse_intensity", 0.5f);
		//cubeShaderProgram.setFloat("light.specular_intensity", 1.0f);
		cubeShaderProgram.setFloat("light.ambient_intensity", 1.0f);
		cubeShaderProgram.setFloat("light.diffuse_intensity", 1.0f);
		cubeShaderProgram.setFloat("light.specular_intensity", 1.0f);

		//Box 1
		cubeShaderProgram.setVec3("material.ambient_color", 0.0f, 0.1f, 0.06f);
		cubeShaderProgram.setVec3("material.diffuse_color", 0.0f, 0.50980392f, 0.50980392f);
		cubeShaderProgram.setVec3("material.specular_color", 0.50196078f, 0.50196078f, 0.50196078f);
		cubeShaderProgram.setFloat("material.shininess", .25f * 128.0f);
		
		//glm::vec3 box1_color(0.3f, 0.3f, 0.7f);
		//cubeShaderProgram.setVec3("material.ambient_color", box1_color);
		//cubeShaderProgram.setVec3("material.diffuse_color", box1_color);
		//cubeShaderProgram.setVec3("material.specular_color", box1_color);
		//cubeShaderProgram.setFloat("material.shininess", 32.0f);

		model = glm::translate(model, glm::vec3(-0.6f, 0.0f, 0.0f));
		cubeShaderProgram.setMat4("model", model);
		cubeShaderProgram.setMat3("normal_matrix", glm::mat3(transpose(inverse(model * local_transform))));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Box 2
		//glm::vec3 box2_color(0.4f, 0.7f, 0.3f);
		//cubeShaderProgram.setVec3("material.ambient_color", box2_color);
		//cubeShaderProgram.setVec3("material.diffuse_color", box2_color);
		//cubeShaderProgram.setVec3("material.specular_color", box2_color);
		//cubeShaderProgram.setFloat("material.shininess", 32.0f);

		cubeShaderProgram.setVec3("material.ambient_color", glm::vec3(0.0215, 0.1745, 0.0215));
		cubeShaderProgram.setVec3("material.diffuse_color", glm::vec3(0.07568, 0.61424, 0.07568));
		cubeShaderProgram.setVec3("material.specular_color", glm::vec3(0.633, 0.727811, 0.633));
		cubeShaderProgram.setFloat("material.shininess", 0.6f * 128.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.3f));
		cubeShaderProgram.setMat4("model", model);
		cubeShaderProgram.setMat3("normal_matrix", glm::mat3(transpose(inverse(model))));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Box 3
		cubeShaderProgram.setVec3("material.ambient_color", glm::vec3(0.19225, 0.19225, 0.19225));
		cubeShaderProgram.setVec3("material.diffuse_color", glm::vec3(0.50754, 0.50754, 0.50754));
		cubeShaderProgram.setVec3("material.specular_color", glm::vec3(0.508273, 0.508273, 0.508273));
		cubeShaderProgram.setFloat("material.shininess", 0.4f * 128.0f);

		//glm::vec3 box3_color(0.0f, 0.5f, 1.0f);
		//cubeShaderProgram.setVec3("material.ambient_color", box3_color);
		//cubeShaderProgram.setVec3("material.diffuse_color", box3_color);
		//cubeShaderProgram.setVec3("material.specular_color", box3_color);
		//cubeShaderProgram.setFloat("material.shininess", 32.0f);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.7f));
		cubeShaderProgram.setMat4("model", model);
		cubeShaderProgram.setMat3("normal_matrix", glm::mat3(transpose(inverse(model))));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, light_position);
		model = glm::scale(model, glm::vec3(0.15f));

		lightSourceShaderProgram.use();
		lightSourceShaderProgram.setMat4("model", model);
		lightSourceShaderProgram.setMat4("projection", projection_matrix);
		lightSourceShaderProgram.setMat4("view", scene_camera.getViewMatrix());

		lightSourceShaderProgram.setVec3("lightColor", light_color);

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cube_vao);
	glDeleteBuffers(1, &cube_vbo);

	glDeleteProgram(cubeShaderProgram.ID);
	glDeleteProgram(lightSourceShaderProgram.ID);

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