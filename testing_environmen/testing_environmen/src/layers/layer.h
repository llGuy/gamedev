#pragma once

#include <string>
#include <unordered_map>
#include "../batch_renderer.h"
#include "../shader_program.h"

struct material
{
	glm::mat4 projection_matrix;
};

/* group models which have the same shader 
(they will have different renderers though) */
class layer
{
private:
	program shaders;
	std::vector<batch_renderer> renderers;
	std::unordered_map<std::string, u32> map;
public:
	auto add(std::string const & model) -> void
	{
		
	}

	auto submit(std::string const & model, glm::mat4 const & model_matrix) -> void
	{

	}
	
	auto render(model_handler & mh) -> void
	{
		for (auto it = map.begin(); it != map.end(); ++it)
		{

		}
	}

	template <typename ... T> auto flush(T ... models) -> void
	{
		/* flush everything */
		if (sizeof...(T) == 0)
		{
			std::for_each(renderers.begin(), renderers.end(), [](batch_renderer & renderer)
			{
				renderer.flush();
			});
		}
		else
		{
			(renderers[map.at(models)], ...);
		}
	}
};