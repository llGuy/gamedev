#pragma once

#include <string>
#include <unordered_map>

#include "render_stage.h"
#include "render_stage3D.h"
#include "render_stage2D.h"
#include "../renderbuffers/renderbuffer_handler.h"

#define RENDER_STAGE_CREATE_INFO_COLOR_NONE 1 << 0
#define RENDER_STAGE_CREATE_INFO_COLOR_TEXTURE 1 << 1
#define RENDER_STAGE_CREATE_INFO_COLOR_RENDERBUFFER 1 << 2
#define RENDER_STAGE_CREATE_INFO_DEPTH_TEXTURE 1 << 3
#define RENDER_STAGE_CREATE_INFO_DEPTH_RENDERBUFFER 1 << 4

#define TEXTURE2D_BINDING_PREVIOUS_OUTPUT nullptr

struct render_stage_create_info
{
	u32 width, height;

	/* all macros with RENDER_STAGE_CREATE_INFO prefix */
	u32 create_flags;
	
	/* for now just supports 1 color attachment */
	std::string color_name;
	std::string depth_name;
};

class pipeline
{
private:

	std::unordered_map<std::string, u32> render_stage_indices;
	std::vector<render_stage *> render_stages;

public:

	auto execute_stages(u32 display_w, u32 display_h) -> void;

	/* function to make life easier */
	auto create_render_stage(std::string const & stage_name
		, render_stage_create_info const & create_info
		, renderbuffer_handler & renderbuffers
		, texture_handler & textures) -> void;

	auto get_render_stage_id(std::string const & name) -> u32;

	auto operator[](std::string const & name) -> render_stage *;

	auto operator[](u32 id) -> render_stage *;

	template <typename T, typename ... Args>
	auto add_render_stage(std::string const & name, Args && ... args) -> T *
	{
		T * new_stage = new T(std::forward<Args>(args)...);

		render_stage_indices[name] = render_stages.size();
		render_stages.push_back(new_stage);

		return new_stage;
	}
};