project (TerrainTest)
cmake_minimum_required(VERSION 3.1)

include_directories($(CMAKE_SOURCE_DIR)/../../SharedGLClasses/headers)

add_executable(
	TerrainTest
	src/Main.cpp 
	src/Player.cpp 
	src/CartesianPlane.cpp 
	src/PerlinTerrainMesh.cpp
)

target_link_libraries(TerrainTest SharedGLClasses)
