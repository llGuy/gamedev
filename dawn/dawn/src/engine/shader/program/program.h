#ifndef PROGRAM_HEADER
#define PROGRAM_HEADER
#include <vector>
#include <functional>
#include <utility>
#include <glm/glm.hpp>
#include "shader_base.h"
#include "udata.h"
#include "../shader.h"

namespace dawn {

    class SHProgram
    {
    public:
	SHProgram(void) = default;
	template<typename... _Sh>
	    SHProgram(_Sh&&... bases)
	    : m_shaders(bases...)
	{
	}
	~SHProgram(void) = default;
    public:
	void Compile(void);
	template<typename... _Sh>
	    void Init(_Sh&&... bases)
	{
	    std::initializer_list<ShaderBase> list {bases...};
	    for (auto& it : list) m_shaders.emplace_back(it);
	}
	template<typename... _Attribs>
	    void Link(_Attribs&&... attribs)
	{
	    utils::Log("linking shaders");
	    bool success = true;
	    for (auto& sh : m_shaders) 
		success &= CheckShaderStatus(sh.ShaderID());
	    if (success)
	    {	
		m_programID = glCreateProgram();
		AttachShadersToProgram();
		BindAttribLocations(attribs...);
		glLinkProgram(m_programID);
		DeleteShaders();

	    }
	    if (CheckProgramStatus(m_programID))
		glUseProgram(m_programID);
	}
	template<typename... _Locs>
	    void GetUniformLocations(_Locs&&... locations /*name of the locations*/)
	{
	    std::initializer_list<UDataLoc> list{ locations... };
	    for (auto& loc : list) m_locations.emplace_back(loc.type, loc.name, GetUniformLocation(loc.name));
	}
	template<typename... _Ptr>
	    void UniformData(_Ptr&&... ptrs)
	{
	    static const std::function<void(float*, const uint32_t&)> funcs[UDType::INV] { uniformMat4f, uniform3f };
	    float* ptrs[sizeof...(ptrs) + 1]{0, ptrs...};
 	    for (uint32_t i = 0; i < m_
		     locations.size(); ++i)
		funcs[m_locations.at(i).type](ptrs[i + 1], m_locations.at(i).location);
	}
	void UseProgram(void);
	inline
	    const uint32_t& ProgramID(void) const
	{
	    return m_programID;
	}
    private:
	inline
	    uint32_t GetUniformLocation(const char* name) const
	{
	    return glGetUniformLocation(m_programID, name);
	}
	static const std::function<void(float*, uint32_t)> uniform3f;
	static const std::function<void(float*, uint32_t)> uniformMat4f;
    private:
	void AttachShadersToProgram(void);
	template<typename... _Attribs>		// const char*
	    void BindAttribLocations(_Attribs&&... attribs)
	{
	    const uint32_t size = sizeof...(attribs);
	    const char* att[size + 1] = { 0, attribs...};
	    for (uint32_t i = 1; i < size + 1; ++i)
		glBindAttribLocation(m_programID, i, att[i]);
	}
	void DeleteShaders(void);
	bool CheckShaderStatus(GLuint shaderID);
	bool CheckProgramStatus(GLuint programID);
    private:
	uint32_t m_programID;
	std::vector<Shader> m_shaders;
	std::vector<UDataLoc> m_locations;
    };

}

#endif
