project (BasicGLSLTestbench)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(BasicGLSLTestbench src/Main.cpp)

target_link_libraries(BasicGLSLTestbench SharedGLClasses)
