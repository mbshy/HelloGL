#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int createBasicProgram(const char* vertexShaderSource, const char* fragmentShaderSource, unsigned int* err);


int main()
{

	// GLFW Initializations
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Loading OpenGl methods
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set size of rendering window
	glViewport(0, 0, 800, 600);
	// Set framebuffer size change callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// First `Vertex Shader` in GLSL(OpenGL Shading Language)
	// we are declaring inpute vertex attribute with `in` keyword
	// we use `vec3` because each vertex has a 3D coordinate
	// we name it `aPos`
	// we set the location of vertex attribute to location 0 via `layout (location = 0)`
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// First `Fragment Shader` in GLSL
	// we are defining one output variable of vec4
	// it defines the final color of Orange
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = ourColor;\n"
		"}\n";
	
	unsigned int err = 0;
	unsigned int shaderProgram = createBasicProgram(vertexShaderSource, fragmentShaderSource, &err);
	if (err != 0) {
		std::cout << "there was some error: " << err << std::endl;
		return err;
	}


	float vertices[] = {
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  0.0f,  0.5f, 0.0f
	};

	// Vertex buffer boject
	unsigned int VBO;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind vertex array
	glBindVertexArray(VAO);

	// Bind buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy data to currently bound buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// telling OpenGL how it should interpret the vertex data (per vertex attribute)
	// the first attribute specifies which arribute we want to configure, we use the same location we used in vertex shader source
	// the second arg specifies the size of the vertex attribute which is vec3
	// third is the type of data
	// fourth is normalization enable/disable
	// the fifth is `stride` and specifies the space between consecutive vertex attributes
	// the sixth is the offset where the the position data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// Keep drawing
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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


unsigned int createBasicProgram(const char* vertexShaderSource, const char* fragmentShaderSource, unsigned int* err)
{
	// These 2 variables are for compile errors
	int success;
	char infoLog[512];


	// creating vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach the source code
	// the second argument is how many strings we are passing as a source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

	// compile shader
	glCompileShader(vertexShader);

	// Check for any compilation error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		*err = 1;
		return 0;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		*err = 2;
		return 0;
	}


	// Shader Program, it simply an object that link shaders together
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for any linking error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINK::LINKING_FAILED\n" << infoLog << std::endl;
		*err = 3;
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	*err = 0;
	return shaderProgram;
}