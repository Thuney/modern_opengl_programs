#version 150

in vec3 vert_color;

out vec4 outColor;

void main()
{
	outColor = vec4(vert_color, 1.0);
	//outColor = vec4(1.0, 0.0, 0.0, 1.0);
}