#include <array>
#include <fstream>

#include "shader.h"

namespace mulgame {

    Shader::Shader(GLenum shadertype)
	: m_shadertype(shadertype)
    {
    }

    void Shader::Compile(const std::string& directory)
    {
	m_shaderID = glCreateShader(m_shadertype);
	auto src = ExtractSource(directory);
	if(src.has_value())
	{
	    std::array<const char*, 1> srcs{ src.value().c_str() };
	    glShaderSource(m_shaderID, 1, srcs.data(), 0);
	    glCompileShader(m_shaderID);
	}
    }

    std::optional<std::string> Shader::ExtractSource(const std::string& directory)
    {
	std::ifstream file(directory);
	if(!file.good())
	{
	    std::cerr << "unable to extract from : " << directory << '\n';
	    return std::optional<std::string>();
	}
	else return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }

}
