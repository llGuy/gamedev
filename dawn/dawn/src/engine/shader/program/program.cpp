#include "program.h"

namespace dawn {

	void SHProgram::UseProgram(void)
	{
		glUseProgram(m_programID);
	}
	void SHProgram::Compile(void)
	{
		utils::Log("compiling shaders");
		for (auto& sh : m_shaders)
			sh.Init();
	}
	const uint32_t& SHProgram::ProgramID(void)
	{
		return m_programID;
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