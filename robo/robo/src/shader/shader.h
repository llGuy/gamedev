#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "..\log.h"

class Shader
{
public:
	explicit Shader(GLenum shader, const std::string& directory)
		: m_shaderType(shader)
	{
		ExtractSrc(directory);
	}
	~Shader(void)
	{
	}
public:
	void Init(void)
	{
		Log("intitializing shader");
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
			Log("shader file failed to load");
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
	GLenum m_shaderType;
};

#endif