project (BasicDiffuseMapDemo)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(BasicDiffuseMapDemo src/Main.cpp)

target_link_libraries(BasicDiffuseMapDemo SharedGLClasses)
