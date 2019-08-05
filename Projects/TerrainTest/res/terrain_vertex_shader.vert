#version 330 core

in vec2 xy_data;
in float height_data;

uniform mat4 projection;
uniform mat4 view;

//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    mat4 view;
//};

uniform mat4 model;
uniform mat4 local_transform;

out float z;

void main()
{
	gl_Position = projection * view * model * local_transform * vec4(xy_data.x, xy_data.y, height_data, 1.0);
	z = height_data;
}	