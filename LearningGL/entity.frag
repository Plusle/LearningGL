#version 460 core

struct Material {
	sampler2D diffuse_map;
	sampler2D specular_map;
	float shininess;
};

struct FlashLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 position;
	vec3 direction;
	float cutoff;
	float decay;
};

struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 position;
};

struct ParallelLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 direction;
};

struct P

in vec3 frag_normal;
in vec3 frag_position;
in vec2 coord;

out vec4 fragment_color;

uniform Material surface;
uniform FlashLight flashlight;
uniform vec3 view_position;

void main() {

	float cutoff = cos(flashlight.cutoff);
	float decay  = cos(flashlight.decay + flashlight.cutoff);
	vec3 frag_direction = normalize(frag_position - flashlight.position);
	float frag_angle = max(0.0, dot(frag_direction, flashlight.direction));
	vec3 color;
	if (frag_angle > decay) {
		vec3 diffuse_map = vec3(texture(surface.diffuse_map, coord));
	
		// ambient
		vec3 ambient = flashlight.ambient * diffuse_map;
	
		// diffuse
		vec3 normal = normalize(frag_normal);
		vec3 light_direction = normalize(frag_position - flashlight.position);
		float diff_coefficient = max(0.0, dot(-light_direction, normal));
		vec3 diffuse = cube_light.diffuse * (diff_coefficient * diffuse_map);

		// specular
		vec3 view_direction = normalize(frag_position - view_position);
		vec3 reflection = reflect(light_direction, normal);
		float specular_coefficient = pow(max(dot(reflection, -view_direction), 0.0), surface.shininess);
		vec3 specular = cube_light.specular * specular_coefficient * texture(surface.specular_map, coord).rgb;

		// attenuation
		float distans = length(flashlight.position - frag_position);
		float attenuation = 1 / (flashlight.constant + distans * flashlight.linear + pow(distans, 2) * flashlight.quadratic);
		color = (ambient + diffuse + specular) * attenuation;

		if (frag_angle < cutoff) color *= (frag_angle - decay) / (cutoff - decay);
	} else {
		color = vec3(0.0);
	}
	fragment_color = vec4(color, 1.0);
}