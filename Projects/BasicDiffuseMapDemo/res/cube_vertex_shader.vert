#version 150

in vec3 position;
in vec3 normal;

in vec2 texture_coords;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 frag_tex_coords;

uniform mat3 normal_matrix;

uniform mat4 local_transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * local_transform * vec4(position, 1.0);
	fragmentPosition = vec3(model * local_transform * vec4(position, 1.0));
//	fragmentPosition = position;
	fragmentNormal = normal_matrix * normal;
	frag_tex_coords = texture_coords;
}