#pragma once

#include "../../api/api.h"

class mesh_handler;

struct render_func
{
private:
public:
	virtual auto draw(mesh_handler & mh) -> void
	{
	}
};

struct render_indices : render_func
{
private:
	std::string name;
public:
	render_indices(std::string const & name);
	auto draw(mesh_handler & mh) -> void override;
};

struct render_arrays : render_func
{
private:
	std::string name;
public:
	render_arrays(std::string const & name);
	auto draw(mesh_handler & mh) -> void override;
};