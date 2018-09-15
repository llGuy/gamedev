#pragma once

#include <GL/glew.h>
#include "ecs.h"
#include "../detail.h"
#include "../program.h"
#include "../renderable.h"
#include "basic_components.h"
#include "../renderable.h"
#include "../render_func.h"
#include <glm/gtx/transform.hpp>

template <> class component <struct graphics>: public icomponent
{
private:
	renderable * rnd;
	program * shaders;
public:
	component(entity & subject, i32 index, program & shadrs, renderable & rnd) 
		: rnd(&rnd), shaders(&shadrs), icomponent::icomponent(index)
	{
	}
	component(void) = default;

	auto operator=(component &) -> component & = default;

	auto update(f32, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto const & ent = entities[entity_index].get_data();

		/* calculate angle that player is facing */
		float y_axis_rot = fequ(ent.dir.x, 0.0f) ? 0.00001f : -atan(ent.dir.z / ent.dir.x);
		float z_axis_rot = fequ(ent.dir.y, 0.0f) ? 0.00001f : -atan(ent.dir.x / ent.dir.y);
		float x_axis_rot = fequ(ent.dir.y, 0.0f) ? 0.00001f : -atan(ent.dir.z / ent.dir.y);

		glm::vec3 translate_vec = ent.pos;

		glm::mat4 rotation = glm::rotate(x_axis_rot, glm::vec3(1, 0, 0)) *
			glm::rotate(y_axis_rot, glm::vec3(0, 1, 0)) *
			glm::rotate(z_axis_rot, glm::vec3(0, 0, 1));
		glm::mat4 model_matrix = glm::translate(translate_vec) * rotation * glm::scale(glm::vec3(ent.size));

		shaders->uniform_mat4(&model_matrix[0][0], 3);
		render_model(*rnd, GL_TRIANGLES);
	}
};