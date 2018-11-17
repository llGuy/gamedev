#include "pipeline.h"

auto pipeline::execute_stages(void) -> void
{
	std::for_each(render_stages.begin(), render_stages.end()
		, [](render_stage * stage) -> void { 
		stage->clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		stage->execute(); 
	});
}

auto pipeline::add_render_stage(std::string const & name, render_stage * stage) -> void
{
	render_stage_indices[name] = render_stages.size();
	render_stages.push_back(stage);
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