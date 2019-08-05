project (MasterProject)
cmake_minimum_required(VERSION 3.1)

add_definitions(-DGLEW_STATIC)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/GLEW EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/GLFW EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/GLM EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/ASSIMP ${PROJECT_SOURCE_DIR}/vendor/ASSIMP/build EXCLUDE_FROM_ALL)
add_subdirectory(${PROJECT_SOURCE_DIR}/SharedGLClasses ${PROJECT_SOURCE_DIR}/SharedGLClasses/build EXCLUDE_FROM_ALL)

include_directories(${PROJECT_SOURCE_DIR}/vendor/IMGUI)
add_library(
	ImGUI
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imconfig.h
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui.cpp
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui.h
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui_internal.h
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imgui_widgets.cpp
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imstb_rectpack.h
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imstb_textedit.h
	${PROJECT_SOURCE_DIR}/vendor/IMGUI/imstb_truetype.h
)
target_link_libraries(ImGUI glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/GLFWBoilerplate ${PROJECT_SOURCE_DIR}/GLFWBoilerplate/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/BasicDiffuseMapDemo ${PROJECT_SOURCE_DIR}/Projects/BasicDiffuseMapDemo/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/BasicGLSLTestbench ${PROJECT_SOURCE_DIR}/Projects/BasicGLSLTestbench/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/BasicLightingDemo ${PROJECT_SOURCE_DIR}/Projects/BasicLightingDemo/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/BasicTextureTestbench ${PROJECT_SOURCE_DIR}/Projects/BasicTextureTestbench/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/SimpleDrawing ${PROJECT_SOURCE_DIR}/Projects/SimpleDrawing/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/TerrainTest ${PROJECT_SOURCE_DIR}/Projects/TerrainTest/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/UnequalCirclePacking ${PROJECT_SOURCE_DIR}/Projects/UnequalCirclePacking/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/Projects/UtahTeapot_ModelLoadingDemo ${PROJECT_SOURCE_DIR}/Projects/UtahTeapot_ModelLoadingDemo/build)