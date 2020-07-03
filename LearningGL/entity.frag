#version 460 core

out vec4 fragment_color;

uniform vec3 entity_color;
uniform vec3 light_color;

uniform vec3 light_position;
uniform vec3 view_position;

in vec3 frag_normal;
in vec3 frag_position;

void main() {
	float ambient_co, diffuse_co, specular_co = 0.0;

	// ambient part
	float ambient_strength = 0.1;
	ambient_co = ambient_strength;

	// diffuse part
	vec3 normal = normalize(frag_normal);
	vec3 light_direction = normalize(frag_position - light_position);
	diffuse_co = max(0.0, dot(-light_direction, normal));

	// specular part
	float specular_strength = 0.5;
	vec3 reflection = reflect(light_direction, normal);
	vec3 inverse_lookat = normalize(view_position - frag_position);
	specular_co = pow(max(0.0, dot(reflection, inverse_lookat)), 32) * specular_strength;

	vec3 light = (ambient_co + diffuse_co + specular_co) * light_color;
	fragment_color = vec4(entity_color * light, 1.0);
	
}