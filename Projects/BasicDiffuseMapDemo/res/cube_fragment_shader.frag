#version 150

//Light type struct declarations
struct PointLight
{
	vec3 position;

	vec3 color;
	float ambient_intensity;
	float diffuse_intensity;
	float specular_intensity;

	float attenuation_constant;
	float attenuation_linear;
	float attenuation_quadratic;
};

struct DirectionalLight
{
    vec3 direction;
  
	vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
}; 

struct SpotLight
{
	vec3 position;
	vec3 direction;
    float cos_inner_cutoff_angle;
	float cos_outer_cutoff_angle;

	vec3 color;
	float ambient_intensity;
	float diffuse_intensity;
	float specular_intensity;

	float attenuation_constant;
	float attenuation_linear;
	float attenuation_quadratic;
};

//Material struct declaration
struct Material
{
	sampler2D diffuse_sampler;
	vec3 specular_color;
	float shininess;
};

//Function prototypes
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 view_direction);

//Input variables
in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 frag_tex_coords;

//Output variables
out vec4 fragColor;

//Uniforms
uniform vec3 cameraPosition;
uniform Material material;
//uniform PointLight light;
uniform SpotLight light;

/*
 *
 * MAIN FUNCTION
 *
 */
void main()
{
	//Normal vector to fragment
	vec3 normal = normalize(fragmentNormal);
	//Vector from fragment to camera
	vec3 view_direction = normalize(cameraPosition - fragmentPosition);

//	fragColor = vec4(CalculatePointLight(light, normal, view_direction), 1.0);
	fragColor = vec4(CalculateSpotLight(light, normal, view_direction), 1.0);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 view_direction)
{
	//Calculate constant ambient light contribution to color
	vec3 ambient_light = (light.ambient_intensity * light.color) * vec3(texture(material.diffuse_sampler, frag_tex_coords));

	//Calculate diffuse light contribution based on surface normal and light position
	vec3 light_direction = normalize(light.position - fragmentPosition);
	float diffuse_projection = max(dot(normal, light_direction), 0.0);
	vec3 diffuse_light = (light.diffuse_intensity * light.color) * vec3(texture(material.diffuse_sampler, frag_tex_coords)) * diffuse_projection;

	//Calculate specular light contribution based on reflected light ray and camera position
	vec3 reflect_direction = reflect(-light_direction, normal);

	float specular_projection = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = (light.specular_intensity * light.color) * material.specular_color * specular_projection;

	//Calculate attenuation factor based on distance from light
	float light_distance = length(light.position - fragmentPosition);
	float attenuation_value = 1.0f / (light.attenuation_constant + light.attenuation_linear * light_distance + light.attenuation_quadratic * pow(light_distance, 2));

	//Without attenuation
//	vec3 result = ambient_light + diffuse_light + specular_light;

	//With attenuation
	vec3 result = attenuation_value*(ambient_light + diffuse_light + specular_light);

	return result;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);

    //Calculate diffuse shading
    float diffuse_projection = max(dot(normal, light_direction), 0.0);
    //Calculate specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_projection = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    //Calculate resulting components and sum
    vec3 ambient_light  = (light.ambient_intensity * light.color)  * vec3(texture(material.diffuse_sampler, frag_tex_coords));
    vec3 diffuse_light  = (light.diffuse_intensity * light.color)  * diffuse_projection * vec3(texture(material.diffuse_sampler, frag_tex_coords));
    vec3 specular_light = (light.specular_intensity * light.color) * specular_projection * material.specular_color;
    
	return (ambient_light + diffuse_light + specular_light);
} 

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - fragmentPosition);
	float cos_theta = dot(light_direction, normalize(-light.direction));

	float cos_epsilon = light.cos_inner_cutoff_angle - light.cos_outer_cutoff_angle;
	float light_intensity = clamp((cos_theta - light.cos_outer_cutoff_angle) / cos_epsilon, 0.0, 1.0);

	//Calculate diffuse shading
    float diffuse_projection = max(dot(normal, light_direction), 0.0);
    //Calculate specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_projection = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

	//Calculate attenuation factor based on distance from light
	float light_distance = length(light.position - fragmentPosition);
	float attenuation_value = 1.0f / (light.attenuation_constant + light.attenuation_linear * light_distance + light.attenuation_quadratic * pow(light_distance, 2));

    //Calculate resulting components and sum
    vec3 ambient_light  = (light.ambient_intensity * light.color)  * vec3(texture(material.diffuse_sampler, frag_tex_coords));
    vec3 diffuse_light  = (light.diffuse_intensity * light.color)  * diffuse_projection * vec3(texture(material.diffuse_sampler, frag_tex_coords));
    vec3 specular_light = (light.specular_intensity * light.color) * specular_projection * material.specular_color;

	diffuse_light *= light_intensity;
	specular_light *= light_intensity;

	return attenuation_value*(ambient_light + diffuse_light + specular_light);
}