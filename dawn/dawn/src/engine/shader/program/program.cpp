#include "program.h"

namespace dawn {

	const std::function<void(float*, uint32_t)> SHProgram::uniform3f =
		[&](float* v, const uint32_t& loc) { glUniform3fv(loc, 1, v); };
	const std::function<void(float*, uint32_t)> SHProgram::uniformMat4f =
		[&](float* m, const uint32_t& loc) { glUniformMatrix4fv(loc, 1, GL_FALSE, m); };

	void SHProgram::UseProgram(void)
	{
		glUseProgram(m_programID);
	}
	void SHProgram::Compile(void)
	{
		utils::NewLog("compiling shaders");
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
			utils::Error("failed to initialize shader");
			GLint infoLogLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));
			GLsizei bufferSize;
			glGetShaderInfoLog(shaderID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
			utils::Error(buffer);
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
			utils::Error("failed to initialize shader program");
			GLint infoLogLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));
			GLsizei bufferSize;
			glGetProgramInfoLog(programID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
			utils::Log(buffer);
			return false;
		}
		return true;
	}

}
