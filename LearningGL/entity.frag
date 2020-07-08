#version 460 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	// here should be a so-called illumination decay rate, maybe
	//float decay
};

in vec3 frag_normal;
in vec3 frag_position;

out vec4 fragment_color;

uniform Material surface;
uniform Light cube_light;
uniform vec3 view_position;

void main() {
	// ambient
	vec3 ambient = cube_light.ambient * surface.ambient;
	
	// diffuse
	vec3 normal = normalize(frag_normal);
	vec3 light_direction = normalize(frag_position - cube_light.position);
	float diff_coefficient = max(0.0, dot(-light_direction, normal));
	vec3 diffuse = cube_light.diffuse * (diff_coefficient * surface.diffuse);

	// specular
	vec3 view_direction = normalize(frag_position - view_position);
	float spec_coefficient = pow(max(0.0, dot(-view_direction, normal)), surface.shininess);
	vec3 specular = cube_light.specular * (spec_coefficient * surface.specular);

	// composite
	vec3 color = ambient + diffuse + specular;
	fragment_color = vec4(color, 1.0);	
}