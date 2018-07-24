#include "grass.h"
#include <glm/gtx/transform.hpp>
#include <array>

auto grass_platform::create(resource_handler & rh) -> void
{
	struct vertex { glm::vec3 p; glm::vec2 t; };
	std::array<vertex, 4> verts
	{
		vertex{ glm::vec3(-10, 0, -10), glm::vec2(1, 0) },
        vertex{ glm::vec3(-10, 0, 10), glm::vec2(0, 0) },
      	vertex{ glm::vec3(10, 0, -10), glm::vec2(1, 1) },
        vertex{ glm::vec3(10, 0, 10), glm::vec2(0, 1) },
	};

	vertex_buffer.create();
	vertex_buffer.bind(GL_ARRAY_BUFFER);
	vertex_buffer.fill(sizeof(vertex) * verts.size(), verts.data(), GL_STATIC_DRAW, GL_ARRAY_BUFFER);

	buffer_layout.create();
	buffer_layout.bind();
	buffer_layout.enable(0);
	buffer_layout.enable(1);
	buffer_layout.push<float>(0, 3, sizeof(vertex), nullptr);
	buffer_layout.push<float>(1, 2, sizeof(vertex), (void*)sizeof(glm::vec3));
	buffer_layout.unbind();

	grass_shaders.create_shader(GL_VERTEX_SHADER, "grass_vsh.shader");
	grass_shaders.create_shader(GL_FRAGMENT_SHADER, "grass_fsh.shader");
	grass_shaders.link_shaders("vertex_position", "texture_coords");
	grass_shaders.get_uniform_locations("projection_matrix", "view_matrix", "model_matrix", 
		"light_position", "camera_position", "normal_texture", "grass_texture");
	grass_shaders.use();
	grass_shaders.uniform_1i(0, 5);
	grass_shaders.uniform_1i(1, 6);

	auto img = rh.load<image>("grass3.png");

	grass_texture.create();
	grass_texture.bind(GL_TEXTURE_2D);
	grass_texture.fill(GL_TEXTURE_2D, GL_RGBA, img.w, img.h, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	grass_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	grass_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	grass_texture.enable_mipmap(GL_TEXTURE_2D);

	auto norms = rh.load<image>("grass3_norm.png");

	normals.create();
	normals.bind(GL_TEXTURE_2D);
	normals.fill(GL_TEXTURE_2D, GL_RGBA, norms.w, norms.h, GL_RGBA, GL_UNSIGNED_BYTE, norms.data);
	normals.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	normals.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	normals.enable_mipmap(GL_TEXTURE_2D);

	translation = glm::translate(glm::vec3(0.0f, 4.0f, 0.0f));
}

auto grass_platform::prepare(glm::mat4 & proj, glm::mat4 & view, glm::vec3 & camera, float elapsed) -> void
{
	grass_shaders.use();

	normals.bind(GL_TEXTURE_2D, 0);
	grass_texture.bind(GL_TEXTURE_2D, 1);

	grass_shaders.uniform_mat4(&proj[0][0], 0);
	grass_shaders.uniform_mat4(&view[0][0], 1);

	glm::vec3 light = glm::vec3(300, 10, 300);

	translation = glm::rotate(elapsed / 5.0f, glm::vec3(0, 1, 0)) * translation;
	grass_shaders.uniform_mat4(&translation[0][0], 2);

	grass_shaders.uniform_3f(&light[0], 3);
	grass_shaders.uniform_3f(&camera[0], 4);
}

auto grass_platform::vao(void)->vertex_array &
{
	return buffer_layout;
}