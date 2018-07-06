#include "scene_game_state.h"
#include "timer.h"

scene_state::scene_state(int32_t w, int32_t h, glm::vec2 const & cursor_pos, resource_handler & rh)
	: resolution { w, h }, projection_matrix(glm::perspective(glm::radians(60.0f), 
	                       (float)w / (float)h, 0.001f, 1000.0f)), main_camera(cursor_pos)
{
	scene_terrain.create(rh);

	scene_program.create_shader(GL_VERTEX_SHADER, "src/scene_vsh.shader");
	scene_program.create_shader(GL_GEOMETRY_SHADER, "src/scene_gsh.shader");
	scene_program.create_shader(GL_FRAGMENT_SHADER, "src/scene_fsh.shader");
	scene_program.link_shaders("vertex_position", "texture_coords");
	scene_program.get_uniform_locations("model_color", "projection_matrix", "view_matrix", 
		"background", "r_texture", "g_texture", "b_texture", "blend_map");

	scene_program.use();
	//scene_terrain.bind_textures();
	connect_texture_units();
}
auto scene_state::render(void) -> void 
{
	// onto default framebuffer
	scene_program.use();
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
		scene_program.uniform_1i(i, i + 3);
}

auto scene_state::render_scene(void) -> void
{
	glm::vec3 color { 1.0f, 0.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 1.0f, 1.0f);

	scene_terrain.bind_textures();
	scene_program.uniform_3f(&color[0], 0);
	scene_program.uniform_mat4(&projection_matrix[0][0], 1);
	scene_program.uniform_mat4(&main_camera.view_matrix()[0][0], 2);

	render_model(scene_terrain.vao(), scene_terrain.element_buffer(), terrain<256, 256>::vertex_count());
}