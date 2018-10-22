#include <fstream>
#include <GL/glew.h>
#include "../io/io.h"
#include "application.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "../gui/panel.h"
#include "../gui/font/font_stream.h"

application::application(void)
	: display(1500, 750, "Game Engine")
{
}

auto application::init(void) -> void
{
	is_running = false;

	try
	{
		glfw_init();

		display.init(false);
		display.launch_input_handler();

		display.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glew_init();

		glEnable(GL_DEPTH_TEST);

		init_data();
		init_textures();
		init_meshes();
		init_shaders();
		init_targets();
		init_layers();
		init_gui();



		renderer = new sprite_batch_renderer(1000);
		renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "texture.font.consolas"));
		renderer->set_mesh(meshes.get_mesh_id("mesh.quad2D"), meshes);

		renderer_green = new sprite_batch_renderer(30);
		renderer_green->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "texture.green"));
		renderer_green->set_mesh(meshes.get_mesh_id("mesh.quad2D"), meshes);

		gui_vertices_cache vertices_parent;
		vertices_parent.coord.position = glm::vec2(30.0f, 30.0f);
		vertices_parent.coord.texture_coords = glm::vec2(0.0f);
		vertices_parent.size.position = glm::vec2(300.0f, 100.0f);
		vertices_parent.size.texture_coords = glm::vec2(1.0f);

		text_test = new font_stream(fonts.get_font_index("consolas"), glm::vec2(120.0f), 20.0f);

		text_parent = new panel(vertices_parent);

		text_parent->add_child(text_test);

		text_test->bind_renderer(renderer);
		text_parent->bind_renderer(renderer_green);
		text_test->submit_to_renderer(display.pixel_width(), display.pixel_height());
		text_parent->submit_to_renderer(display.pixel_width(), display.pixel_height());

		is_running = true;

		timer.start();
	}
	catch (xcp::gl_xcp const & exception)
	{
		glfw_terminate();
		display.destroy();
		std::cerr << exception.what();

		std::cin.get();
	}
}

auto application::render(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	first_out.bind();
	first_out.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	first_out.clear_color(0.1f, 0.1f, 0.1f, 1.0f);
	first_out.render(meshes, shaders);

	unbind_all_framebuffers(display.pixel_width(), display.pixel_height());

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	final_out.bind();
	final_out.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	final_out.clear_color(1.0f, 1.0f, 1.0f, 1.0f);
	final_out.render(meshes, shaders);

	glm::vec3 black(0.0f);

	auto & program = shaders[shader_handle("shader.font")];
	program.bind();
	program.send_uniform_mat4("projection_matrix", glm::value_ptr(detail::identity_matrix), 1);
	program.send_uniform_vec3("color", glm::value_ptr(black), 1);
	renderer->render(program, meshes);



//	renderer_green->render(program, meshes);

}

auto application::update(void) -> void
{
	display.refresh();

	entities.update(timer.elapsed());

	meshes.flush_renderers();

	first_out[0][0]->submit(detail::identity_matrix);
	first_out[1][0]->submit(world.env.calculate_translation(entities.get_camera().pos()));

	/* update view matrix of the camera */
	glm::mat4 view_matrix = entities.get_camera().matrix();
	first_out[0].get_view_matrix() = view_matrix;
	first_out[1].get_view_matrix() = world.env.calculate_view_matrix(view_matrix);

	final_out[0][0]->submit(detail::identity_matrix);


	renderer->flush();
	text_test->clear();
	text_test->submit_text("fps ");
	text_test->submit_text(1.0f / timer.elapsed());
	text_test->update(fonts);
	text_test->submit_to_renderer(display.pixel_width(), display.pixel_height());

	timer.reset();

	is_running = display.is_open();

	unbind_all_textures(GL_TEXTURE_2D);
	unbind_all_textures(GL_TEXTURE_CUBE_MAP);
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::init_meshes(void) -> void
{
	meshes.init();
	 
	/* process to creating a mesh : 
	-  create the handle for the mesh e.g. name ...
	-  load the model which will return a "shader handle" 
	-  set the name for the shader that you want to use for the handle 
	-  create the program with the handle (which will have all the properties selected) */

	u32 icosphere_mesh_id = meshes.create_mesh("mesh.icosphere");
	/* loads contents of ico.obj to the mesh "icosphere" */
	meshes.load_mesh("res/models/icosphere_blue.obj", icosphere_mesh_id);

	u32 cube_mesh_id = meshes.create_mesh("mesh.cube.sky");
	cube_mesh_computation computation;
	meshes.compute_mesh(computation, cube_mesh_id);

	u32 quad2D_mesh_id = meshes.create_mesh("mesh.quad2D");
	quad2D_mesh_computation quad_computation;
	meshes.compute_mesh(quad_computation, quad2D_mesh_id);
}

auto application::init_textures(void) -> void
{
	auto image = extract_png("res/textures/low_poly.png");

	u32 low_poly_id = textures.create_texture("texture.lowpoly");
	auto & low_poly_colors = textures[low_poly_id];
	create_color_texture(low_poly_colors, image.w, image.h, image.data, GL_LINEAR);

	auto image_gui = extract_png("res/textures/gui/gui_test.png");
	u32 gui_test_id = textures.create_texture("texture.gui_test");
	auto & gui_test_colors = textures[gui_test_id];
	create_color_texture(gui_test_colors, image_gui.w, image_gui.h, image_gui.data, GL_NEAREST);

	auto image_green = extract_png("res/textures/gui/green.png");
	u32 green = textures.create_texture("texture.green");
	auto & green_tex = textures[green];
	create_color_texture(green_tex, image_green.w, image_green.h, image_green.data, GL_NEAREST);
}

auto application::init_data(void) -> void
{
	entities.create(display.user_inputs());

	world.env.init_texture(textures, "res/textures/environment/sky");
}

auto application::init_shaders(void) -> void
{
	shaders.init();

	shader_handle quad2D_shader("shader.quad2D.simple");
	quad2D_shader.set(shader_property::texture_coords);
	shaders.create_program(quad2D_shader, "2D");

	shader_handle font_shader("shader.font");
	font_shader.set(shader_property::texture_coords, shader_property::font);
	shaders.create_program(font_shader, "2D");

	shader_handle low_poly_shader = meshes.create_shader_handle(meshes.get_mesh_id("mesh.icosphere"));
	low_poly_shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);
	low_poly_shader.set_name("shader.icosphere");

	shaders.create_program(low_poly_shader, "3D");



	auto & quad2D_program = shaders[shader_handle("shader.quad2D.simple")];
	quad2D_program.bind();
	quad2D_program.send_uniform_mat4("projection_matrix", glm::value_ptr(detail::identity_matrix), 1);

	auto & program = shaders[shader_handle("shader.icosphere")];
	program.bind();
	/* send light information to shader */
	lights.prepare_shader(program);

	world.env.init_shader(shaders);
}

