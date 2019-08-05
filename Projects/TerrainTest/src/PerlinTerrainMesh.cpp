#include "PerlinTerrainMesh.h"

PerlinTerrainMesh::PerlinTerrainMesh(float width, float height, float cell_size, float scale_factor, Shader* shader)
{
	this->cols = (int)(width / cell_size);
	this->rows = (int)(height / cell_size);
	this->cell_size = cell_size;
	this->scale_factor = scale_factor;
	
	this->mesh_shader = shader;
	this->perlin = siv::PerlinNoise();

	generate_mesh();
}

PerlinTerrainMesh::~PerlinTerrainMesh()
{
	delete terrain_xy_data;
	delete perlin_height_data;

	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
}

void PerlinTerrainMesh::add_terrain_vertex(float** cur_xy_element, float** cur_height_element, int col, int row, float xoff, float yoff)
{
	*((*cur_xy_element)++) = (float)(this->cell_size * (col));
	*((*cur_xy_element)++) = (float)(this->cell_size * (row));
	*((*cur_height_element)++) = (float)(this->perlin.noise(xoff, yoff) * this->scale_factor);
}

void PerlinTerrainMesh::generate_mesh()
{
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	int num_triangles = 2 * (this->cols) * (this->rows);

	//3 Vertices per triangle, 2 Elements per Vertex
	int num_xy_elements = 2 * 3 * num_triangles;
	this->xy_data_size = sizeof(float) * (num_xy_elements);

	this->terrain_xy_data = new float[(num_xy_elements)];
	float* cur_xy_element = this->terrain_xy_data;

	int num_height_elements = 3 * num_triangles;
	this->height_data_size = sizeof(float) * (num_height_elements);

	this->perlin_height_data = new float[(num_height_elements)];
	float* cur_height_element = this->perlin_height_data;

	float xoff = x_start, yoff = y_start;

	for (int r = 0; r < this->rows; r++)
	{
		for (int c = 0; c < this->cols; c++)
		{
			add_terrain_vertex(&cur_xy_element, &cur_height_element, c, r, xoff, yoff);
			add_terrain_vertex(&cur_xy_element, &cur_height_element, c, r + 1, xoff, yoff + this->yinc);
			add_terrain_vertex(&cur_xy_element, &cur_height_element, c + 1, r, xoff + this->xinc, yoff);

			add_terrain_vertex(&cur_xy_element, &cur_height_element, c + 1, r + 1, xoff + this->xinc, yoff + this->yinc);
			add_terrain_vertex(&cur_xy_element, &cur_height_element, c, r + 1, xoff, yoff + this->yinc);
			add_terrain_vertex(&cur_xy_element, &cur_height_element, c + 1, r, xoff + this->xinc, yoff);

			xoff += this->xinc;
		}
		xoff = x_start;
		yoff += this->yinc;
	}
	yoff = y_start;

	this->num_vertices = 3 * num_triangles;

	glBufferData(GL_ARRAY_BUFFER, this->xy_data_size + this->height_data_size, 0, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->xy_data_size, this->terrain_xy_data);
	glBufferSubData(GL_ARRAY_BUFFER, this->xy_data_size, this->height_data_size, this->perlin_height_data);

	int terrain_attribute_stride = 2 * sizeof(float);

	GLint terrain_xy_data = glGetAttribLocation(this->mesh_shader->ID, "xy_data");
	glVertexAttribPointer(terrain_xy_data, 2, GL_FLOAT, GL_FALSE, terrain_attribute_stride, 0);
	glEnableVertexAttribArray(terrain_xy_data);

	GLint terrain_height_data = glGetAttribLocation(this->mesh_shader->ID, "height_data");
	glVertexAttribPointer(terrain_height_data, 1, GL_FLOAT, GL_FALSE, 0, (void *)xy_data_size);
	glEnableVertexAttribArray(terrain_height_data);

	glBindVertexArray(GL_NONE);
}

void PerlinTerrainMesh::update_height_data()
{
	float* cur_height_element = this->perlin_height_data;

	float xoff = x_start, yoff = y_start;

	for (int r = 0; r < this->rows; r++)
	{
		for (int c = 0; c < this->cols; c++)
		{
			*(cur_height_element++) = (float)(this->perlin.noise(xoff, yoff) * this->scale_factor);
			*(cur_height_element++) = (float)(this->perlin.noise(xoff, yoff + this->yinc) * this->scale_factor);
			*(cur_height_element++) = (float)(this->perlin.noise(xoff + this->xinc, yoff) * this->scale_factor);

			*(cur_height_element++) = (float)(this->perlin.noise(xoff + this->xinc, yoff + this->yinc) * this->scale_factor);
			*(cur_height_element++) = (float)(this->perlin.noise(xoff, yoff + this->yinc) * this->scale_factor);
			*(cur_height_element++) = (float)(this->perlin.noise(xoff + this->xinc, yoff) * this->scale_factor);

			xoff += this->xinc;
		}
		xoff = x_start;
		yoff += this->yinc;
	}
	yoff = y_start;

	this->y_start += 0.005;

	glBufferSubData(GL_ARRAY_BUFFER, this->xy_data_size, this->height_data_size, this->perlin_height_data);
}

void PerlinTerrainMesh::draw()
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

	glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);

	update_height_data();

	glBindVertexArray(GL_NONE);
}

