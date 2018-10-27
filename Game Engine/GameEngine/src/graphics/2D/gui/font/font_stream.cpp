#include "font_stream.h"

font_stream::font_stream(font * bound_font, glm::vec2 const & font_size, f32 line_height)
	: bound_font(bound_font), font_size(font_size), line_height(line_height)
{
	diffuse = bound_font->get_texture();
}

auto font_stream::update(font_handler & fonts) -> void
{
	f32 line_y = line_height;
	f32 x = 0.0f;

	std::vector<gui_cache> new_caches;

	new_caches.clear();
	new_caches.reserve(text.size());

	for (u32 i = 0; i < text.size(); ++i)
	{
		if (text[i] == '\n')
		{
			line_y += line_height;
			x = 0.0f;
		}

		character & ch = bound_font->get_data(text[i]);

		gui_cache new_char;

		new_char.size.pos = ch.size * font_size;
		new_char.size.uv = ch.max_texture_coord - ch.texture_coord;

		new_char.position.pos = glm::vec2(x, line_y) + glm::vec2(ch.offset.x, ch.offset.y) * font_size;

		new_char.position.uv = ch.texture_coord;

		new_caches.push_back(new_char);

		x += ch.advance_x * font_size.x;
	}

	vertices = std::move(new_caches);
}

auto font_stream::clear(void) -> void
{
	vertices.clear();
	text = "";
}