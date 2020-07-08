#version 460 core

struct Material {
	sampler2D diffuse_map;
	sampler2D specular_map;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;

	float constant;
	float linear;
	float quadratic;
	// here should be a so-called illumination decay rate, maybe
	//float decay
};

in vec3 frag_normal;
in vec3 frag_position;
in vec2 coord;

out vec4 fragment_color;

uniform Material surface;
uniform Light cube_light;
uniform vec3 view_position;

void main() {
	vec3 diffuse_map = vec3(texture(surface.diffuse_map, coord));
	
	// ambient
	vec3 ambient = cube_light.ambient * diffuse_map;
	
	// diffuse
	vec3 normal = normalize(frag_normal);
	vec3 light_direction = normalize(frag_position - cube_light.position);
	float diff_coefficient = max(0.0, dot(-light_direction, normal));
	vec3 diffuse = cube_light.diffuse * (diff_coefficient * diffuse_map);

	// specular
	vec3 view_direction = normalize(frag_position - view_position);
	vec3 reflection = reflect(light_direction, normal);
	float specular_coefficient = pow(max(dot(reflection, -view_direction), 0.0), surface.shininess);
	vec3 specular = cube_light.specular * specular_coefficient * texture(surface.specular_map, coord).rgb;

	// attenuation
	float distans = length(cube_light.position - frag_position);
	float attenuation = 1 / (cube_light.constant + distans * cube_light.linear + pow(distans, 2) * cube_light.quadratic);

	// composite
	vec3 color = (ambient + diffuse + specular) * attenuation;


	fragment_color = vec4(color, 1.0);	
}