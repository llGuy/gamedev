#include "font_stream.h"
#include "font_handler.h"

font_stream::font_stream(u32 font_id, f32 font_size, f32 line_height)
	: font_id(font_id), font_size(font_size), line_height(line_height)
{
}

auto font_stream::submit_text(std::string const & text) -> void
{
	this->text += text;
}

auto font_stream::update(font_handler & fonts) -> void
{
	f32 line_y = line_height;
	f32 x = 0.0f;

	std::vector<gui_vertices_cache> new_caches;

	new_caches.clear();
	new_caches.reserve(text.size());

	font & font_used = fonts[font_id];

	for (u32 i = 0; i < text.size(); ++i)
	{
		character & ch = font_used.get_data(text[i]);

		gui_vertices_cache new_char;

		new_char.size.position = ch.size * font_size;
		new_char.size.texture_coords = ch.max_texture_coord - ch.texture_coord;

		new_char.coord.position = glm::vec2(x, line_y) + glm::vec2(ch.offset.x, -ch.offset.y) * font_size;
		new_char.coord.texture_coords = ch.texture_coord;
		 
		new_caches.push_back(new_char);

		x += ch.advance_x * font_size;
	}

	vertices = std::move(new_caches);
}
/*
auto font_stream::submit_to_renderer(i32 display_w, i32 display_h) -> void
{

}*/