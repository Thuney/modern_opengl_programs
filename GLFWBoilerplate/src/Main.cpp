#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void calculate_frame_rate(GLdouble& lastTime);

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

int window_width = 800;
int window_height = 600;

GLdouble lastTime = glfwGetTime();
GLdouble deltaTime = 0.0;

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

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}