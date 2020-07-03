#version 460 core

out vec4 fragment_color;

uniform vec3 entity_color;
uniform vec3 light_color;

uniform vec3 light_position;
uniform vec3 view_position;

in vec3 frag_normal;
in vec3 frag_position;

void main() {
	float ambient = 1.0;

	vec3 light_direction = normalize(frag_position - light_position);
	vec3 frag_norm = normalize(frag_normal);
	float diffuse = max(dot(frag_norm, light_direction), 0.0);

	float specular_strength = 0.5;
	vec3 lookat_direction = normalize(view_position - frag_position);
	vec3 reflect_vec = reflect(-light_direction, frag_norm);
	float specular = pow(max(dot(lookat_direction, reflect_vec), 0.0), 32) * specular_strength;


	float light_coefficient = ambient + diffuse + specular;
	vec3 light = light_color * light_coefficient;

	fragment_color = vec4(entity_color * light, 1.0);
}