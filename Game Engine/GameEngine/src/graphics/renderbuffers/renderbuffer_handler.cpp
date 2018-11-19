#include "renderbuffer_handler.h"

auto renderbuffer_handler::add_renderbuffer(std::string const & name) -> renderbuffer *
{
	renderbuffer * new_renderbuffer = new renderbuffer;

	u32 id = renderbuffers.size();

	renderbuffer_indices[name] = id;

	renderbuffers.push_back(new_renderbuffer);

	return new_renderbuffer;
}

auto renderbuffer_handler::get_renderbuffer_id(std::string const & name) -> u32
{
	return renderbuffer_indices[name];
}

auto renderbuffer_handler::operator[](std::string const & name) -> renderbuffer *
{
	return renderbuffers[renderbuffer_indices[name]];
}

auto renderbuffer_handler::operator[](u32 id) -> renderbuffer *
{
	return renderbuffers[id];
}