#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

//Hardcoded strings for the very simple shaders required for drawing the white triangle

const GLchar* vertex_shader =
	"#version 150"										//Defines the GLSL version of this shader to be 1.50
	"\n"
	"in vec2 position;"									//Defines an input to the shader which is a 2-dimensional vector
	"\n"
	"void main()"
	"{"
	"	gl_Position = vec4(position, 0.0, 1.0);"		//Set the homogenous coordinates of the vertex given our 2D vector input
	"};";

const GLchar* fragment_shader_1 =
	"#version 150"										//Defines the GLSL version of this shader to be 1.50
	"\n"
	"out vec4 outColor;"								//Defines an output to the shader which is a 4-dimensional vector
	"\n"
	"void main()"
	"{"
	"	outColor = vec4(1.0, 1.0, 1.0, 1.0);"			//Set the value of the (in this case constant and white) color output
	"}";

const GLchar* fragment_shader_2 =
	"#version 150"										//Defines the GLSL version of this shader to be 1.50
	"\n"
	"out vec4 outColor;"								//Defines an output to the shader which is a 4-dimensional vector
	"\n"
	"void main()"
	"{"
	"	outColor = vec4(1.0, 0.6, 0.0, 1.0);"			//Set the value of the (in this case constant and orange) color output
	"}";

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); //Windowed
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLEW to allow GL functions to be called
	glewExperimental = GL_TRUE;
	glewInit();

	//Create and compile our vertex shader from our vertex shader source code
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);

	//Get the status of our shader compilation. If successful, vertStatus = 1
	GLint vertStatus;
	char vertCompLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertStatus);
	if (!vertStatus)
	{
		//If an error occurs during compilation, we grab the error log
		glGetShaderInfoLog(vertexShader, 512, NULL, vertCompLog);
	}

	//Create and compile our fragment shader from our fragement shader source code
	GLuint fragmentShader_1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_1, 1, &fragment_shader_1, NULL);
	glCompileShader(fragmentShader_1);

	//Get the status of our shader compilation. If successful, fragStatus = 1
	GLint frag1_Status;
	char frag1_CompLog[512];
	glGetShaderiv(fragmentShader_1, GL_COMPILE_STATUS, &frag1_Status);
	if (!frag1_Status)
	{
		//If an error occurs during compilation, we grab the error log
		glGetShaderInfoLog(vertexShader, 512, NULL, frag1_CompLog);
	}

	//Create and compile our fragment shader from our fragement shader source code
	GLuint fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_2, 1, &fragment_shader_2, NULL);
	glCompileShader(fragmentShader_2);

	//Get the status of our shader compilation. If successful, fragStatus = 1
	GLint frag2_Status;
	char frag2_CompLog[512];
	glGetShaderiv(fragmentShader_2, GL_COMPILE_STATUS, &frag2_Status);
	if (!frag2_Status)
	{
		//If an error occurs during compilation, we grab the error log
		glGetShaderInfoLog(vertexShader, 512, NULL, frag2_CompLog);
	}

	//Create a shader program and add our vertex and fragment shaders to it
	GLuint shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader_1);
	//Specify that the outColor is to be output to buffer 0
	glBindFragDataLocation(shaderProgram1, 0, "outColor");

	//Link the individual shaders together in the shader program and set the program to be used
	glLinkProgram(shaderProgram1);

	//Get the status of our shader program linking. If successful, shaderLinkStatus = 1
	GLint shader1_LinkStatus;
	char shader1_LinkLog[512];
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &shader1_LinkStatus);
	if (!shader1_LinkStatus)
	{
		//If an error occurs during compilation, we grab the error log
		glGetProgramInfoLog(shaderProgram1, 512, NULL, shader1_LinkLog);
	}

	//Create a shader program and add our vertex and fragment shaders to it
	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader_2);
	//Specify that the outColor is to be output to buffer 0
	glBindFragDataLocation(shaderProgram2, 0, "outColor");

	//Link the individual shaders together in the shader program and set the program to be used
	glLinkProgram(shaderProgram2);

	//Get the status of our shader program linking. If successful, shaderLinkStatus = 1
	GLint shader2_LinkStatus;
	char shader2_LinkLog[512];
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &shader2_LinkStatus);
	if (!shader2_LinkStatus)
	{
		//If an error occurs during compilation, we grab the error log
		glGetProgramInfoLog(shaderProgram2, 512, NULL, shader2_LinkLog);
	}

	//Now that we have compiled the shaders into a single program, we can dispose of them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader_1);
	glDeleteShader(fragmentShader_2);

	//Initialization and binding of a vertex array object for linking our vertex attributes to
	//our vertex buffer object containing the vertex data. The VAO has to be instantiated and bound 
	//before the vertex buffer object and vertex attribute arrays, and those components will be automatically
	//added to the VAO when they are instantiated
	GLuint vaos[2];
	glGenVertexArrays(2, vaos);

	//Initialization of our vertex buffer object, which stores the vertex data for the triangle we're trying to draw
	GLuint vbos[2];
	glGenBuffers(2, vbos);

	//Create VAO[0] with settings for triangle 1
	glBindVertexArray(vaos[0]);

	//Hardcoded array of our triangle vertices in (X, Y) pairs
	//Note that these values are in the range [-1.0, 1.0] to fit in
	//OpenGL's unprojected coordinate system
	float triangle_1_vertices[] =
	{
		//First Triangle
		-0.5f, 0.5f,  //Vertex 1 (X, Y), top point
		0.0f, -0.5f,  //Vertex 2 (X, Y), bottom right point
		-1.0f, -0.5f, //Vertex 3 (X, Y), bottom left point
	};

	//Set the vbo as the system's active buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	//Add our vertex data to the vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1_vertices), triangle_1_vertices, GL_STATIC_DRAW);

	//Find the index of the "position" attribute in the vertex shader
	GLint triangle1_posAttrib = glGetAttribLocation(shaderProgram1, "position");
	//Specify how to interpret the vertex data for our position attribute
	glVertexAttribPointer(triangle1_posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(triangle1_posAttrib);

	//Create VAOS[1] with settings for triangle 2
	
	glBindVertexArray(vaos[1]);

	float triangle_2_vertices[] =
	{
		//First Triangle
		0.5f, 0.5f,  //Vertex 1 (X, Y), top point
		1.0f, -0.5f,  //Vertex 2 (X, Y), bottom right point
		0.0f, -0.5f, //Vertex 3 (X, Y), bottom left point
	};

	//Set the vbo as the system's active buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	//Add our vertex data to the vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_2_vertices), triangle_2_vertices, GL_STATIC_DRAW);

	//Find the index of the "position" attribute in the vertex shader
	GLint triangle2_posAttrib = glGetAttribLocation(shaderProgram2, "position");
	//Specify how to interpret the vertex data for our position attribute
	glVertexAttribPointer(triangle2_posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(triangle2_posAttrib);

	glBindVertexArray(0);

	//Rendering loop; breaks on window close
	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vaos[0]);
		glUseProgram(shaderProgram1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(vaos[1]);
		glUseProgram(shaderProgram2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, vaos);
	glDeleteBuffers(2, vbos);

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