#pragma once

#include "../../api/api.h"
#include "../mesh/mesh_handler.h"
/*
struct render_func
{
private:
public:
	virtual auto draw(mesh_handler & mh) -> void = 0;
};

struct render_indices : render_func
{
private:
	std::string name;
public:
	render_indices(std::string const & name) 
		: name(name)
	{
	}

	auto draw(mesh_handler & mh) -> void override
	{
		mesh_data & data = mh.get_data(name);
		buffer & ibo = mh.get_buffer<index_buffer_component>(name);

		data.vao.bind();
		ibo.bind(GL_ELEMENT_ARRAY_BUFFER);

		glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, nullptr);

		unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
		unbind_vertex_layouts();
	}
};

struct render_arrays : render_func
{
private:
	std::string name;
public:
	render_arrays(std::string const & name)
		: name(name)
	{
	}

	auto draw(mesh_handler & mh)-> void override
	{
		mesh_data & data = mh.get_data(name);

		data.vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, data.count);

		unbind_vertex_layouts();
	}
};*/