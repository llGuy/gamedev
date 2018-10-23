#include "font_stream.h"
#include "font_handler.h"
#include "../../graphics/renderer/2D/renderer_2D.h"

auto font_handler::create_font(texture_mapper & textures, std::string const & name, std::string const & dir) -> u32
{
	u32 id = fonts.size();
	font new_font(name);
	new_font.load_font(textures, dir);

	fonts.push_back(new_font);
	index_map[name] = id;

	return id;
}

auto font_handler::create_font_stream(std::string const & stream_name, std::string const & name, glm::vec2 const & size, f32 line_height, renderer_2D * renderer) -> u32
{
	u32 font_stream_id = streams.size();

	font_stream_index_map[stream_name] = font_stream_id;

	streams.push_back(new font_stream(get_font_index(name), size, line_height));

	streams.back()->bind_renderer(renderer);

	return font_stream_id;
}

auto font_handler::get_font_index(std::string const & name)->u32
{
	return index_map[name];
}

auto font_handler::get_font(u32 index) -> font &
{
	return fonts[index];
}

auto font_handler::get_font_stream(u32 index) -> font_stream *
{
	return streams[index];
}

auto font_handler::get_font_stream_index(std::string const & name)->u32
{
	return font_stream_index_map[name];
}