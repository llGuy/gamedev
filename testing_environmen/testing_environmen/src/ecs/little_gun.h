#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../input_handler.h"
#include "../puff_handler.h"
#include "../tracer_handler.h"

struct aabb { glm::vec3 min, max; };

template <> class component <struct little_gun> : public icomponent
{
private:
	tracer_handler * th;
	input_handler * ih;
	puff_handler * puffs;
	bool clicked{ false };
public:
	component(entity & subject, i32 index, input_handler & ih, tracer_handler & th, puff_handler & ph)
		: icomponent::icomponent(index), ih(&ih), th(&th), puffs(&ph)
	{
	}
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		auto & data = entities[entity_index].get_data();
		glm::vec3 position = data.pos;
		position.y += 2.0f;

		glm::vec3 v1 = position + data.dir * 100.0f;

		glm::vec3 intersection;
		bool hit = false;

		if (ih->got_mouse_button(GLFW_MOUSE_BUTTON_1))
		{
			for (i32 i = 0; i < entities.amount(); ++i)
			{
				if (i != entity_index)
				{
					auto & data = entities[i].get_data();
					/* get rotation matrix of the cube */
					u32 model_matrix_comp_id = entities[i].get_component_index<model_matrix>();
					auto & mm_comp = ecs.get_component<model_matrix>(model_matrix_comp_id);

					auto trans_m = glm::translate(-glm::vec3(data.pos.x, data.pos.y + 2.0f, data.pos.z));
					auto rota_m = glm::inverse(ecs.get_component<model_matrix>(model_matrix_comp_id).get_rotation());

					glm::mat4 model = mm_comp.get_model_matrix();
					glm::mat4 inverse_model = rota_m * trans_m;

					aabb box{ glm::vec3(-2.0f), glm::vec3(2.0f) };

					f32 frac;

					glm::vec3 translated_pos = glm::vec3(inverse_model * glm::vec4(position, 1.0f));
					glm::vec3 translated_v1 = glm::vec3(inverse_model * glm::vec4(v1, 1.0f));

					hit = line_aabb_intersection(box, translated_pos, translated_v1, intersection, frac);

					if (hit)
					{
						auto rotation = mm_comp.get_rotation();
						auto translation = mm_comp.get_translation();
						//intersection = glm::vec3(translation * rotation * glm::vec4(intersection, 1.0f));
						intersection =  glm::inverse(inverse_model) * glm::vec4(intersection, 1.0f);
						break;
					}
				}
			}
			if (hit)
			{
				glm::vec3 direction_test = glm::normalize(intersection - position);

				th->push(glm::vec3(1.f, 1.f, 0.0f),
					position + detail::up * 1.5f + glm::cross(data.dir, detail::up) / 5.0f, intersection);

				if (!clicked) puffs->add_puff(intersection);
			}

			clicked = true;
		}
		else clicked = false;
	}
private:
	auto clip_line(i32 d, aabb const & box, glm::vec3 const & v0, glm::vec3 const & v1, f32 & f_low, f32 & f_high) -> bool
	{
		f32 f_dim_low, f_dim_high;

		f_dim_low = (box.min[d] - v0[d]) / (v1[d] - v0[d]);
		f_dim_high = (box.max[d] - v0[d]) / (v1[d] - v0[d]);

		if (f_dim_high < f_dim_low) std::swap(f_dim_high, f_dim_low);

		if (f_dim_high < f_low) return false;
		if (f_dim_low > f_high) return false;

		f_low = std::max(f_dim_low, f_low);
		f_high = std::min(f_dim_high, f_high);

		if (f_low > f_high) return false;

		return true;
	}
	auto line_aabb_intersection(aabb const & box, glm::vec3 const & v0,
		glm::vec3 const & v1, glm::vec3 & intersection, f32 & fraction) -> bool
	{
		f32 f_low = 0;
		f32 f_high = 1;

		if (!clip_line(0, box, v0, v1, f_low, f_high)) return false;
		if (!clip_line(1, box, v0, v1, f_low, f_high)) return false;
		if (!clip_line(2, box, v0, v1, f_low, f_high)) return false;

		glm::vec3 b = v1 - v0;
		intersection = v0 + b * f_low;

		fraction = f_low;

		return true;
	}
};
