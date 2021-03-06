#pragma once

#include <GL/glew.h>
#include "ecs.h"
#include "../detail.h"
#include "../shader_program.h"
#include "../renderable.h"
#include "basic_components.h"
#include "../renderable.h"
#include "../render_func.h"
#include "model_matrix.h"
#include <glm/gtx/transform.hpp>

template <> class component <struct graphics>: public icomponent
{
private:
	i32 model_matrix_component;

	std::string model_name;

	model_handler * mh;
	program * shaders;
public:
	component(entity & subject, i32 index, program & shadrs, std::string const & model, model_handler & mh) 
		: model_name(model), shaders(&shadrs), icomponent::icomponent(index), mh(&mh)
	{
		model_matrix_component = subject.get_component_index<model_matrix>();
	}
	component(void) = default;

	auto operator=(component &) -> component & = default;

	auto update(f32, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		using detail::fequ;
		auto const & ent = entities[entity_index].get_data();

		auto & mmatrix = ecs.get_component<model_matrix>(model_matrix_component);

		glm::mat4 model_matrix = mmatrix.get_translation() * mmatrix.get_rotation() * mmatrix.get_scale();

		//shaders->send_uniform_mat4("model_matrix", &model_matrix[0][0], 1);

//		mh->render(model_name);

		mh->render_model(model_name, model_matrix);
	}
};