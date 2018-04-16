#ifndef SHADER_HEADER
#define SHADER_HEADER
#include <GL/glew.h>
#include <fstream>
#include "../../utils/logger/log.h"
#include "../../utils/logger/error.h"
#include "program/shader_base.h"

namespace dawn {

	class Shader
	{
	public:
		Shader(void) = default;
		Shader(const ShaderBase& base)
			: m_shaderType(base.type)
		{
			switch (base.type)
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
			ExtractSrc(base.dir);
		}
		~Shader(void) = default;
	public:
		void Init(void)
		{
			std::string log = std::string("initializing ") + m_strShaderType;
			utils::Log(log);
			m_shaderID = glCreateShader(m_shaderType);
			const char* srcs[1] = { m_src.c_str() };
			glShaderSource(m_shaderID, 1, srcs, 0);
			glCompileShader(m_shaderID);
		}
		uint32_t& ShaderID(void)
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
				utils::Error(log);
			}
			std::string log = m_strShaderType + " load success";
			utils::Log(m_strShaderType);
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
		uint32_t m_shaderID;
		std::string m_src;
		std::string m_strShaderType;
		GLenum m_shaderType;
	};

}

#endif