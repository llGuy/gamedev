#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#define UP glm::vec3(0.0f, 1.0f, 0.0f)
#define SENSITIVITY 0.02f
#define CAMERA_SPEED 0.01f

bool keys[512] { 0 };

class Camera
{
public:
	Camera(glm::vec2 mousePosition)
		: m_mousePosition(mousePosition), m_position(0.0f, 0.0f, 0.0f), m_direction(0.2f, 0.2f, 0.6f)
	{
	}

	void UpdateDirection(const glm::vec2& cursor)
	{
		glm::vec2 cursorPosDiff = cursor - m_mousePosition;
		m_mousePosition = cursor;

		float xAngle = glm::radians(-cursorPosDiff.x) * SENSITIVITY;
		float yAngle = glm::radians(-cursorPosDiff.y) * SENSITIVITY;

		m_direction = glm::mat3(glm::rotate(xAngle, UP)) * m_direction;

		glm::vec3 rotateYAx = glm::cross(m_direction, UP);
		m_direction = glm::mat3(glm::rotate(yAngle, rotateYAx)) * m_direction;
		m_direction = glm::normalize(m_direction);
	}
	void Move(void)
	{
		if (keys[GLFW_KEY_W]) m_position += glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)) * CAMERA_SPEED;
		if (keys[GLFW_KEY_A]) m_position -= glm::cross(glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)), UP) * CAMERA_SPEED;
		if (keys[GLFW_KEY_S]) m_position -= glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)) * CAMERA_SPEED;
		if (keys[GLFW_KEY_D]) m_position += glm::cross(glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)), UP) * CAMERA_SPEED;
		if (keys[GLFW_KEY_SPACE]) m_position += UP * CAMERA_SPEED;
		if (keys[GLFW_KEY_LEFT_SHIFT]) m_position -= UP * CAMERA_SPEED;
	}
	glm::mat4& UpdateViewMatrix(void)
	{
		m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, UP);
		return m_viewMatrix;
	}
	glm::mat4& ViewMatrix(void)
	{
		return m_viewMatrix;
	}
private:
	glm::vec2 m_mousePosition;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::mat4 m_viewMatrix;
};

/*
	data necessary to render a model
	e.g. buffer, vao, index count, index offset
*/
struct Model
{
	uint32_t buffer;
	uint32_t vao;
	uint32_t count;
	void* offset;
};

/*
	data necessary for the program to be created
*/
struct Shader
{
	uint32_t shader;
	GLenum type;
};

/*
	data necessary for programs to be used
	in order to render objects
*/
struct Program
{
	uint32_t program;
};

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) keys[key] = true;
	else if (action == GLFW_RELEASE) keys[key] = false;
}

GLFWwindow* CreateWindow(int32_t w, int32_t h)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(w, h, "playground", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, KeyCallback);
	return window;
}

Model Quad(void)
{
	#define RADIUS 6.0f

	glm::vec3 vertices[]
	{
		glm::vec3(-RADIUS, 0.0f, -RADIUS),
		glm::vec3(-RADIUS, 0.0f, +RADIUS),
		glm::vec3(+RADIUS, 0.0f, -RADIUS),
		glm::vec3(+RADIUS, 0.0f, +RADIUS)
	};

	uint16_t indices[]
	{
		0, 1, 2,
		2, 1, 3
	};

	// create buffer
	uint32_t buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(indices), nullptr, GL_STATIC_DRAW);	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(indices), indices);

	// create vao
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	return { buffer, vao, 6, (void*)sizeof(vertices) };
}

Model Cube(void)
{
	#define CUBE_RADIUS 0.2f
	glm::vec3 verts[]
	{
		// front
		glm::vec3(-CUBE_RADIUS, -CUBE_RADIUS,  CUBE_RADIUS),
		glm::vec3(CUBE_RADIUS, -CUBE_RADIUS,  CUBE_RADIUS),
		glm::vec3(CUBE_RADIUS,  CUBE_RADIUS,  CUBE_RADIUS),
		glm::vec3(-CUBE_RADIUS,  CUBE_RADIUS,  CUBE_RADIUS),
		// back
		glm::vec3(-CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS),
		glm::vec3(CUBE_RADIUS, -CUBE_RADIUS, -CUBE_RADIUS),
		glm::vec3(CUBE_RADIUS,  CUBE_RADIUS, -CUBE_RADIUS),
		glm::vec3(-CUBE_RADIUS,  CUBE_RADIUS, -CUBE_RADIUS)
	};

	uint16_t indices[]
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	uint32_t buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts) + sizeof(indices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verts), sizeof(indices), indices);

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	return { buffer, vao, 36, (void*)sizeof(verts) };
}

