#version 330 core
out vec4 fragment_color;

in vec3 texture_coords;

uniform samplerCube skybox;

void main()
{    
    fragment_color = texture(skybox, texture_coords);
}