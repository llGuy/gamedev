#include "program.h"

const std::function<void(void*, uint32_t)> SHProgram::uniform3f =
[&](void* v, const uint32_t& loc) { glUniform3fv(loc, 1, static_cast<float*>(v)); };
const std::function<void(void*, uint32_t)> SHProgram::uniformMat4f =
[&](void* m, const uint32_t& loc) { glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<float*>(m)); };

void SHProgram::UseProgram(void)
{
	glUseProgram(m_programID);
}
void SHProgram::Compile(void)
{
	std::cout << ("compiling shaders") << std::endl;
	for (auto& sh : m_shaders)
		sh.Init();
}
void SHProgram::AttachShadersToProgram(void)
{
	for (auto& sh : m_shaders)
		glAttachShader(m_programID, sh.ShaderID());
}
void SHProgram::DeleteShaders(void)
{
	for (auto& sh : m_shaders)
		glDeleteShader(sh.ShaderID());
}
bool SHProgram::CheckShaderStatus(GLuint shaderID)
{
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cout << ("failed to initialize shader") << std::endl;
		GLint infoLogLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
		std::cout << (buffer) << std::endl;
		return false;
	}
	return true;
}
bool SHProgram::CheckProgramStatus(GLuint programID)
{
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		std::cout << ("failed to initialize shader program") << std::endl;
		GLint infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));
		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
		std::cout << (buffer) << std::endl;
		return false;
	}
	return true;
}