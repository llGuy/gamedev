#pragma once

#include <vector>
#include <bitset>
#include "../../io/io.h"
#include "../../api/api.h"
#include "shader_properties.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../json_loader.h"

class shader_handle
{
private:
	std::vector<shader_property> properties_list;
	std::bitset<32> flags;
	std::string name;
public:
	shader_handle(void) = default;

	shader_handle(std::string const & name)
		: name(name)
	{
	}

	auto set_name(std::string const & str) -> void
	{
		name = str;
	}
	auto size(void) const -> u32
	{
		return properties_list.size();
	}
	template <typename ... Ts> auto set(Ts ... properties) -> void
	{
		((flags[static_cast<u32>(properties)] = true), ...);
		(properties_list.push_back(properties), ...);
	}
	auto index_in(u32 i) const -> shader_property
	{
		return properties_list[i];
	}
	auto operator[](shader_property property)
	{
		return flags[static_cast<u32>(property)];
	}
	auto operator[](shader_property property) const
	{
		return flags[static_cast<u32>(property)];
	}
	auto str(void) const -> std::string const &
	{
		return name;
	}
	auto str(void) -> std::string &
	{
		return name;
	}
	auto bit_set(void) const -> std::bitset<32>
	{
		return flags;
	}
	auto operator==(shader_handle const & other) -> bool
	{
		return (name == other.name && flags == other.flags);
	}
};

static auto operator==(shader_handle const & left, shader_handle const & right) -> bool
{
	return (left.str() == right.str());
}

namespace std {
	template <> struct hash<shader_handle>
	{
		auto operator()(shader_handle const & handle) const -> size_t
		{
			return hash<string>()(handle.str());
		}
	};
}

struct shader_load_functor
{
	struct data_type { nlohmann::json * handle; nlohmann::json::iterator * iterator; };

	auto apply(data_type & data) -> void {}
};

class shader_handler
{
private:
	struct
	{
		std::unordered_map<GLenum, std::string> srcs;
	} master2D_srcs;

	struct
	{
		std::unordered_map<GLenum, std::string> srcs;
	} master3D_srcs;

	std::unordered_map<shader_handle, u32> program_index_map;

	std::vector<glsl_program *> programs;

	json_loader<shader_load_functor> loader;
public:
	shader_handler(void) = default;

	auto init(void) -> void
	{
		master2D_srcs.srcs[GL_VERTEX_SHADER] = extract_file("src/shaders/master/master2D_vsh.shader");
		master2D_srcs.srcs[GL_FRAGMENT_SHADER] = extract_file("src/shaders/master/master2D_fsh.shader");

		master3D_srcs.srcs[GL_VERTEX_SHADER] = extract_file("src/shaders/master/master3D_vsh.shader");
		master3D_srcs.srcs[GL_GEOMETRY_SHADER] = extract_file("src/shaders/master/master3D_gsh.shader");
		master3D_srcs.srcs[GL_FRAGMENT_SHADER] = extract_file("src/shaders/master/master3D_fsh.shader");
	}

