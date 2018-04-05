#ifndef PROGRAM_HEADER
#define PROGRAM_HEADER
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "shader_base.h"
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
		template<typename... _Sh>
		void Init(_Sh&&... bases)
		{
			std::initializer_list<ShaderBase> initlist {bases...};
			for (auto& it : initlist) m_shaders.emplace_back(it);
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


		void UseProgram(void);
		void Compile(void);
		const uint32_t& ProgramID(void);
	public:
		inline
		uint32_t GetUniformLocation(const std::string& name) const
		{
			return glGetUniformLocation(m_programID, name.c_str());
		}
		inline
		void UniformMat4(const glm::mat4& m, const uint32_t& loc) const
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
		}
		inline
		void Uniform3f(const glm::vec3& v, const uint32_t& loc) const
		{
			glUniform3fv(loc, 1, &v[0]);
		}
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
	};

}

#endif