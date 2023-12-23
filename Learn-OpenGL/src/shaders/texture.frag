#version 330 core

in vec3 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D customTexture;

void main() {
	FragColor = texture(customTexture, texCoord);
}
