#version 150									//Defines the GLSL version of this shader to be 1.50

in vec2 position;								//Defines an input to the shader which is a 2-dimensional vector

out vec2 out_position;

uniform float offsetX;

uniform float offsetY;

void main()
{
	gl_Position = vec4(position.x + 0.5*offsetX, position.y + 0.5*offsetY, 0.0, 1.0);		//Set the homogenous coordinates of the vertex given our 2D vector input
	out_position = gl_Position.xy;
}