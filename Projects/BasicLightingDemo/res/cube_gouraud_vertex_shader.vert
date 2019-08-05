#version 150

in vec3 position;
in vec3 normal;

out vec3 vertexColor;

struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 position;
	vec3 color;
	float ambient_intensity;
	float diffuse_intensity;
	float specular_intensity;
};
uniform Light light;

uniform vec3 cameraPosition;

uniform mat3 normal_matrix;

uniform mat4 local_transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * local_transform * vec4(position, 1.0);
	vec3 vertexNormal = normal_matrix * normal;

	vec3 vertex_position = vec3(model * local_transform * vec4(position, 1.0));

	//Phong shading model implementation

	//Calculate constant ambient light contribution to color
	vec3 ambient_light = (light.ambient_intensity * light.color) * material.ambient_color;

	//Calculate diffuse light contribution based on surface normal and light position
	vec3 normal = normalize(vertexNormal);
	vec3 light_direction = normalize(light.position - vertex_position);

	float diffuse_projection = max(dot(normal, light_direction), 0.0);
	vec3 diffuse_light = (light.diffuse_intensity * light.color) * material.diffuse_color * diffuse_projection;	//Calculate specular light contribution based on reflected light ray and camera position
	vec3 view_direction = normalize(cameraPosition - vertex_position);
	vec3 reflect_direction = reflect(-light_direction, normal);	float specular = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = (light.specular_intensity * light.color) * material.specular_color * specular;

	vertexColor = ambient_light + diffuse_light + specular_light;
}