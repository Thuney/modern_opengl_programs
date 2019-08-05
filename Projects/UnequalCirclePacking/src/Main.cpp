#include "ImGui_GLFW_GLEW_OpenGL_Window.h"

#include <stdio.h>

//GL Mathematics headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "UtilityEntryMenu.h"
#include "UnequalCirclePacker.h"

#include "Shader.h"
#include "Circle.h"

//--------------------------------------------

void calculate_frame_rate(GLdouble& lastTime);

//--------------------------------------------

int init_gl_window_width = 800;
int init_gl_window_height = 600;

GLdouble lastTime = glfwGetTime();
GLdouble deltaTime = 0.0;

glm::mat4 projection_matrix(1.0f);
glm::mat4 view_matrix(1.0f);

//--------------------------------------------

ImGui_GLFW_GLEW_OpenGL_Window* program_window = new ImGui_GLFW_GLEW_OpenGL_Window(init_gl_window_width, init_gl_window_height, true, "Unequal Circle Packing");

int main()
{
	//srand(lastTime);

	//--------------------------------------------

	Shader shader("../res/simple_position_vertex_shader.vert", "../res/single_color_fragment_shader.frag");
	shader.use();

	//Set the shader used for all Circle types
	Circle::circle_shader = &shader;

	//Set view matrix for Circle shader
	shader.setMat4("view", view_matrix);

	bool show_demo_window = false;
	bool show_utility_entry_window = true;

	UtilityEntryMenu utility_entry_menu;

	UtilityBundle* bundle = nullptr;
	UnequalCirclePacker* packer = nullptr;

	//Rendering loop; breaks on window close
	while(!program_window->should_close())
	{
		glfwPollEvents();
		
		//Calculate frame time delta
		GLdouble currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		#if _DEBUG
			//calculate_frame_rate(currentTime);
		#endif

		//--------------------------------------------

		program_window->new_frame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		int testmenu_width = program_window->window_width / 3;
		int testmenu_height = program_window->window_height;
		int testmenu_x = program_window->window_width - testmenu_width;
		int testmenu_y = 0;

		utility_entry_menu.show(testmenu_x, testmenu_y, testmenu_width, testmenu_height, &show_utility_entry_window);


		//--------------------------------------------
		//----------Algorithm Settings Bar------------
		//--------------------------------------------

		bool* p_open = NULL;

		static bool no_titlebar = false;
		static bool no_scrollbar = false;
		static bool no_menu = true;
		static bool no_move = true;

		static bool no_resize = false;
		//static bool no_resize = true;

		static bool no_collapse = true;
		static bool no_close = true;
		static bool no_nav = false;
		static bool no_background = false;
		static bool no_bring_to_front = false;

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
		if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
		if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

		int settings_bar_width = program_window->window_width - testmenu_width;
		int settings_bar_height = 180;
		int settings_bar_x = 0;
		int settings_bar_y = 0;

		ImGui::SetNextWindowPos(glm::vec2(settings_bar_x, settings_bar_y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(glm::vec2(settings_bar_width, settings_bar_height), ImGuiCond_Always);

		ImGui::Begin("Algorithm Settings", p_open, window_flags);

		static bool draw_intermediate = false;
		ImGui::Checkbox("Draw Intermediate", &draw_intermediate);

		static float radius_stepdown_value = 0.1f;
		static float done_time_cutoff = 2.0f;
		static float unit_step_cutoff = pow(10, -6);
		static float system_potential_cutoff = pow(10, -6);
		static float unit_step_stepdown_value = 0.8f;
		ImGui::DragFloat("Radius Stepdown Value", &radius_stepdown_value, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Done Time Cutoff", &done_time_cutoff, 0.1f, 0.0f, 10.0f);
		ImGui::DragFloat("Unit Step Cutoff", &unit_step_cutoff, 0.000001f, 0.0f, 1.0f, "%.06f");
		ImGui::DragFloat("System Potential Cutoff", &system_potential_cutoff, 0.000001f, 0.0f, 1.0f, "%.06f");
		ImGui::DragFloat("Unit Step Stepdown Value", &unit_step_stepdown_value, 0.01f, 0.0f, 1.0f);

		ImGui::End();

		//--------------------------------------------
		//--------------------------------------------
		//--------------------------------------------

		program_window->clear_color(0.2f, 0.3f, 0.3f, 1.0f);
		program_window->render();

		int viewport_width = testmenu_x;
		int viewport_height = program_window->window_height - settings_bar_height;
		float viewport_aspect_ratio = ((float)viewport_height / (float)viewport_width);

		glViewport(0, 0, viewport_width, viewport_height);

		if (bundle)
		{
			float x_extent = 2.5 * bundle->containing_circle.radius;
			projection_matrix = glm::ortho(-x_extent, x_extent, -(x_extent * viewport_aspect_ratio), (x_extent * viewport_aspect_ratio), -10.0f, 10.0f);
		}

		shader.setMat4("projection", projection_matrix);

		if (utility_entry_menu.execute_algorithm_flag)
		{
			if (packer) delete packer;

			bundle = utility_entry_menu.bundle;

			//packer = new UnequalCirclePacker(bundle);
			packer = new UnequalCirclePacker(bundle, true);

			utility_entry_menu.execute_algorithm_flag = false;
		}

		if(packer)
		{
			packer->radius_stepdown_value = radius_stepdown_value;
			packer->done_time_cutoff = done_time_cutoff;
			packer->unit_step_cutoff = unit_step_cutoff;
			packer->system_potential_cutoff = system_potential_cutoff;
			packer->unit_step_stepdown_value = unit_step_stepdown_value;

			packer->draw_intermediate = draw_intermediate;
		}

		if (bundle && packer)
		{
			packer->iterate(deltaTime);

			if ((packer->done && packer->packed) || packer->draw_intermediate)
				bundle->draw();
		}

		program_window->swap_buffers();
	}

	if (packer) delete packer;

	delete program_window;

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