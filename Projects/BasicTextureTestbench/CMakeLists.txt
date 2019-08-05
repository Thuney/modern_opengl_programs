project (BasicTextureTestbench)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(BasicTextureTestbench src/Main.cpp)

target_link_libraries(BasicTextureTestbench SharedGLClasses)