void RenderElements(const Model& data)
{
	glBindVertexArray(data.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.buffer);
	glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_SHORT, data.offset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderArrays(const Model& data)
{
	glBindVertexArray(data.vao);
	glDrawArrays(GL_TRIANGLES, 0, data.count);
	glBindVertexArray(0);
}

bool CheckShaderStatus(uint32_t shader, GLenum type, const std::string& dir)
{
	int32_t status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cerr << dir << " > failed to create shader" << std::endl;
		int32_t infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* buffer = (char*)alloca(infoLogLength);
		int32_t bufferSize;
		glGetShaderInfoLog(shader, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		return false;
	}
	else return true;
}

bool CheckProgramStatus(uint32_t program)
{
	int32_t status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cerr << "failed to create program\n";
		int32_t infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* buffer = (char*)alloca(infoLogLength * sizeof(char));
		GLsizei bufferSize;
		glGetProgramInfoLog(program, infoLogLength * sizeof(char), &bufferSize, buffer);
		std::cerr << buffer << std::endl;
		std::cin.get();
		exit(0);
	}
	return true;
}

Shader CreateShader(const std::string& dir, GLenum type)
{
	uint32_t shader = glCreateShader(type);
	std::string src;
	if (std::ifstream file(dir); !file.good()) std::cerr << "unable to extract from : " << dir << std::endl;
	else src = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	const char* sources[1] { src.c_str() };
	glShaderSource(shader, 1, sources, 0);
	glCompileShader(shader);

	// check errors
	if (CheckShaderStatus(shader, type, dir)) return { shader, type };
	else return { 0, 0 };
}

template<typename... _Ty> Program InitProgram(_Ty... sh)
{
	Shader shaders[] { sh... };
	uint32_t program = glCreateProgram();
	for (uint32_t i = 0; i < sizeof...(sh); ++i) 
		glAttachShader(program, shaders[i].shader);

	return { program };
}

template<typename... _Ty> void BindAttribs(const Program& program, _Ty... attribs)
{
	const char* attrib[] { attribs... };
	for (uint32_t i = 0; i < sizeof...(attribs); ++i)
		glBindAttribLocation(program.program, i, attrib[i]);
}

void Link(const Program& program)
{
	glLinkProgram(program.program);
	CheckProgramStatus(program.program);
}

int main(int argc, char* argv[])
{
	auto window = CreateWindow(1400, 750);
	glEnable(GL_DEPTH_TEST);

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	Camera camera(glm::vec2(static_cast<float>(x), static_cast<float>(y)));

	auto shader1 = CreateShader("res/gen_vsh.shader", GL_VERTEX_SHADER);
	auto shader2 = CreateShader("res/gen_fsh.shader", GL_FRAGMENT_SHADER);
	auto program = InitProgram(shader1, shader2);
	BindAttribs(program, "vertex_position");
	Link(program);
	glUseProgram(program.program);
	uint32_t colorUDataLocation = glGetUniformLocation(program.program, "color");
	uint32_t viewMatrixUDataLocation = glGetUniformLocation(program.program, "view_matrix");
	uint32_t projectionMatrixUDataLocation = glGetUniformLocation(program.program, "projection_matrix");
	uint32_t modelMatrixUDataLocation = glGetUniformLocation(program.program, "model_matrix");

	auto quad = Quad();
	auto cube = Cube();

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), 1400.0f / 750.0f, 0.01f, 500.0f);

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		camera.Move();
		camera.UpdateDirection(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
		camera.UpdateViewMatrix();
		/*
			remember to clear the depth buffer bit. with the depth buffer bit not cleared
			and the depth test enabled, models won't draw
		*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.146078f, 0.6f, 0.9f, 1.0f);

		glm::vec3 color(5.0f, 0.5f, 0.5f);
		decltype(auto) view = camera.ViewMatrix();
		glm::mat4 translation = glm::mat4(1.0f);

		glUniform3fv(colorUDataLocation, 1, &color[0]);
		glUniformMatrix4fv(viewMatrixUDataLocation, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionMatrixUDataLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix4fv(modelMatrixUDataLocation, 1, GL_FALSE, &translation[0][0]);

		RenderElements(quad);

		color = glm::vec3(0.2f, 0.2f, 0.2f);
		translation = glm::translate(glm::vec3(2.0f, 2.0f, 2.0f));

		glUniform3fv(colorUDataLocation, 1, &color[0]);
		glUniformMatrix4fv(modelMatrixUDataLocation, 1, GL_FALSE, &translation[0][0]);

		RenderElements(cube);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	std::cin.get();

	return 0;
}