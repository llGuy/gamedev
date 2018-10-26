#pragma once

#include <glm/glm.hpp>
#include "../../../api/api.h"
#include "../../3D/model_handler.h"
#include "../../../light/light_handler.h"

/* describes exactly how to render a model */
class material_prototype
{
private:
	glsl_program * shader;
	material_light_info light_info_receive;
	light_handler * lights;

	std::vector<texture *> textures;
public:
	material_prototype(void) = default;

	/* T ... must be pointers */
	template <typename ... T> material_prototype(material_light_info const & light_info, glsl_program * shader, light_handler & lights, T ... textures)
		: shader(shader), lights(&lights), textures{ textures... }, light_info_receive(light_info)
	{
	}

	auto prepare(void) -> void;

	auto get_shader(void) -> glsl_program * &;
};

/* controled probably by an entity (game object) */
class material
{
private:
	friend class renderer3D;

	model renderable;
	glm::mat4 transform_matrix;

	material_prototype * prototype;
public:
	material(model const & renderable, glm::mat4 const & transform);

	auto render(void) -> void;

	inline auto get_transform(void) -> glm::mat4 &
	{
		return transform_matrix;
	}
};