//INSTANCED GEOMETRY

#define GLM_ENABLE_EXPERIMENTAL

#include "glfwwindow.h"
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vertex.h"
#include "camera.h"
#include "ShapeGenerator.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <fstream>

using namespace std;
using glm::vec3;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint teapotNumIndices;
GLuint teapotNormalsNumIndices;
GLuint arrowNumIndices;
GLuint arrowNormalsNumIndices;
GLuint planeNumIndices;
GLuint planeNormalsNumIndices;
Camera camera;
GLuint fullTransformationUniformLocation;

GLuint theBufferID;

GLuint teapotVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArrayObjectID;
GLuint teapotIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint planeIndexByteOffset;

GLuint teapotNormalsVertexArrayObjectID;
GLuint arrowNormalsVertexArrayObjectID;
GLuint planeNormalsVertexArrayObjectID;
GLuint teapotNormalsIndexDataByteOffset;
GLuint arrowNormalsIndexDataByteOffset;
GLuint planeNormalsIndexDataByteOffset;

GLuint vertexArrayID;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//glBindAttribLocation(m_program, 0, "");

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

GLuint cubeNumIndices;
void* offset;

void Window::sendDataToOpenGL()
{
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane(20);

	
	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize(), 0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	teapotIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	arrowIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();

	teapotNumIndices = teapot.numIndices;
	arrowNumIndices = arrow.numIndices;
	planeNumIndices = plane.numIndices;

	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glBindVertexArray(teapotVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = teapot.vertexBufferSize() + teapot.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	
	teapot.DeleteAll();
	arrow.DeleteAll();
	plane.DeleteAll();
}


Window::Window(unsigned int width, unsigned int height, const char* name)
	: m_width(width), m_height(height), m_name(name)
{
	enum 
	{
		INIT_SUCCESS = true,

		INIT_FAIL = false
	};
	if(Initialize() == INIT_FAIL)
	{
		std::cerr << "error !" << std::endl;
	}
}

Window::~Window(void)
{
	glUseProgram(NULL);
	glDeleteProgram(programID);

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

const bool Window::Initialize(void)
{
	if(!glfwInit()) 
	{
		glfwTerminate();
		std::cerr << "failed to initialize GLFW!" << std::endl;
		return false;
	}
	m_window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);

	if(!m_window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	
	if(glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cerr << "failed to initialize GLEW!" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	sendDataToOpenGL();
	installShaders();

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}



void Window::Draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, m_width, m_height);
	glm::vec3 lightPositionWorld(0.0f, 1.0f, 0.0f);
	mat4 mvp;
	mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)m_width) / m_height, 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);
	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	GLint lightUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 light(0.3f, 0.3f, 0.3f);
	glUniform3fv(lightUniformLocation, 1, &lightPositionWorld[0]);
	GLint eyePosition = glGetUniformLocation(programID, "eyePositionWorld") ;
	glm::vec3 eyePositionWorld = camera.GetEyePosition();
	glUniform3fv(eyePosition, 1, &eyePositionWorld[0]);
	GLint modelToWorldTransformMatrixLoc = glGetUniformLocation(programID, "modelToWorldTransformMatrix");
	
	
	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot1ModelToWorldMatrix =
		glm::translate(vec3(-3.0f, 0.0f, -6.0f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	mvp = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexByteOffset);
	/*
	glBindVertexArray(teapotVertexArrayObjectID);
	mat4 teapot2ModelToWorldMatrix =
		glm::translate(vec3(3.0f, 0.0f, -6.75f)) *
		glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	mvp = worldToProjectionMatrix * teapot2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexByteOffset);
	
	// Arrow
	glBindVertexArray(arrowVertexArrayObjectID);
	mat4 arrowModelToWorldMatrix = glm::translate(glm::vec3( 0.0f, 0.0f, -3.0f));
	mvp = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(modelToWorldTransformMatrixLoc, 1, GL_FALSE, &arrowModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);
	
	glBindVertexArray(arrowVertexArrayObjectID);
	arrowModelToWorldMatrix = glm::translate(glm::vec3(0.0f, 3.0f, +4.0)) *glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mvp = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(modelToWorldTransformMatrixLoc, 1, GL_FALSE, &arrowModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);
	*/
	// Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = glm::mat4(1.0f);
	mvp = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(modelToWorldTransformMatrixLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
}


const bool Window::IsWindowOpen(void)
{
	return !glfwWindowShouldClose(m_window);
}

void Window::Update(void)
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	camera.MouseUpdate(glm::vec2(x, y));

	if (glfwGetKey(m_window, GLFW_KEY_W))
	{
		camera.MoveForward();
	}
	if (glfwGetKey(m_window, GLFW_KEY_S))
	{
		camera.MoveBackward();
	}
	if (glfwGetKey(m_window, GLFW_KEY_A))
	{
		camera.MoveLeft();
	}
	if (glfwGetKey(m_window, GLFW_KEY_D))
	{
		camera.MoveRight();
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	{
		camera.MoveUp();
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT))
	{
		camera.MoveDown();
	}
}
