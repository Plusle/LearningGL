#version 460 core

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D scene;

const float offset = 1 / 200.0;

void main() {
	
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	vec2(0, offset),   vec2(offset, offset),
		vec2(-offset, 0),       vec2(0, 0),        vec2(offset, 0),
		vec2(-offset, -offset), vec2(0, -offset),  vec2(offset, -offset)
	);

	float kernel[9] = float[9](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 sample_colors[9];
	for (int i = 0; i < 9; i++) {
		sample_colors[i] = vec3(texture(scene, TexCoord.st + offsets[i]));
	}

	vec3 final_color = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		final_color += kernel[i] * sample_colors[i];
	}

	FragColor = vec4(final_color, 1.0);
}