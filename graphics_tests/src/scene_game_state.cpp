#include "scene_game_state.h"
#include "timer.h"

#include "detail.h"

#include <glm/gtx/string_cast.hpp>

scene_state::scene_state(int32_t w, int32_t h, glm::vec2 const & cursor_pos, resource_handler & rh)
	: resolution { w, h }, projection_matrix(glm::perspective(glm::radians(60.0f), 
											(float)w / (float)h, 0.001f, 1000.0f)), main_camera(cursor_pos), 
											test_cube(2.0f, glm::vec3(0.0f, 0.8f, 0.0f))
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
	terrain_program.get_uniform_locations("model_color", "projection_matrix", "view_matrix", "depth_bias",
										  "background", "r_texture", "g_texture", "b_texture", "blend_map", "shadow_map");

	terrain_program.use();
	//scene_terrain.bind_textures();
	connect_texture_units();

	cube_program.create_shader(GL_VERTEX_SHADER, "cube_vsh.shader");
	cube_program.create_shader(GL_GEOMETRY_SHADER, "cube_gsh.shader");
	cube_program.create_shader(GL_FRAGMENT_SHADER, "cube_fsh.shader");
	cube_program.link_shaders("vertex_position", "vertex_color");
	cube_program.get_uniform_locations("projection_matrix", "view_matrix", "model_matrix");

	shadow_handler.create();

	guis.create();
	guis.push(glm::vec2(-0.5f, -0.5f), 0.6f);

	glEnable(GL_DEPTH_TEST);
}
auto scene_state::render(void) -> void 
{
	glViewport(0, 0, resolution.x, resolution.y);
	// onto default framebuffer
	auto & shadow_fbo = shadow_handler.fbo();
	shadow_fbo.unbind();
	render_scene();

	shadow_fbo.bind();
	glViewport(0, 0, 1024, 1024);
	render_depth();

	// render contents in depth texture
	shadow_fbo.unbind();
	glViewport(0, 0, resolution.x, resolution.y);
	render_depth_gui();
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
	uint32_t i = 0;
	for(; i < 5; ++i)
		terrain_program.uniform_1i(i, i + 4);
	terrain_program.uniform_1i(i, i + 4);
}

auto scene_state::render_scene(void) -> void
{
	glCullFace(GL_BACK);

	glm::vec3 color { 1.0f, 0.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 1.0f, 1.0f);

	terrain_program.use();
	scene_terrain.bind_textures();
	shadow_handler.tex().bind(5);
	terrain_program.uniform_3f(&color[0], 0);
	terrain_program.uniform_mat4(&projection_matrix[0][0], 1);
	terrain_program.uniform_mat4(&main_camera.view_matrix()[0][0], 2);
	auto & bias = shadow_handler.bias();
	terrain_program.uniform_mat4(&bias[0][0], 3);

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

auto scene_state::render_depth_gui(void) -> void
{
	auto & quad2D = guis.quad();
	auto & shaders = guis.shaders();
	shaders.use();
	auto & texture = shadow_handler.tex();
	texture.bind(0);
	guis.prepare_render();
	render_model_arrays(quad2D.vao(), 4, GL_TRIANGLE_STRIP);
}

auto scene_state::render_depth(void) -> void
{
	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	auto & fbo = shadow_handler.fbo();
	auto & shaders = shadow_handler.shaders();
	auto & depth_texture = shadow_handler.tex();
	auto & projection = shadow_handler.depth_projection();

	fbo.bind();
	shaders.use();

	depth_texture.bind(0);

	glm::mat4 matrix = glm::perspective(glm::radians(60.0f), (float)resolution.x / resolution.y, 0.001f, 1000.0f) * main_camera.view_matrix();
	//shaders.uniform_mat4(&matrix[0][0], 0);

	glm::mat4 identity { 1.0f };
	shaders.uniform_mat4(&identity[0][0], 1);
	render_model(scene_terrain.vao(), scene_terrain.element_buffer(), terrain<256, 256>::vertex_count());

	for (uint32_t i = 0; i < 10; ++i)
	{
		auto trans = glm::translate(cube_positions[i]);
		shaders.uniform_mat4(&trans[0][0], 1);
		render_model(test_cube.vao(), test_cube.element_buffer(), 36);
	}

	render_player(shaders, 1);
}

auto scene_state::render_player(program & shaders, uint32_t udata_index) -> void
{
	auto trans = glm::translate(main_camera.position());
	shaders.uniform_mat4(&trans[0][0], udata_index);
	render_model(test_cube.vao(), test_cube.element_buffer(), 36);
}