#version 150

in vec3 position;
in vec3 normal;

out vec3 vertexPosition;
out vec3 vertexNormal;

uniform mat3 normal_matrix;

uniform mat4 local_transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * local_transform * vec4(position, 1.0);
	vertexPosition = vec3(model * local_transform * vec4(position, 1.0));
	vertexNormal = normal_matrix * normal;
}