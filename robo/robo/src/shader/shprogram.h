#ifndef SHPROGRAM_HEADER
#define SHPROGRAM_HEADER

#include <vector>

#include "shader.h"

class SHProgram
{
public:
	explicit SHProgram(const std::string& vshDir,
		const std::string& fshDir, const std::string& gshDir)
		: m_vsh(GL_VERTEX_SHADER, vshDir),
		m_fsh(GL_FRAGMENT_SHADER, fshDir),
		m_gsh(GL_GEOMETRY_SHADER, gshDir)
	{
	}
	~SHProgram(void)
	{
	}
public:
	void Compile(void) 
	{
		Log("compiling shaders");
		m_vsh.Init();
		m_fsh.Init();
		m_gsh.Init();
	}
	void Link(const std::vector<std::string>& vec)
	{
		Log("linking shaders");
		if (CheckShaderStatus(m_vsh.ShaderID())
			&& CheckShaderStatus(m_fsh.ShaderID()) && CheckShaderStatus(m_gsh.ShaderID()))
		{
			m_programID = glCreateProgram();
			AttachShadersToProgram();
			BindAttribLocations(vec);
			glLinkProgram(m_programID);
			DeleteShaders();
		}
		if (CheckProgramStatus(m_programID))
		{
			glUseProgram(m_programID);
		}
	}
	unsigned int ProgramID(void)
	{
		return m_programID;
	}
	void UseProgram(void)
	{
		glUseProgram(m_programID);
	}
private:
	void AttachShadersToProgram(void)
	{
		glAttachShader(m_programID, m_vsh.ShaderID());
		glAttachShader(m_programID, m_fsh.ShaderID());
		glAttachShader(m_programID, m_gsh.ShaderID());
	}
	void BindAttribLocations(const std::vector<std::string>& vec)
	{
		for (unsigned int i = 0; i < vec.size(); ++i)
			glBindAttribLocation(m_programID, i, vec[i].c_str());
	}
	void DeleteShaders(void)
	{
		glDeleteShader(m_vsh.ShaderID());
		glDeleteShader(m_fsh.ShaderID());
		glDeleteShader(m_gsh.ShaderID());
	}
	bool CheckShaderStatus(GLuint shaderID)
	{
		GLint status;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			GLint infoLogLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));

			GLsizei bufferSize;
			glGetShaderInfoLog(shaderID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
			Log(buffer);

			return false;
		}
		return true;
	}
	bool CheckProgramStatus(GLuint programID)
	{
		GLint status;
		glGetProgramiv(programID, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			GLint infoLogLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = (GLchar*)alloca(infoLogLength * sizeof(GLchar));

			GLsizei bufferSize;
			glGetProgramInfoLog(programID, infoLogLength * sizeof(GLchar), &bufferSize, buffer);
			std::cout << "hi" << std::endl;
			Log(buffer);

			return false;
		}
		return true;
	}
private:
	unsigned int m_programID;

	Shader m_vsh;
	Shader m_fsh;
	Shader m_gsh;
};

#endif