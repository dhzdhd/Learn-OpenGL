// Include glad before glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	glfwInit();
	// Set GLFW version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Explicitly use the core-profile (no backwards compatibility)
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(2560, 1440, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // Clean up
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Register callback to adjust viewport on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Triangle vertices
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,   // First triangle
		1, 2, 3    // Second triangle
	};

	// Read shader code from file
	std::ifstream ifs("src/shaders/triangle.vert");
	std::string vert((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	std::ifstream ifs1("src/shaders/triangle.frag");
	std::string frag((std::istreambuf_iterator<char>(ifs1)),
		(std::istreambuf_iterator<char>()));
	const char* vert_c = vert.c_str();
	const char* frag_c = frag.c_str();

	// Create shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Replace shader source code with the one from file
	glShaderSource(vertexShader, 1, &vert_c, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &frag_c, NULL);
	glCompileShader(fragmentShader);

	// Check for compilation errors 
	int success;
	char infoLog[512];  // Error message buffer
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create shader program and link shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Delete shader objects after linking as they are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex buffer object to batch data to GPU and vertex array object to tell OpenGL how to interpret vertex data
	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); // Bind VAO first then bind and set VBOs and then configure vertex attributes

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind EBO to GL_ELEMENT_ARRAY_BUFFER type
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Tell OpenGL how to interpret vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO 
	// as the vertex attribute's bound vertex buffer object so afterwards 
	// we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify 
	// this VAO, but this rarely happens. 
	// Modifying other VAOs requires a call to glBindVertexArray anyways 
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// Wireframe mode, change to GL_FILL for filled triangles
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop	
	while (!glfwWindowShouldClose(window)) {
		// Process input events
		processInput(window);

		// Clear the screen
		glClearColor(0.2f, 0.7f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader program when we want to render an object
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
