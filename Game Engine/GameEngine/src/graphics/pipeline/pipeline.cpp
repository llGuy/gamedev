#include "pipeline.h"

auto pipeline::execute_stages(void) -> void
{
	/*std::for_each(render_stages.begin(), render_stages.end()
		, [](render_stage * stage) -> void { 
		stage->clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		stage->execute(); 
	});*/

	for (u32 i = 0; i < render_stages.size(); ++i)
	{
		render_stages[i]->clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_stages[i]->execute(i, render_stages);
	}
}

auto pipeline::create_render_stage(std::string const & stage_name
	, render_stage_create_info const & create_info
	, renderbuffer_handler & renderbuffers
	, texture_handler & textures) -> void
{
	auto stage = render_stages[render_stage_indices[stage_name]];
	auto & fbo = stage->get_fbo();

	stage->init(create_info.width, create_info.height);

	if (create_info.create_flags & RENDER_STAGE_CREATE_INFO_COLOR_NONE) fbo.select_color_buffer(GL_NONE);
	else if (create_info.create_flags & RENDER_STAGE_CREATE_INFO_COLOR_TEXTURE) stage->attach_texture(*textures.get_texture(create_info.color_name), GL_COLOR_ATTACHMENT0, 0);
	else if (create_info.create_flags & RENDER_STAGE_CREATE_INFO_COLOR_RENDERBUFFER) stage->attach_renderbuffer(*renderbuffers[create_info.color_name], GL_COLOR_ATTACHMENT0);
	
	if (create_info.create_flags & RENDER_STAGE_CREATE_INFO_DEPTH_TEXTURE) stage->attach_texture(*textures.get_texture(create_info.depth_name), GL_DEPTH_ATTACHMENT, 0);
	else if (create_info.create_flags & RENDER_STAGE_CREATE_INFO_DEPTH_RENDERBUFFER) stage->attach_renderbuffer(*renderbuffers[create_info.depth_name], GL_DEPTH_ATTACHMENT);
}

auto pipeline::get_render_stage_id(std::string const & name)->u32
{
	return render_stage_indices[name];
}

auto pipeline::operator[](std::string const & name)->render_stage *
{
	return render_stages[render_stage_indices[name]];
}

auto pipeline::operator[](u32 id)->render_stage *
{
	return render_stages[id];
}