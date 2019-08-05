#version 150

in vec3 vertexPosition;
in vec3 vertexNormal;

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

out vec4 fragColor;

void main()
{
	//Phong shading model implementation

	//Calculate constant ambient light contribution to color
	vec3 ambient_light = (light.ambient_intensity * light.color) * material.ambient_color;

	//Calculate diffuse light contribution based on surface normal and light position
	vec3 normal = normalize(vertexNormal);
	vec3 light_direction = normalize(light.position - vertexPosition);

	float diffuse_projection = max(dot(normal, light_direction), 0.0);
	vec3 diffuse_light = (light.diffuse_intensity * light.color) * material.diffuse_color * diffuse_projection;	//Calculate specular light contribution based on reflected light ray and camera position
	vec3 view_direction = normalize(cameraPosition - vertexPosition);
	vec3 reflect_direction = reflect(-light_direction, normal);	float specular = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = (light.specular_intensity * light.color) * material.specular_color * specular;

	vec3 result = ambient_light + diffuse_light + specular_light;

	fragColor = vec4(result, 1.0);
}