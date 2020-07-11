#version 460 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_normal;
out vec3 frag_position;
out vec2 coord;

void main()	 {
	gl_Position = projection * view * model * vec4(position, 1.0);
	frag_normal = mat3(transpose(inverse(model))) * normal;
	frag_position = vec3(model * vec4(position, 1.0));
	coord = tex_coord;
}


