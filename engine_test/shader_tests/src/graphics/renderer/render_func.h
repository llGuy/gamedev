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

	virtual ~render_func(void) {}
};

struct render_indices : render_func
{
private:
	u32 model_id;
public:
	render_indices(u32 model_id);
	auto draw(mesh_handler & mh) -> void override;

	~render_indices(void) = default;
};

struct render_arrays : render_func
{
private:
	u32 model_id;
public:
	render_arrays(u32 model_id);
	auto draw(mesh_handler & mh) -> void override;

	~render_arrays(void) = default;
};