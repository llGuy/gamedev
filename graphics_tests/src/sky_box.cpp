#include <array>
#include <string>
#include "sky_box.h"

auto sky_box::create(resource_handler & rh) -> void
{
	sky_box_texture.create();
	sky_box_texture.bind(GL_TEXTURE_CUBE_MAP);
	std::array<std::string, 6> files
	{
		"right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png"
	};

	for (uint32_t i = 0; i < files.size(); ++i)
	{
		auto img = rh.load<image>(files[i]);
		sky_box_texture.fill(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGBA, img.w, img.h, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	sky_box_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	sky_box_texture.int_param(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	sky_box_shaders.create_shader(GL_VERTEX_SHADER, "sky_box_vsh.shader");
	sky_box_shaders.create_shader(GL_FRAGMENT_SHADER, "sky_box_fsh.shader");
	sky_box_shaders.link_shaders("vertex_position");
	sky_box_shaders.get_uniform_locations("projection_matrix", "view_matrix");

	create_cube();
}

auto sky_box:: prepare(glm::mat4 & projection) -> void
{
	sky_box_shaders.use();
	sky_box_shaders.uniform_mat4(&projection[0][0], 0);
//	sky_box_shaders.uniform_mat4(&view[0][0], 1);

	sky_box_texture.bind(GL_TEXTURE_CUBE_MAP, 0);
}

auto sky_box::create_cube(void) -> void
{
	std::array<float, 108> vertices
	{
		-cube_size,  cube_size, -cube_size,
		-cube_size, -cube_size, -cube_size,
		cube_size, -cube_size, -cube_size,
		cube_size, -cube_size, -cube_size,
		cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size, -cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		cube_size, -cube_size, -cube_size,
		cube_size, -cube_size,  cube_size,
		cube_size,  cube_size,  cube_size,
		cube_size,  cube_size,  cube_size,
		cube_size,  cube_size, -cube_size,
		cube_size, -cube_size, -cube_size,

		-cube_size, -cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		cube_size,  cube_size,  cube_size,
		cube_size,  cube_size,  cube_size,
		cube_size, -cube_size,  cube_size,
		-cube_size, -cube_size,  cube_size,

		-cube_size,  cube_size, -cube_size,
		cube_size,  cube_size, -cube_size,
		cube_size,  cube_size,  cube_size,
		cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size,  cube_size,
		-cube_size,  cube_size, -cube_size,

		-cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		cube_size, -cube_size, -cube_size,
		cube_size, -cube_size, -cube_size,
		-cube_size, -cube_size,  cube_size,
		cube_size, -cube_size,  cube_size
	};

	cube_vertices.create();
	cube_vertices.bind(GL_ARRAY_BUFFER);
	cube_vertices.fill(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	buffer_layout.create();
	buffer_layout.bind();
	buffer_layout.enable(0);
	buffer_layout.push<float>(0, 3, sizeof(float) * 3, nullptr);
	buffer_layout.unbind();
}

auto sky_box::vao(void) -> vertex_array &
{
	return buffer_layout;
}

auto sky_box::count(void) -> uint32_t
{
	return 36;
}