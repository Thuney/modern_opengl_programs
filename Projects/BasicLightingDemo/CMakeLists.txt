project (BasicLightingDemo)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(BasicLightingDemo src/Main.cpp)

target_link_libraries(BasicLightingDemo SharedGLClasses)
