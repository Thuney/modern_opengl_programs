#ifndef PERLIN_MESH_H
#define PERLIN_MESH_H

#include <GL/glew.h>

#include "PerlinNoise.hpp"
#include "Shader.h"

class PerlinTerrainMesh
{

public:
	GLuint vao;
	GLuint vbo;

	float* terrain_xy_data;
	float* perlin_height_data;

	int xy_data_size;
	int height_data_size;

	PerlinTerrainMesh(float width, float height, float cell_size, float scale_factor, Shader* shader);
	~PerlinTerrainMesh();

	void draw();

	Shader* mesh_shader;

private:
	siv::PerlinNoise perlin;
	
	int cols, rows;
	
	float cell_size;
	float scale_factor;

	float x_start = 0.0f;
	float y_start = 0.0f;

	const float xinc = 0.1f;
	const float yinc = 0.1f;

	int num_vertices;

	void generate_mesh();
	void add_terrain_vertex(float** cur_xy_element, float** cur_height_element, int col, int row, float xoff, float yoff);

	void update_height_data();
};

#endif