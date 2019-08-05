#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "Shader.h"

#define PI 3.14159265

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	Shader shaderProgram("../res/vertex_shader.vert", "../res/fragment_shader.frag");
	shaderProgram.use();

	float triangle_vertices[] =
	{
		//First Triangle
		0.0f, 0.5f,   //Vertex 1 (X, Y), top point
		0.5f, -0.5f,  //Vertex 2 (X, Y), bottom right point
		-0.5f, -0.5f, //Vertex 3 (X, Y), bottom left point
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	GLint triangle_position_attribute = glGetAttribLocation(shaderProgram.ID, "position");
	glVertexAttribPointer(triangle_position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(triangle_position_attribute);

	GLdouble lastTime = glfwGetTime();
	int numberOfFrames = 0;

	//Rendering loop; breaks on window close
	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		//Measure ms per frame
		GLdouble currentTime = glfwGetTime();
		numberOfFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0/double(numberOfFrames));
			numberOfFrames = 0;
			lastTime += 1.0;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.setFloat("red", sin(currentTime));
		shaderProgram.setFloat("green", sin(2*currentTime));
		shaderProgram.setFloat("blue", sin(3*currentTime));

		shaderProgram.setFloat("offsetX", cos(currentTime));
		shaderProgram.setFloat("offsetX", sin(currentTime));

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{

}