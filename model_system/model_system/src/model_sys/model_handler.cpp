#include "../inc/texture.h"
#include "../inc/vao.h"
#include "../inc/buffer.h"
#include "model_handler.h"

model_handler::model_handler(void)
	: models(30)
{
}

auto model_handler::init(void) -> void
{
	data_system.add_system<comp_vertex_buffer>(100);
	data_system.add_system<comp_texture_buffer>(100);
	data_system.add_system<vertex_layout>(100);
}

auto model_handler::create_model(void) -> model_instance
{
	u32 index = models.vec_size();
	models.add(model_prototype());
	return index;
}