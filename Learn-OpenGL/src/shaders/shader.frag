#version 330 core

// in vec4 vertexColor;  // Get color from vertex shader
uniform vec4 customColor;  // Get color from C++ code

out vec4 fragColor;

void main() {
	// fragColor = vertexColor;
	fragColor = customColor;
}
