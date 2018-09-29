#pragma once

#include <glm/glm.hpp>
#include <stack>
#include "shader_program.h"
#include <glm/gtc/type_ptr.hpp>
#include "render_func.h"
#include "cube.h"
#include <algorithm>
#include "detail.h"
#include <vector>
#include "types.h"
#include <glm/gtx/transform.hpp>

struct puff
{
	f32 scale;
	glm::vec3 position;
};

class puff_handler
{
private:
	static constexpr f32 default_scale = 0.1f;

	std::vector<puff> puffs;
public:
	auto add_puff(glm::vec3 const & pos) -> void
	{
		puffs.push_back(puff{ default_scale, pos });
	}
	auto render(program & shaders, u32 uni_id, u32 color_id, cube & cbe) -> void
	{
		for (auto & pf : puffs)
		{
			f32 factor = (pf.scale - 0.1f) * 0.6f;
			glm::vec3 color = glm::vec3(4.0f, 0.0f, 0.0f);

			color = glm::mix(glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(4.0f, 4.0f, 0.0f), factor);
			glm::mat4 model_matrix = glm::translate(pf.position) * glm::scale(glm::vec3(pf.scale));
			shaders.send_uniform_mat4(uni_id, glm::value_ptr(model_matrix), 1);
			shaders.send_uniform_vec3(color_id, glm::value_ptr(color), 1);
			render_model(cbe, GL_TRIANGLES);
		}
	}
	auto update(f32 td) -> void
	{
		puffs.erase(
			std::remove_if(puffs.begin(), puffs.end(),
				[&](puff & pf) -> bool { pf.scale += td / 1.3f; return pf.scale > 0.6f; }),
			puffs.end());
	}
};