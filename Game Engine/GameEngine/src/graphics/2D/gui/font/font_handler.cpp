#include "font_handler.h"

auto font_handler::create_font_stream(std::string const & stream_name, std::string const & name, glm::vec2 const & size, f32 line_height, batch_renderer2D * renderer) -> font_stream *
{
	u32 font_stream_id = streams.size();

	font_stream_index_map[stream_name] = font_stream_id;

	streams.push_back(new font_stream);

	streams.back()->init(get_font(name), size, line_height);

	streams.back()->bind_renderer(*renderer);

	return streams.back();
}

auto font_handler::create_font(texture_handler & textures, std::string const & name, std::string const & dir) -> font *
{
	u32 id = fonts.size();
	font * new_font = new font(name);
	new_font->load_font(textures, dir);

	fonts.push_back(new_font);
	index_map[name] = id;

	return new_font;
}

auto font_handler::get_font(std::string const & name) -> font *
{
	return fonts[index_map[name]];
}

auto font_handler::get_font_stream(std::string const & name) -> font_stream *
{
	return streams[font_stream_index_map[name]];
}