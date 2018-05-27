#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>
#include <GL/glew.h>
#if (defined cpp_no_optional)
#include "optional.hpp"
#else
#include <optional>
#endif

namespace mulgame {
	    
    class Shader
    {
    public:
	Shader(void) = default;
	Shader(GLenum shadertype);
	~Shader(void) = default;

	void Compile(const std::string& directory);
    public:
	inline
	uint32_t ShaderID(void) const
	{
	    return m_shaderID;
	}

	// temporary
	inline
	GLenum Type(void) const
	{
		return m_shadertype;
	}
    private:
	std::optional<std::string> ExtractSource(const std::string& directory);
    private:
	//union
	//{
	    uint32_t m_shaderID;
	    GLenum m_shadertype;
	//};
    };

}

#endif /* _SHADER_H_ */
