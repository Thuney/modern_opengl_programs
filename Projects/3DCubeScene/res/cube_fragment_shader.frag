#version 150									//Defines the GLSL version of this shader to be 1.50

out vec4 outColor;								//Defines an output to the shader which is a 4-dimensional vector

in vec2 frag_tex_coords;

uniform sampler2D ourTexture;

void main()
{
	//outColor = vec4(0.5, 1.0, 0.5, 1.0);
	outColor = texture(ourTexture, frag_tex_coords);
}