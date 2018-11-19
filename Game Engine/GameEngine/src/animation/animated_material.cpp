#include "animated_material.h"
#include <glm/gtc/type_ptr.hpp>

skeletal_material::skeletal_material(model const & renderable, glm::mat4 const & model_matrix, u32 material_type_id)
	: material::material(renderable, model_matrix, material_type_id), uniform_block(ANIMATION_BLOCK_INDEX)
{
}

auto skeletal_material::render(glsl_program * shader) -> void
{
	shader->bind();

	shader->send_uniform_mat4("model_matrix", glm::value_ptr(transform_matrix), 1);

	for (u32 i = 0; i < joint_transforms.size(); ++i)
	{
		shader->send_uniform_mat4("joint_transforms[" + std::to_string(i) + "]", glm::value_ptr(joint_transforms[i]), 1);
	}

	//shader->bind_uniform_block(uniform_block, "transform_matrices");

	renderable->vao.bind();
	auto & indices = renderable.get_component<index_buffer_component>();
	indices->value.bind(GL_ELEMENT_ARRAY_BUFFER);
	
	glDrawElements(GL_TRIANGLES, renderable->count, GL_UNSIGNED_INT, nullptr);

	unbind_buffers(GL_ELEMENT_ARRAY_BUFFER);
	unbind_vertex_layouts();
}

auto skeletal_material::get_transforms_array(void)->std::vector<glm::mat4> &
{
	return joint_transforms;
}

auto skeletal_material::get_uniform_block(void) -> uniform_buffer &
{
	return uniform_block;
}