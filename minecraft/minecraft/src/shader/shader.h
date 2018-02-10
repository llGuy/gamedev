#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "..\log.h"

namespace sh
{
	class Shader
	{
	public:
		explicit Shader(void) = default;
		explicit Shader(GLenum shader, const std::string& directory)
			: m_shaderType(shader)
		{
			switch (shader)
			{
			case GL_VERTEX_SHADER:
				m_strShaderType = "vertex shader";
				break;
			case GL_FRAGMENT_SHADER:
				m_strShaderType = "fragment shader";
				break;
			case GL_GEOMETRY_SHADER:
				m_strShaderType = "geometry shader";
				break;
			}
			ExtractSrc(directory);
		}
		~Shader(void)
		{
		}
	public:
		void Init(void)
		{
			std::string log = std::string("initializing ") + m_strShaderType;
			Log(log);
			m_shaderID = glCreateShader(m_shaderType);
			const char* srcs[1] = { m_src.c_str() };
			glShaderSource(m_shaderID, 1, srcs, 0);
			glCompileShader(m_shaderID);
		}
		unsigned int& ShaderID(void)
		{
			return m_shaderID;
		}
	private:
		void ExtractSrc(const std::string& directory)
		{
			std::ifstream fileStream(directory);
			if (!fileStream.good())
			{
				std::string log = m_strShaderType + " file : " + directory + " failed to load";
				Error(log);
			}
			std::string log = m_strShaderType + " load success";
			Log(m_strShaderType);
			ReadFromFile(fileStream);
		}
		void ReadFromFile(std::ifstream& fileStream)
		{
			m_src = std::string(
				std::istreambuf_iterator<char>(fileStream),
				std::istreambuf_iterator<char>()
			);
		}
	private:
		unsigned int m_shaderID;
		std::string m_src;
		std::string m_strShaderType;
		GLenum m_shaderType;
	};
}

#endif