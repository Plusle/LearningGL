#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;


void main() {    
    vec4 texColor = texture(texture0, TexCoords);
    if (texColor.a < 0.1) {
        discard;
    }
    // float average = (texColor.r + texColor.g + texColor.b) / 3;
    // FragColor = vec4(average, average, average, texColor.a);

    FragColor = texColor;
}