#version 330 core

in vec3 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	// * mixes texture sample with passed color
	// mix takes 2 texture samples and interpolates between them based on the 3rd argument
	// 0.2 means 80% of the first texture and 20% of the second texture
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) * vec4(vertexColor, 1.0);
}
