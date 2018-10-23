#pragma once

#include "../2D/renderer_2D.h"
#include "../3D/renderer.h"
#include <unordered_map>

class renderer_handler
{
private:
	std::vector<renderer_3D *> renderers_3D;
	std::vector<renderer_2D *> renderers_2D;

	std::unordered_map<std::string, u32> index_map_3D;
	std::unordered_map<std::string, u32> index_map_2D;
public:
	auto flush_3D(void) -> void;
	auto flush_2D(void) -> void;

	template <typename T, typename ... Ts> auto create_renderer_3D(std::string const & name, Ts ... init_params) -> std::pair<renderer_3D *, u32>
	{
		renderer_3D * renderer = new T(init_params...);
		u32 renderer_id = renderers_3D.size();
		index_map_3D[name] = renderer_id;
		renderers_3D.push_back(renderer);

		return std::pair(renderer, renderer_id);
	}

	template <typename T, typename ... Ts> auto create_renderer_2D(std::string const & name, Ts ... init_params) -> std::pair<renderer_2D *, u32>
	{
		renderer_2D * renderer = new T(init_params...);
		u32 renderer_id = renderers_2D.size();
		index_map_2D[name] = renderer_id;
		renderers_2D.push_back(renderer);

		return std::pair(renderer, renderer_id);
	}

	auto get_renderer_3D_index(std::string const & name) -> u32;
	auto get_renderer_2D_index(std::string const & name) -> u32;

	auto get_renderer_3D(u32 index) -> renderer_3D *;
	auto get_renderer_2D(u32 index) -> renderer_2D *;
};