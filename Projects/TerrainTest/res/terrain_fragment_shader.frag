#version 330 core

in float z;

out vec4 outColor;	

void main()
{
	if(z < 0)
		outColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(z >= 0 && z < 20)
		outColor = vec4(0.0, 0.0, 1.0, 1.0);
	else if(z >= 20)
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
}