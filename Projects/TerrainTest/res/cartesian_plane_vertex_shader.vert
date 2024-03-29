#version 330 core

in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    mat4 view;
//};

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
}