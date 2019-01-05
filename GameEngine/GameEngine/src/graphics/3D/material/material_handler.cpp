#include "material_handler.h"
#include "material_json_functors.h"

auto material_handler::add_material(std::string mat_name, material_light_info const & light_info
	, glsl_program * shader, light_handler & lights) -> material_prototype *
{
	u32 material_type_index = materials.size();
	materials.push_back(new material_prototype(light_info, shader, lights, mat_name));
	material_indices[mat_name] = material_type_index;
	materials.back()->init();
	return materials.back();
}

auto material_handler::init_material(std::string const & name) -> material_prototype *
{
	u32 material_type_index = materials.size();
	materials.push_back(new material_prototype(name));
	material_indices[name] = material_type_index;
	materials.back()->init();
	return materials.back();
}

auto material_handler::init(texture_handler & textures, shader_handler & shaders, light_handler & lights) -> void
{
	material_loader_functor::data_type data{ nullptr, this, &shaders, &textures, &lights };

	loader.init_data(data);

	using loader_type = json_loader<material_loader_functor>;

	loader.init_functors(
		loader_type::functor_type<shader_material_functor>{ "shader" },
		loader_type::functor_type<flush_material_functor>{ "flush" },
		loader_type::functor_type<textures2D_material_functor>{ "textures_2D" },
		loader_type::functor_type<textures3D_material_functor>{ "textures_3D" },
		loader_type::functor_type<lighting_material_functor>{ "lighting" },
		loader_type::functor_type<toggle_lighting_functor>{ "lit" }
	);
}

auto material_handler::load_materials(void) -> void
{
	loader.load(extract_file("res/saves/materials.json")
		, [](material_loader_functor::data_type & data) -> void {
		
		std::string material_name = "material." + data.iterator->key();

		auto prototype = data.dest->init_material(material_name);

		prototype->get_light_handler_ptr() = data.lights;

		data.mat = prototype;

	});
}

auto material_handler::render_all(camera & scene_camera, timer & timeh) -> void
{
	for (auto renderer : materials)
	{
		renderer->render(scene_camera, timeh);
	}
}

auto material_handler::flush(void) -> void
{
	for (auto renderer : materials)
	{
		if (renderer->get_flush_each_frame()) 
			renderer->flush();
	}
}

auto material_handler::submit(material * mat) -> void
{
	materials[mat->material_type_id]->submit_material(mat);
}

auto material_handler::get_material_id(std::string const & name)->u32
{
	return material_indices[name];
}

auto material_handler::operator[](u32 id)->material_prototype *
{
	return materials[id];
}

auto material_handler::operator[](std::string const & name)->material_prototype *
{
	return materials[material_indices[name]];
}

auto material_handler::get_size(void) const -> u32
{
	return materials.size();
}