#ifndef IMGUI_GLFW_OPENGL_WINDOW_H
#define IMGUI_GLFW_OPENGL_WINDOW_H

#include "imgui.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ImGui_GLFW_GLEW_OpenGL_Window
{
public:
	int window_width, window_height;

	ImGui_GLFW_GLEW_OpenGL_Window(int window_width, int window_height, bool resizeable = false, const char* window_name = "OpenGL");
	~ImGui_GLFW_GLEW_OpenGL_Window();

	void clear_color(float r, float g, float b, float a);

	void new_frame();
	void swap_buffers();

	void render();

	//Called when ESC pressed
	bool should_close();

private:

	char g_GlslVersionString[32] = "";
	GLuint g_FontTexture = 0;
	GLuint g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
	int g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
	int g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
	unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

	//Setup functions
	void setup_imgui();
	void setup_glfw_window(bool resizeable, const char* window_name);
	void setup_glfw_inputs(bool install_callbacks = true);
	bool setup_opengl(const char* glsl_version);

	//New Frame Functions
	void glfw_new_frame();
	void opengl_new_frame();

	//Shutdown functions
	void glfw_shutdown();
	void opengl_shutdown();

	//OpenGL GUI element rendering functions
	void opengl_renderDrawData(ImDrawData* draw_data);
	bool opengl_createFontsTexture();
	void opengl_destroyFontsTexture();
	bool opengl_createDeviceObjects();
	void opengl_destroyDeviceObjects();

};

#endif