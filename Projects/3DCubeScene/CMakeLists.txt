project (3DCubeScene)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(3DCubeScene src/Main.cpp)

target_link_libraries(3DCubeScene SharedGLClasses)
