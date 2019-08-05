project (UnequalCirclePacking)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(
	UnequalCirclePacking 
	src/Main.cpp 
	src/Circle.cpp 
	src/ImGui_GLFW_GLEW_OpenGL_Window.cpp 
	src/UnequalCirclePacker.cpp 
	src/UtilityBundle.cpp 
	src/UtilityEntryMenu.cpp 
)

target_link_libraries(UnequalCirclePacking SharedGLClasses)
target_link_libraries(UnequalCirclePacking ImGUI)
