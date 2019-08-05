#version 150									//Defines the GLSL version of this shader to be 1.50

out vec4 outColor;								//Defines an output to the shader which is a 4-dimensional vector

in vec2 out_position;

uniform float red;

uniform float green;

uniform float blue;

void main()
{
	outColor = vec4(out_position, 0.0, 1.0);		//Set the value of the color output
}