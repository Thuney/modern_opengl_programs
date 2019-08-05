project (SimpleDrawing)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(SimpleDrawing src/Main.cpp)

target_link_libraries(SimpleDrawing SharedGLClasses)
