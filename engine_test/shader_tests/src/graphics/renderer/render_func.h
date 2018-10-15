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
	u32 model_id;
public:
	render_indices(u32 model_id);
	auto draw(mesh_handler & mh) -> void override;
};

struct render_arrays : render_func
{
private:
	u32 model_id;
public:
	render_arrays(u32 model_id);
	auto draw(mesh_handler & mh) -> void override;
};