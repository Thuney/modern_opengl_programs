#version 150									//Defines the GLSL version of this shader to be 1.50

out vec4 outColor;								//Defines an output to the shader which is a 4-dimensional vector

in vec2 frag_tex_coords;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mix_value;

void main()
{
	//outColor = texture(ourTexture1, frag_tex_coords) * vec4(0.5, 1.0, 0.5, 1.0);
	outColor = mix(texture(ourTexture1, frag_tex_coords), texture(ourTexture2, frag_tex_coords), mix_value);
}