auto application::init_layers(void) -> void
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (f32)display.pixel_width() / display.pixel_height(), 0.1f, 100000.0f);



	u32 icosphere_mesh_id = meshes.get_mesh_id("mesh.icosphere");
	auto renderer = meshes.create_renderer<basic_renderer>(icosphere_mesh_id);
	renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "texture.lowpoly"));
	renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_CUBE_MAP, 0, "texture.environment"));
	renderer->submit_pre_render(new material(glm::vec3(0.8f), glm::vec3(0.7f), glm::vec3(0.1f), 4.0f, 1.0f));
	renderer->submit_pre_render(&entities.get_pre_render_cam_pos(), false);



	u32 sky_box_mesh_id = meshes.get_mesh_id("mesh.cube.sky");
	auto sky_renderer = meshes.create_renderer<basic_renderer>(sky_box_mesh_id);
	sky_renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_CUBE_MAP, 0, "texture.environment"));

	

	u32 quad2D_mesh_id = meshes.get_mesh_id("mesh.quad2D");
	auto quad2D_renderer = meshes.create_renderer<basic_renderer>(quad2D_mesh_id);
	quad2D_renderer->submit_pre_render(new renderer_pre_render_texture_bind(textures, GL_TEXTURE_2D, 0, "texture.rendered.final"));



	layer main_layer;
	main_layer.init(renderer, shader_handle("shader.icosphere"), projection);

	layer sky_layer;
	sky_layer.init(sky_renderer, shader_handle("shader.environment"), projection);

	first_out.add_layer("layer.main", main_layer);
	first_out.add_layer("layer.sky", sky_layer);

	/* just a quad */
	layer final_main_layer;
	final_main_layer.init(quad2D_renderer, shader_handle("shader.quad2D.simple"), detail::identity_matrix);

	final_out.add_layer("layer.quad2D", final_main_layer);
}

auto application::init_targets(void) -> void
{
	i32 display_width = display.pixel_width();
	i32 display_height = display.pixel_height();

	final_out.get_fbo().width() = display_width;
	final_out.get_fbo().height() = display_height;

	framebuffer & first_out_fbo = first_out.get_fbo();
	first_out_fbo.create(display_width, display_height);
	first_out_fbo.bind(GL_FRAMEBUFFER);

	u32 first_out_color = textures.create_texture("texture.rendered.final");
	texture & first_out_tex_color = textures[first_out_color];
	create_color_texture(first_out_tex_color, display_width, display_height, nullptr, GL_LINEAR);

	renderbuffer first_out_depthbuffer;
	create_depth_renderbuffer(first_out_depthbuffer, display_width, display_height);

	first_out_fbo.attach(first_out_tex_color, GL_COLOR_ATTACHMENT0, 0);
	first_out_fbo.attach(first_out_depthbuffer, GL_DEPTH_ATTACHMENT);

	first_out_fbo.select_color_buffer(GL_COLOR_ATTACHMENT0);
}

auto application::init_gui(void) -> void
{
	fonts.create_font(textures, "consolas", "res/font/consolas");
	fonts.create_font(textures, "verdana", "res/font/verdana");
}

auto application::clean_up(void) -> void
{

}
