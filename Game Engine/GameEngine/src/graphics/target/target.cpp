#include "target.h"

auto target::bind(void) -> void
{
	output.bind(GL_FRAMEBUFFER);
}

auto target::clear_color(f32 r, f32 g, f32 b, f32 a) -> void
{
	glClearColor(r, g, b, a);
}

auto target::clear(u32 flags) -> void
{
	glClear(flags);
}

auto target::add_color_output(texture * color) -> void
{
	
}

auto target::add_depth_output(texture * depth) -> void
{

}