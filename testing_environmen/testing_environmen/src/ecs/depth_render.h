#pragma once

#include <GL/glew.h>
#include "ecs.h"
#include "../detail.h"
#include "../program.h"
#include "../renderable.h"
#include "basic_components.h"
#include "../renderable.h"
#include "../render_func.h"
#include "model_matrix.h"
#include <glm/gtx/transform.hpp>

template <> class component <struct depth> : public icomponent
{
private:
	i32 model_matrix_component;
	renderable * rnd;
	program * shaders;
public:
	component(entity & subject, i32 index, program & shadrs, renderable & rnd)
		: rnd(&rnd), shaders(&shadrs), icomponent::icomponent(index)
	{
		model_matrix_component = subject.get_component_index<model_matrix>();
	}
	component(void) = default;

	auto operator=(component &)->component & = default;

	auto update(f32, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto const & ent = entities[entity_index].get_data();

		auto & mmatrix = ecs.get_component<model_matrix>(model_matrix_component);
		glm::mat4 model_matrix = mmatrix.get_translation() * mmatrix.get_rotation() * mmatrix.get_scale();

		shaders->send_uniform_mat4(1, &model_matrix[0][0], 1);
		render_model(*rnd, GL_TRIANGLES);
	}
};