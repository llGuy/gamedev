#ifndef SHPROGRAM_HEADER
#define SHPROGRAM_HEADER

#include "shader.h"
#include <vector>

namespace sh
{
	class SHProgram
	{
	public:
		explicit SHProgram(void) = default;
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
		void Init(const std::string& vshDir,
			const std::string& fshDir, const std::string& gshDir)
		{
			m_vsh = Shader(GL_VERTEX_SHADER, vshDir);
			m_fsh = Shader(GL_FRAGMENT_SHADER, fshDir);
			m_gsh = Shader(GL_GEOMETRY_SHADER, gshDir);
		}
		void UseProgram(void)
		{
			glUseProgram(m_programID);
		}
		void Compile(void)
		{
			Log("compiling shaders");
			m_vsh.Init();
			m_fsh.Init();
			m_gsh.Init();
		}
		void Link(const std::vector<const char*>& locs)
		{
			Log("linking shaders");
			if (CheckShaderStatus(m_vsh.ShaderID())
				&& CheckShaderStatus(m_fsh.ShaderID())
				&& CheckShaderStatus(m_gsh.ShaderID()))
			{
				m_programID = glCreateProgram();
				AttachShadersToProgram();
				BindAttribLocations(locs);
				glLinkProgram(m_programID);
				DeleteShaders();

			}
			if (CheckProgramStatus(m_programID))
				glUseProgram(m_programID);
		}
		unsigned int ProgramID(void)
		{
			return m_programID;
		}
	private:
		void AttachShadersToProgram(void)
		{
			glAttachShader(m_programID, m_vsh.ShaderID());
			glAttachShader(m_programID, m_fsh.ShaderID());
			glAttachShader(m_programID, m_gsh.ShaderID());
		}
		void BindAttribLocations(const std::vector<const char*>& locs)
		{
			for (unsigned int i = 0; i < locs.size(); ++i)
				glBindAttribLocation(m_programID, i, locs[i]);
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
		bool m_hasGeometry;
	};
}

#endif