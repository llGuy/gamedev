#include "gui_handler.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../../utils/detail.h"

auto gui_handler::init(shader_handler & shaders) -> void
{
	shader_handle gui_shader_handle("shader.gui");
	auto program = shaders.create_program(gui_shader_handle, "2D");
	program->bind();
	program->send_uniform_mat4("projection_matrix"_hash, glm::value_ptr(detail::identity_matrix), 1);

	renderer.init(program);
}

auto gui_handler::init_font_type(texture_handler & textures, std::string const & font_name, std::string const & dir) -> void
{
	fonts.create_font(textures, font_name, dir);
}

auto gui_handler::init_font_stream(std::string const & parent_name, std::string const & stream_name, std::string const & font_name, glm::vec2 const & size, f32 line_height) -> font_stream *
{
	font_stream * stream = fonts.create_font_stream(stream_name, font_name, size, line_height, &renderer);

	/* get parent gui */
	gui * parent = gui_sets[index_map[parent_name]];
	parent->add_child(stream);

	return stream;
}

auto gui_handler::update(u32 display_w, u32 display_h) -> void
{
	for (auto gui_set : gui_sets)
	{
		gui_set->update();
		gui_set->submit_to_renderer(display_w, display_h);
	}
}

auto gui_handler::render(void) -> void
{
	renderer.render();
}

auto gui_handler::render_one(glsl_program * shader) -> void
{
	renderer.render_first(shader);
}