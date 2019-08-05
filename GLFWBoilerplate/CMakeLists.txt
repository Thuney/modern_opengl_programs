project (GLFWBoilerplate)
cmake_minimum_required(VERSION 3.1)

add_executable(GLFWBoilerplate src/Main.cpp)

target_link_libraries(GLFWBoilerplate libglew_static)
target_link_libraries(GLFWBoilerplate glfw)
target_link_libraries(GLFWBoilerplate glm)