	auto load_from_json(void) -> void
	{
		loader.load(extract_file("res/saves/shaders.json")
			, [this](shader_load_functor::data_type & data) -> void {

			auto int_uniform = [](nlohmann::json::iterator & it, glsl_program & shader) 
			{
				shader.bind();
				for (nlohmann::json::iterator uniform = it.value().begin(); uniform != it.value().end(); ++uniform)
				{
					shader.send_uniform_int(uniform.key(), uniform.value());
				}
			};

			auto float_uniform = [](nlohmann::json::iterator & it, glsl_program & shader)
			{
				shader.bind();
				for (nlohmann::json::iterator uniform = it.value().begin(); uniform != it.value().end(); ++uniform)
				{
					shader.send_uniform_float(uniform.key(), uniform.value());
				}
			};

			auto vec3_uniform = [](nlohmann::json::iterator & it, glsl_program & shader)
			{
				shader.bind();
				for (nlohmann::json::iterator uniform = it.value().begin(); uniform != it.value().end(); ++uniform)
				{
					std::vector<float> vec_array = uniform.value();
					glm::vec3 vec3 = glm::vec3(vec_array[0], vec_array[1], vec_array[2]);
					shader.send_uniform_vec3(uniform.key(), glm::value_ptr(vec3), 1);
				}
			};

			std::unordered_map<std::string, std::function<void(nlohmann::json::iterator &, glsl_program &)>> uniform_funcs
			{
				std::pair("int", int_uniform),
				std::pair("float", float_uniform),
				std::pair("vec3", vec3_uniform)
			};

			std::unordered_map<std::string, GLenum> map_shader_type
			{
				std::pair("vsh", GL_VERTEX_SHADER)
				, std::pair("fsh", GL_FRAGMENT_SHADER)
				, std::pair("gsh", GL_GEOMETRY_SHADER)
			};

			std::string shader_name = "shader." + std::string(data.iterator->key());

			shader_handle handle(shader_name);

			std::vector<glsl_shader> shaders;

			auto src_node = data.iterator->value().find("src");

			for (nlohmann::json::iterator src_it = src_node.value().begin()
				; src_it != src_node.value().end(); ++src_it)
			{
				shaders.push_back(create_shader(map_shader_type[src_it.key()], handle, extract_file(src_it.value())));
			}

			auto shader = combine(handle, false, shaders);

			auto send_uniform_node = data.iterator->value().find("send");
			if (send_uniform_node != data.iterator->value().end())
			{
				for (nlohmann::json::iterator uniform_it = send_uniform_node.value().begin()
					; uniform_it != send_uniform_node.value().end(); ++uniform_it)
				{
					auto key = uniform_it.key();
					uniform_funcs[key](uniform_it, *shader);
				}
			}

		});
	}

	auto create_program(shader_handle const & handle, std::string const & shader /* 3D or 2D */, bool prepend_version = true) -> glsl_program *
	{
		glsl_program * ret;
		if (shader == "2D")
		{
			glsl_shader vsh = create_shader(GL_VERTEX_SHADER, handle, shader);
			glsl_shader fsh = create_shader(GL_FRAGMENT_SHADER, handle, shader);
			ret = combine(handle, prepend_version, vsh, fsh);
		}
		else if (shader == "3D")
		{
			glsl_shader vsh = create_shader(GL_VERTEX_SHADER, handle, shader);
			glsl_shader fsh = create_shader(GL_FRAGMENT_SHADER, handle, shader);

			if (handle[shader_property::linked_to_gsh] == true)
			{
				glsl_shader gsh = create_shader(GL_GEOMETRY_SHADER, handle, shader);
				ret = combine(handle, prepend_version, vsh, gsh, fsh);
			}
			else ret = combine(handle, prepend_version, vsh, fsh);
		}
		else ret = nullptr;

		return ret;
	}

	/* src_provided is either "2D" for 2D shader, "3D" for 3D shader or a custom raw src */
	auto create_shader(GLenum type, shader_handle const & handle, std::string const & src_provided) -> glsl_shader
	{
		glsl_shader new_shader(type);
		for (u32 i = 0; i < handle.size(); ++i)
		{
			new_shader.provide(property_map[static_cast<u32>(handle.index_in(i))]);
		}

		if (src_provided == "2D")
		{
			new_shader.provide(master2D_srcs.srcs.at(type));
		}
		else if (src_provided == "3D")
		{
			new_shader.provide(master3D_srcs.srcs.at(type));
		}
		else
		{
			new_shader.provide(src_provided);
		}

		return new_shader;
	}

	template <typename ... Ts> auto combine(shader_handle const & handle, bool prepend_version, Ts ... shaders) -> glsl_program *
	{
		glsl_program * program = new glsl_program;
		(program->attach(handle.str(), shaders, prepend_version), ...);
		program->link();

		u32 id = programs.size();

		program_index_map[handle] = id;

		programs.push_back(program);

		return program;
	}

	auto combine(shader_handle const & handle, bool prepend_version, std::vector<glsl_shader> & shaders) -> glsl_program *
	{
		glsl_program * program = new glsl_program;

		for (auto shader : shaders)
		{
			program->attach(handle.str(), shader, prepend_version);
		}
		program->link();

		u32 id = programs.size();

		program_index_map[handle] = id;

		programs.push_back(program);

		return program;
	}

	auto get_index(shader_handle const & handle) -> u32
	{
		return program_index_map[handle];
	}

	auto operator[](shader_handle const & handle) -> glsl_program *
	{
		return programs[program_index_map.at(handle)];
	}

	auto operator[](u32 index) -> glsl_program *
	{
		return programs[index];
	}
};