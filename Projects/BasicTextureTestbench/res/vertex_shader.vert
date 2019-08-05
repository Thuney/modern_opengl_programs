#version 150									//Defines the GLSL version of this shader to be 1.50

in vec2 position;								//Defines an input to the shader which is a 2-dimensional vector
in vec2 tex_coords;

out vec2 frag_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position.x, position.y, 0.0, 1.0);		//Set the homogenous coordinates of the vertex given our 2D vector input
	frag_tex_coords = tex_coords;
}