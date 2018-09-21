#include "render_pipeline.h"
#include "../detail.h"

auto render_model(renderable & model, GLenum type) -> void
{
	auto & vao = model.vao();
	vao.bind();
	if (model.indexed())
	{
		model.element_buffer().value()->bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(type, model.count(), model.index_type(), model.index_ptr());
		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	}
	else
	{
		glDrawArrays(type, 0, model.count());
	}
	unbind_vertex_layouts();
}

auto clear(i32 flags, f32 r, f32 g, f32 b) -> void
{
	glClear(flags);
	glClearColor(r, g, b, 1);
}

render_pipeline::render_pipeline(void)
	: blur_stages{ blur_stage{4, 6}, blur_stage{8, 10}, blur_stage{12, 16} }
{
}

auto render_pipeline::create(i32 w, i32 h, resource_handler & rh) -> void
{
	default_target.create(w, h);

	for (auto & stage : blur_stages)
	{
		stage.h.create(w, h);
		stage.v.create(w, h);
	}

	render_quad.create(rh);
	shaders.create_shader(GL_VERTEX_SHADER, "shaders/gui_quad/vsh.shader");
	shaders.create_shader(GL_FRAGMENT_SHADER, "shaders/gui_quad/fsh.shader");
	shaders.link_shaders("vertex_position", "texture_coords");
	shaders.get_uniform_locations("model_matrix");
}

auto render_pipeline::bind_default(void) -> void
{
	default_target.bind();
}

auto render_pipeline::bind_glow(void) -> void
{
}

auto render_pipeline::finalize_process(void) -> void
{
	using detail::identity_matrix;

	carry_blur();

	unbind_all_framebuffers(default_target.width(), default_target.height());
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0, 0, 0);

	shaders.use();
	shaders.uniform_mat4(&identity_matrix[0][0], 0);

	render_model(render_quad, GL_TRIANGLE_STRIP);
}

auto render_pipeline::carry_blur(void) -> void
{
	blur_stages[0].v.render(render_quad, blur_stages[0].h.output(), 0, 0);

	for (u32 i = 1; i < blur_stages.size(); ++i)
	{
		blur_stages[i].h.render(render_quad, blur_stages[i - 1].v.output(), 0, 0);
		blur_stages[i].v.render(render_quad, blur_stages[i].h.output(), 0 ,0);
	}
}