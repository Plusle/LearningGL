#version 460 core

out vec4 fragment_color;

uniform vec3 entity_color;
uniform vec3 light_color;

uniform vec3 light_position;

in vec3 frag_normal;
in vec3 frag_position;

void main() {
	float ambient = 1.0;

	float diffuse = max(dot(normalize(frag_normal), normalize(frag_position - light_position)), 0.0);

	float specular = 0.0;

	float light_coefficient = ambient + diffuse + specular;
	vec3 light = light_color * light_coefficient;

	fragment_color = vec4(entity_color * light, 1.0);
}