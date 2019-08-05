project (SharedGLClasses)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/headers)
include_directories($(CMAKE_SOURCE_DIR)/vendor)

add_library(
			SharedGLClasses
			sources/Shader.cpp
			sources/Texture.cpp
			sources/Camera.cpp
			sources/Cubemap.cpp
			sources/Model.cpp
			)

set_target_properties(SharedGLClasses PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SharedGLClasses glfw)
target_link_libraries(SharedGLClasses libglew_static)
target_link_libraries(SharedGLClasses glm)
target_link_libraries(SharedGLClasses assimp)