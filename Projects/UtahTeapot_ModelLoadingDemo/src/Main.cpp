#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

void calculate_frame_rate(GLdouble& lastTime);

//Prototype for the callback function to reset the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);

void process_input(GLFWwindow* window);

int window_width = 1600;
int window_height = 1200;

GLdouble lastTime = glfwGetTime();
GLdouble deltaTime = 0.0;

//Camera scene_camera(FREE_FLOATING, glm::vec3(0.0f, 2.0f, 6.0f));
Camera scene_camera(FREE_FLOATING, glm::vec3(0.0f, 70.0f, 6.0f));

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
	glEnable(GL_STENCIL_TEST);

	glm::mat4 model(1.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(115.0f), (155.0f / 115.0f), 0.1f, 1000.0f);

	Shader single_color_shader("../res/normal_scaled_outline_vertex_shader.vert", "../res/single_color_fragment_shader.frag");

	Shader skybox_shader("../res/skybox_vertex_shader.vert", "../res/skybox_fragment_shader.frag");

	//Model teapot_model("resources//teapot.obj");
	Model teapot_model("../res/GlobeTheatreModel/GlobeTheatre_NewModel.obj");
	//Model teapot_model("resources//GenericTexturedBlock//TexturedBlock.obj");

	float skyboxVertices[] =
	{
		//Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	GLuint skybox_vao;
	glGenVertexArrays(1, &skybox_vao);
	glBindVertexArray(skybox_vao);

	GLuint skybox_vbo;
	glGenBuffers(1, &skybox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(GL_NONE);

	vector<std::string> faces
	{
		"../res/ame_iceflats/iceflats_ft.tga",
		"../res/ame_iceflats/iceflats_bk.tga",
		"../res/ame_iceflats/iceflats_up.tga",
		"../res/ame_iceflats/iceflats_dn.tga",
		"../res/ame_iceflats/iceflats_rt.tga",
		"../res/ame_iceflats/iceflats_lf.tga"
	};
	//vector<std::string> faces
	//{
	//	"resources/skybox/right.jpg",
	//	"resources/skybox/left.jpg",
	//	"resources/skybox/top.jpg",
	//	"resources/skybox/bottom.jpg",
	//	"resources/skybox/front.jpg",
	//	"resources/skybox/back.jpg"
	//};
	GLuint cubemapTexture = Texture::loadCubemap(faces);

	Shader phong_texture_shader = Shader("../res/phong_textured_vertex_shader.vert", "../res/phong_textured_fragment_shader.frag");
	Shader phong_solidmaterial_shader = Shader("../res/phong_solidmaterial_vertex_shader.vert", "../res/phong_solidmaterial_fragment_shader.frag");

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 60.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));

		//-------------------------------------------------------------------------------------------------------------

		phong_texture_shader.use();

		phong_texture_shader.setMat4("model", model);
		phong_texture_shader.setMat4("view", scene_camera.getViewMatrix());
		phong_texture_shader.setMat4("projection", projection);

		phong_texture_shader.setMat3("normal_matrix", glm::mat3(transpose(inverse(model))));

		phong_texture_shader.setVec3("cameraPosition", scene_camera.Position);

		//-------------------------------------------------------------------------------------------------------------

		phong_solidmaterial_shader.use();

		phong_solidmaterial_shader.setMat4("model", model);
		phong_solidmaterial_shader.setMat4("view", scene_camera.getViewMatrix());
		phong_solidmaterial_shader.setMat4("projection", projection);

		phong_solidmaterial_shader.setMat3("normal_matrix", glm::mat3(transpose(inverse(model))));

		phong_solidmaterial_shader.setVec3("cameraPosition", scene_camera.Position);

		//-------------------------------------------------------------------------------------------------------------

		glm::vec3 light_direction = glm::vec3(0.2, -0.4, -0.3);
		glm::vec3 light_color(1.0f);

		//-------------------------------------------------------------------------------------------------------------

		phong_texture_shader.use();

		//phong_texture_shader.setVec3("light.direction", light_direction);

		phong_texture_shader.setVec3("light.position", glm::vec3(0.0f, 70.0f, 0.0f));

		phong_texture_shader.setVec3("light.color", light_color);
		phong_texture_shader.setFloat("light.ambient_intensity", 0.4f);
		phong_texture_shader.setFloat("light.diffuse_intensity", 0.8f);
		phong_texture_shader.setFloat("light.specular_intensity", 0.1f);

		phong_texture_shader.setFloat("light.attenuation_constant", 1.0f);
		phong_texture_shader.setFloat("light.attenuation_linear", 0.045f);
		phong_texture_shader.setFloat("light.attenuation_quadratic", 0.0075f);

		//Spot Light

		phong_texture_shader.setVec3("flashlight.position", scene_camera.Position);
		phong_texture_shader.setVec3("flashlight.direction", scene_camera.Front);

		phong_texture_shader.setFloat("flashlight.cos_inner_cutoff_angle", glm::cos(glm::radians(12.5f)));
		phong_texture_shader.setFloat("flashlight.cos_outer_cutoff_angle", glm::cos(glm::radians(17.5f)));

		phong_texture_shader.setVec3("flashlight.color", light_color);
		phong_texture_shader.setFloat("flashlight.ambient_intensity", 0.4f);
		phong_texture_shader.setFloat("flashlight.diffuse_intensity", 0.8f);
		phong_texture_shader.setFloat("flashlight.specular_intensity", 0.1f);

		phong_texture_shader.setFloat("flashlight.attenuation_constant", 1.0f);
		phong_texture_shader.setFloat("flashlight.attenuation_linear", 0.22f);
		phong_texture_shader.setFloat("flashlight.attenuation_quadratic", 0.20f);

		//-------------------------------------------------------------------------------------------------------------

		phong_solidmaterial_shader.use();

		//phong_texture_shader.setVec3("light.direction", light_direction);

		phong_solidmaterial_shader.setVec3("light.position", glm::vec3(0.0f, 70.0f, 0.0f));

		phong_solidmaterial_shader.setVec3("light.color", light_color);
		phong_solidmaterial_shader.setFloat("light.ambient_intensity", 0.4f);
		phong_solidmaterial_shader.setFloat("light.diffuse_intensity", 0.8f);
		phong_solidmaterial_shader.setFloat("light.specular_intensity", 0.1f);

		phong_solidmaterial_shader.setFloat("light.attenuation_constant", 1.0f);
		phong_solidmaterial_shader.setFloat("light.attenuation_linear", 0.045f);
		phong_solidmaterial_shader.setFloat("light.attenuation_quadratic", 0.0075f);

		//Spot Light

		phong_solidmaterial_shader.setVec3("flashlight.position", scene_camera.Position);
		phong_solidmaterial_shader.setVec3("flashlight.direction", scene_camera.Front);

		phong_solidmaterial_shader.setFloat("flashlight.cos_inner_cutoff_angle", glm::cos(glm::radians(12.5f)));
		phong_solidmaterial_shader.setFloat("flashlight.cos_outer_cutoff_angle", glm::cos(glm::radians(17.5f)));

		phong_solidmaterial_shader.setVec3("flashlight.color", light_color);
		phong_solidmaterial_shader.setFloat("flashlight.ambient_intensity", 0.4f);
		phong_solidmaterial_shader.setFloat("flashlight.diffuse_intensity", 0.8f);
		phong_solidmaterial_shader.setFloat("flashlight.specular_intensity", 0.1f);

		phong_solidmaterial_shader.setFloat("flashlight.attenuation_constant", 1.0f);
		phong_solidmaterial_shader.setFloat("flashlight.attenuation_linear", 0.22f);
		phong_solidmaterial_shader.setFloat("flashlight.attenuation_quadratic", 0.20f);

		//-------------------------------------------------------------------------------------------------------------

		teapot_model.Draw(&phong_texture_shader, &phong_solidmaterial_shader);

		////Stencil test outline
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor

		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		//teapot_model.Draw(model_shader);

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);

		//single_color_shader.use();
		//single_color_shader.setMat4("model", model);
		//single_color_shader.setMat4("view", scene_camera.GetViewMatrix());
		//single_color_shader.setMat4("projection", projection);

		//single_color_shader.setMat3("normal_matrix", glm::mat3(transpose(inverse(model))));

		//teapot_model.Draw(single_color_shader);

		//glStencilMask(0xFF);
		//glEnable(GL_DEPTH_TEST);

		//Render skybox
		skybox_shader.use();
		skybox_shader.setMat4("view", glm::mat4(glm::mat3(scene_camera.getViewMatrix())));
		skybox_shader.setMat4("projection", projection);
		skybox_shader.setInt("skybox", 0);

		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(skybox_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

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
		printf("%d frames/sec | %f ms/frame\n", numberOfFrames, 1000.0 / double(numberOfFrames));
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
