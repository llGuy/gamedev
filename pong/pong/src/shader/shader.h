#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "..\misc.h"

#include <string>
#include <fstream>
#include <GL\glew.h>

class Shader
{
public:
	explicit Shader(GLenum shader, const std::string& fileDir)
		: m_shader(shader)
	{
		ExtractSourceCode(fileDir);
	}
public:
	void Compile(void)
	{
		m_shaderID = glCreateShader(m_shader);
		const char* srcs[1];
		srcs[0] = m_src.c_str();
		glShaderSource(m_shaderID, 1, srcs, 0);
		glCompileShader(m_shaderID);
	}
	unsigned int ShaderID(void)
	{
		return m_shaderID;
	}
private:
	void ExtractSourceCode(const std::string& fileDir)
	{
		std::ifstream file(fileDir);
		if (!file.good())
		{
			Log("file failed to load", fileDir);
		}
		else
		{
			m_src = std::string(
				std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>()
			);
		}
	}
private:
	GLenum m_shader;
	std::string m_src;
	unsigned int m_shaderID;
};

#endif