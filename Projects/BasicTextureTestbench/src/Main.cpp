#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"

void calculate_frame_rate(GLdouble& lastTime, int& numberOfFrames);

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

int window_width = 800;
int window_height = 600;

float mix_value = 0.0f;

glm::mat4 projection_matrix(1.0f);
glm::mat4 view_matrix(1.0f);
glm::mat4 model_matrix(1.0f);

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	Shader shaderProgram("../res/vertex_shader.vert", "../res/fragment_shader.frag");
	shaderProgram.use();

	float triangle_attributes[] =
	{
		//First Triangle w/ Texture Coords
		0.0f, 0.5f, 0.5f, 1.0f,  //Vertex 1 (X, Y), top point
		0.5f, -0.5f, 1.0f, 0.0f, //Vertex 2 (X, Y), bottom right point
		-0.5f, -0.5f, 0.0f, 0.0f //Vertex 3 (X, Y), bottom left point
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	GLint triangle_position_attribute = glGetAttribLocation(shaderProgram.ID, "position");
	glVertexAttribPointer(triangle_position_attribute, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(triangle_position_attribute);

	GLint triangle_texture_coord_attribute = glGetAttribLocation(shaderProgram.ID, "tex_coords");
	glVertexAttribPointer(triangle_texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(triangle_texture_coord_attribute);

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

	Texture puzzle_texture("../res/test_rect.png", true);
	Texture color_block_texture("../res/img_test.png", true);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, puzzle_texture.ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, color_block_texture.ID);

	shaderProgram.setInt("ourTexture1", 0);
	shaderProgram.setInt("ourTexture2", 1);

	GLdouble lastTime = glfwGetTime();
	int numberOfFrames = 0;

	unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");

	projection_matrix = glm::perspective(glm::radians(45.0f), (float)(window_width/window_height), 0.1f, 100.0f);
	model_matrix = glm::rotate(model_matrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));

	//Rendering loop; breaks on window close
	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		#if _DEBUG
			calculate_frame_rate(lastTime, numberOfFrames);
		#endif

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.setFloat("mix_value", mix_value);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	//Clean up GLFW
	glfwTerminate();

	return 0;
}

void calculate_frame_rate(GLdouble& lastTime, int& numberOfFrames)
{
	//Measure ms per frame
	GLdouble currentTime = glfwGetTime();
	numberOfFrames++;
	if (currentTime - lastTime >= 1.0)
	{
		// printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(numberOfFrames));
		numberOfFrames = 0;
		lastTime += 1.0;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

//Input processing flags

bool up_pressed = false;
bool down_pressed = false;

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (!up_pressed)
		{
			mix_value += 0.1;
			if (mix_value >= 1.0f)
				mix_value = 1.0f;

			up_pressed = true;
		}
	}
	else
		up_pressed = false;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (!down_pressed)
		{
			mix_value -= 0.1;
			if (mix_value <= 0.0f)
				mix_value = 0.0f;

			down_pressed = true;
		}
	}
	else
		down_pressed = false;

	//if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	//{
	//	transformation_matrix = glm::rotate(transformation_matrix, glm::radians(0.1f), glm::vec3(0.0, 0.0, 1.0));
	//}

	//if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//{
	//	transformation_matrix = glm::rotate(transformation_matrix, glm::radians(-0.1f), glm::vec3(0.0, 0.0, 1.0));
	//}
}