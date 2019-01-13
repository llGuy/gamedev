#pragma once

#include <glm/glm.hpp>
#include "../../../api/api.h"
#include "../../../scene/camera.h"
#include "../../3D/model_handler.h"
#include "../../../light/light_handler.h"
#include "../../../api/uniform_buffer.h"
#include "../../../time/timer.h"

class material;

/* describes exactly how to render a model 
   not necessarilly just for lighting */
/* in new version of the game engine, this class is also the renderer */
class material_prototype
{
protected:
	camera * bound_cam;
	glsl_program * shader;

	uniform_buffer material_block;
	material_light_info light_info_receive;

	light_handler * lights;
	std::string material_type_name;

	std::vector<texture *> textures2D;
	std::vector<texture *> textures_cubemap;

	std::vector<material *> materials;
	
	bool enabled_lighting{ true };

	bool flush_each_frame{ true };
public:
	material_prototype(std::string const & name)
		: material_block(MATERIAL_PROTOTYPE_BLOCK_INDEX)
		, material_type_name(name)
	{
	}

	material_prototype(material_light_info const & light_info, glsl_program * shader, light_handler & lights, std::string const & name)
		: shader(shader)
		, lights(&lights)
		, light_info_receive(light_info)
		, material_type_name(name)
		, material_block(MATERIAL_PROTOTYPE_BLOCK_INDEX)
	{
	}

	auto init(void) -> void;
	
	auto submit_material(material * mat) -> void;
	auto render(camera & scene_camera, timer & timeh) -> void;
	auto flush(void) -> void;

	virtual auto prepare(camera & scene_camera, timer & timeh) -> void;
	
	auto get_shader(void) -> glsl_program * &;
	auto get_name(void) -> std::string &;
	auto get_textures_2D(void) -> std::vector<texture *> &;
	auto get_textures_cubemap(void) -> std::vector<texture *> &;
	auto toggle_lighting(void) -> void;
	auto get_light_info(void) -> material_light_info &;
	auto get_light_handler_ptr(void) -> light_handler * &;
	auto get_flush_each_frame(void) -> bool &;
	auto is_lit(void) -> bool &;

	template <typename ... T> auto set_texture_2D(T ... textures) -> void
	{
		(textures2D.push_back(textures), ...);
	}

	template <typename ... T> auto set_texture_3D(T ... textures) -> void
	{
		(textures_cubemap.push_back(textures), ...);
	}
	auto update_uniform_block(void) -> void;
};

/* controled probably by an entity (game object) */
struct transparency
{
	bool yes;
	GLenum sfactor;
	GLenum dfactor;
};

class material
{
protected:
	friend class renderer3D;
	friend class material_handler;

	model & renderable;
	glm::mat4 transform_matrix;

	u32 material_type_id;

	transparency is_transparent;
public:
	material(model & renderable, glm::mat4 const & transform, u32 material_type_id, transparency transp);

	virtual auto render(glsl_program * shader) -> void;

	inline auto get_transform(void) -> glm::mat4 &
	{
		return transform_matrix;
	}

	inline auto get_is_transparent(void) -> transparency &
	{
		return is_transparent;
	}
};

#define MATERIAL_ONLY_REFLECTION    material_light_info{ glm::vec3(1.0f), glm::vec3(0.7f), glm::vec3(0.5f), 10.0f, 1.0f }
#define MATERIAL_HIGHLY_REFLECTIVE  material_light_info{ glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.5f), 20.0f, 0.2f }
#define MATERIAL_NOT_TOO_REFLECTIVE material_light_info{ glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.2f), 30.0f, 0.2f }
#define MATERIAL_NO_REFLECTIONS material_light_info{ glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.2f), 30.0f, 0.0f }