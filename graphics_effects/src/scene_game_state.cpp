#include "scene_game_state.h"
#include "timer.h"

#include "detail.h"

#include <glm/gtx/string_cast.hpp>

scene_state::scene_state(int32_t w, int32_t h, glm::vec2 const & cursor_pos, resource_handler & rh)
	: resolution { w, h }, projection_matrix(glm::perspective(glm::radians(60.0f), 
															  (float)w / (float)h, 0.001f, 1000.0f)), main_camera(cursor_pos), test_cube(2.0f, glm::vec3(0.0f, 0.8f, 0.0f))
{
	using detail::vec_rand;

	test_cube.create(rh);
	// create test cubes
	std::for_each(cube_positions.begin(), cube_positions.end(),
				  [&] (glm::vec3 & p) { p = vec_rand<float, 3>(256); p.y = 10.0f; } );

	scene_terrain.create(rh);

	terrain_program.create_shader(GL_VERTEX_SHADER, "scene_vsh.shader");
	terrain_program.create_shader(GL_GEOMETRY_SHADER, "scene_gsh.shader");
	terrain_program.create_shader(GL_FRAGMENT_SHADER, "scene_fsh.shader");
	terrain_program.link_shaders("vertex_position", "texture_coords");
	terrain_program.get_uniform_locations("model_color", "projection_matrix", "view_matrix", 
										  "background", "r_texture", "g_texture", "b_texture", "blend_map");

	terrain_program.use();
	//scene_terrain.bind_textures();
	connect_texture_units();

	cube_program.create_shader(GL_VERTEX_SHADER, "cube_vsh.shader");
	cube_program.create_shader(GL_GEOMETRY_SHADER, "cube_gsh.shader");
	cube_program.create_shader(GL_FRAGMENT_SHADER, "cube_fsh.shader");
	cube_program.link_shaders("vertex_position", "vertex_color");
	cube_program.get_uniform_locations("projection_matrix", "view_matrix", "model_matrix");

	glEnable(GL_DEPTH_TEST);
}
auto scene_state::render(void) -> void 
{
	// onto default framebuffer
	render_scene();
}
auto scene_state::update(input_handler & ih, timer & time) -> game_state * 
{
	main_camera.handle_input(ih, time.elapsed());

	if (ih.cursor_moved())
		main_camera.look_at(ih.cursor_position());

	main_camera.update_view_matrix();

	// no new game state possible
	return nullptr;
}

auto scene_state::connect_texture_units(void) -> void
{
	for(uint32_t i = 0; i < 5; ++i)
		terrain_program.uniform_1i(i, i + 3);
}

auto scene_state::render_scene(void) -> void
{
	glm::vec3 color { 1.0f, 0.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 1.0f, 1.0f);

	terrain_program.use();
	scene_terrain.bind_textures();
	terrain_program.uniform_3f(&color[0], 0);
	terrain_program.uniform_mat4(&projection_matrix[0][0], 1);
	terrain_program.uniform_mat4(&main_camera.view_matrix()[0][0], 2);

	render_model(scene_terrain.vao(), scene_terrain.element_buffer(), terrain<256, 256>::vertex_count());

	cube_program.use();
	cube_program.uniform_mat4(&projection_matrix[0][0], 0);
	cube_program.uniform_mat4(&main_camera.view_matrix()[0][0], 1);

	for(uint32_t i = 0; i < 10; ++i)
		{
			auto trans = glm::translate(cube_positions[i]);
			cube_program.uniform_mat4(&trans[0][0], 2);
			render_model(test_cube.vao(), test_cube.element_buffer(), 36);
		}
}
