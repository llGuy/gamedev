#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

uint32_t CreateTriangle(void)
{
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 color;
	};

	Vertex vertices[]
	{
		// attribute 0 position				attribute 1 color
		Vertex{ glm::vec2(-1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		Vertex{ glm::vec2(+0.0f, +1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		Vertex{ glm::vec2(+1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
	};

	uint32_t bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 2));

	return bufferID;
}

uint32_t ShaderProgram(void)
{
	std::ifstream vshfileStream("src\\vsh.shader");
	std::ifstream fshfileStream("src\\fsh.shader");

	std::string vshSources = std::string(
		std::istreambuf_iterator<char>(vshfileStream),
		std::istreambuf_iterator<char>()
	);
	std::string fshSources = std::string(
		std::istreambuf_iterator<char>(fshfileStream),
		std::istreambuf_iterator<char>()
	);
	
	uint32_t vshID, fshID;
	vshID = glCreateShader(GL_VERTEX_SHADER);
	fshID = glCreateShader(GL_FRAGMENT_SHADER);
	
	const char* vshsrcs[1] = { vshSources.c_str() };
	const char* fshsrcs[1] = { fshSources.c_str() };

	glShaderSource(vshID, 1, vshsrcs, 0);
	glShaderSource(fshID, 1, fshsrcs, 0);

	glCompileShader(vshID);
	glCompileShader(fshID);


	uint32_t programID;
	programID = glCreateProgram();
	glAttachShader(programID, vshID);
	glAttachShader(programID, fshID);

	glBindAttribLocation(programID, 0, "vertex_position");
	glBindAttribLocation(programID, 1, "vertex_color");

	glLinkProgram(programID);

	glUseProgram(programID);

	return programID;
}

int main(int argc, char* argv[])
{
	// initializing glfw
	if (!glfwInit())
	{
		std::cerr << "failed to init glfw" << std::endl;
		glfwTerminate();
		return -1;
	}

	GLFWwindow* window;
	window = glfwCreateWindow(1700, 1200, "fuck machine", 0, 0);
	glfwMakeContextCurrent(window);
	
	// initializing glew
	GLenum errCode = glewInit();
	if (errCode != GLEW_OK)
	{
		std::cerr << "failed to init glew" << std::endl;
		glfwTerminate();
		return -1;
	}

	uint32_t triangle = CreateTriangle();
	uint32_t program = ShaderProgram();

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		// i am drawing triangles which means that 
		// when i specify the 3 vertices, openGL will fill
		// in all the vertices in the triangle with a color (or a blend of color)
		// glDrawArrays draws each vertex one by one
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}