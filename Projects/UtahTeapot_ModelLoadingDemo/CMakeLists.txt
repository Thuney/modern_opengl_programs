project (UtahTeapot_ModelLoadingDemo)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(UtahTeapot_ModelLoadingDemo src/Main.cpp)

target_link_libraries(UtahTeapot_ModelLoadingDemo SharedGLClasses)
