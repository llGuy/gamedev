#include <iostream>
#include "../io/io.h"
#include "application.h"
#include "../xcp/exception.h"
#include "../graphics/3D/model_comp/cube.h"
#include "../graphics/3D/model_comp/quad3D.h"
#include "../animation/animation_component.h"

#include <glm/gtc/type_ptr.hpp>

#define DISPLAY_WIDTH 1500
#define DISPLAY_HEIGHT 800

application::application(void)
	: display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Game Engine")
{
}

auto application::init(void) -> void
{
	try
	{
		glfw_init();

		display.init(false);
		display.launch_input_handler();
		display.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glew_init();

		shaders.init();
		world.init();
		models.init();
		lights.create();

		init_game_objects();
		init_models();
		init_fonts();
		init_textures();
		init_shaders();
		init_3D_test();
		init_2D_test();
		init_pipeline();

		time_handler.start();
		is_running = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	catch(xcp::gl_xcp const & exception)
	{
		glfw_terminate();
		display.destroy();
		std::cerr << exception.what();

		std::cin.get();
	}
}

auto application::update(void) -> void
{
	display.refresh();

	/* reset animation renderer */
	material_prototype * mat_type = materials["material.animated"];
	mat_type->flush();

	/* reset low poly renderer */
	material_prototype * mat_type_lp = materials["material.low_poly"];
	mat_type_lp->flush();

	f32 aspect = (f32)display.pixel_width() / (f32)display.pixel_height();

	camera & scene_camera = world.get_scene_camera();
	shadows.update_light_view(lights.light_position());
	shadows.update(100.0f, 1.0f, aspect, 60.0f, scene_camera.get_position(), scene_camera.get_direction());

	world.update(time_handler.elapsed());
	time_handler.reset();

	is_running = display.is_open();
}

auto application::render(void) -> void
{
	render_pipeline.execute_stages(display.pixel_width(), display.pixel_height());

	guis.render();
}

auto application::running(void) -> bool
{
	return is_running;
}

auto application::clean_up(void) -> void
{
	display.destroy();
	glfw_terminate();
}

auto application::init_game_objects(void) -> void
{
	game_object & player = world.init_game_object({ 
		glm::vec3(10.0f)
		, glm::vec3(-1.0f)
		, glm::vec3(0.4f)
		, "game_object.player" });

	component<component_behavior_key, game_object_data> key_comp{
		DEFAULT_KEY_BINDINGS, display.user_inputs() };

	component<component_behavior_mouse, game_object_data> mouse_comp{ display.user_inputs() };

	player.add_component(key_comp);
	player.add_component(mouse_comp);
	world.bind_camera_to_object(player);

	game_object & monkey = world.init_game_object({ 
		glm::vec3(0.0f)
		, glm::vec3(1.0f)
		, glm::vec3(1.0f, 0.7f, 1.0f)
		, "game_object.monkey" });

	component<component_model_matrix, game_object_data> model_matrix_comp;

	monkey.add_component(model_matrix_comp);

	game_object & platform = world.init_game_object({ 
		glm::vec3(0.0f, -4.0f, 0.0f)
		, glm::vec3(1.0f, 0.0f, 0.0f)
		, glm::vec3(3.0f)
		, "game_object.platform" });

	component<component_model_matrix, game_object_data> model_matrix_comp_platform;

	platform.add_component(model_matrix_comp_platform);
}

auto application::init_models(void) -> void
{
	animations.init(shaders, lights, materials, &world.get_scene_camera());

	monkey_model = models.init_model("model.monkey");
	models.load_model_from_obj("res/model/monkey.obj", monkey_model);

	platform_model = models.init_model("model.platform");
	models.load_model_from_obj("res/model/platform.obj", platform_model);

	player_model = models.init_model("model.player");
	std::pair xml_doc = models.load_model_from_dae(player_model, "res/model/model.dae");

	game_object & player = world.get_game_object("game_object.monkey");

	/* load all animation data for player */
	materials.add_material("material.animated"
		, MATERIAL_HIGHLY_REFLECTIVE
		, shaders[shader_handle("shader.animation3D")]
		, lights);

	animations.load_model_animation_data(player_model, xml_doc);
	animations.load_skeleton(player, player_model, materials.get_material_id("material.animated"), materials, xml_doc);
	animations.load_animation_data("animation.running", player, xml_doc);

	auto & animation_comp = player.get_component<component_animation3D>();
	animation_comp.set_animation("animation.running");

	cube_model_computation comp;
	cube_model = models.init_model("model.cube");
	models.compute_model(comp, cube_model);

	quad3D_model_computation quad3D_comp;
	quad3D_model = models.init_model("model.quad3D");
	models.compute_model(quad3D_comp, quad3D_model);
}

auto application::init_3D_test(void) -> void
{
	shadows.create(lights.light_position());

	/* initializing normal 3D renderer */
	/* just initializing uniform variables */
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), 
		(f32)display.pixel_width() / display.pixel_height(), 0.1f, 100000.0f);

	world.get_scene_camera().get_projection_matrix() = projection_matrix;

	/* initializing low poly material */

	auto low_poly_material = materials.add_material("material.low_poly"
		, MATERIAL_HIGHLY_REFLECTIVE
		, shaders[shader_handle("shader.low_poly")]
		, lights);

	low_poly_material->set_texture_2D(textures.get_texture("texture.low_poly"));
	low_poly_material->set_texture_3D(textures.get_texture("texture.sky"));

	component<component_render, game_object_data> render_comp_platform{ 
		platform_model
		, materials.get_material_id("material.low_poly")
        , materials };

	world.get_game_object("game_object.platform").add_component(render_comp_platform);

	/* initialize sky renderer and material */
	auto sky_material = materials.add_material("material.sky"
		, material_light_info() /* no lighting */
		, shaders[shader_handle("shader.sky")]
		, lights);

	sky_material->set_texture_3D(textures.get_texture("texture.sky"));
	sky_material->toggle_lighting();

	material * sky = new material{ cube_model, glm::scale(glm::vec3(1000.0f)), materials.get_material_id("material.sky") };

	materials.submit(sky);

	/* initializing animation material_prototype textures */
	materials["material.animated"]->set_texture_2D(textures.get_texture("texture.player"));
}

auto application::init_2D_test(void) -> void
{
	guis.init(shaders);

	panel * stream_panel = guis.init_gui_set<panel>("gui.panel.font_test", dont_render_gui);

	gui_cache vertices;
	vertices.position = vertex2D{ glm::vec2(30.0f, 50.0f), glm::vec2(0.0f, 0.0f) };
	vertices.size = vertex2D{ glm::vec2(100.0f, 50.0f), glm::vec2(1.0f, 1.0f) };

	stream_panel->init(vertices, nullptr);

	font_stream * stream = guis.init_font_stream("gui.panel.font_test", "gui.font.stream.font_test", "comic", glm::vec2(320.0f), 20.0f);
	stream->submit_text("Super Awesome new Rendering System!!!");

	guis.update(display.pixel_width(), display.pixel_height());
}

auto application::init_shaders(void) -> void
{
	shader_handle shader = models.create_shader_handle(platform_model);
	shader.set(shader_property::linked_to_gsh, shader_property::sharp_normals);
	shader.set_name("shader.low_poly");
	shaders.create_program(shader, "3D");

	shader_handle sky_shader_handle("shader.sky");
	glsl_shader sky_vsh = shaders.create_shader(GL_VERTEX_SHADER, sky_shader_handle, extract_file("src/shaders/environment/vsh.shader"));
	glsl_shader sky_fsh = shaders.create_shader(GL_FRAGMENT_SHADER, sky_shader_handle, extract_file("src/shaders/environment/fsh.shader"));
	shaders.combine(sky_shader_handle, sky_vsh, sky_fsh);

	auto blur_fsh_raw = extract_file("src/shaders/post_processing/gaussian_blur/fsh.shader");

	shader_handle vertical_blur("shader.vertical_blur");
	glsl_shader vertical_vsh = shaders.create_shader(GL_VERTEX_SHADER, vertical_blur, extract_file("src/shaders/post_processing/gaussian_blur/v_vsh.shader"));
	glsl_shader blur_fsh = shaders.create_shader(GL_FRAGMENT_SHADER, vertical_blur, blur_fsh_raw);
	shaders.combine(vertical_blur, vertical_vsh, blur_fsh);

	shader_handle horizontal_blur("shader.horizontal_blur");
	glsl_shader horizontal_vsh = shaders.create_shader(GL_VERTEX_SHADER, horizontal_blur, extract_file("src/shaders/post_processing/gaussian_blur/h_vsh.shader"));
	shaders.combine(horizontal_blur, horizontal_vsh, blur_fsh);
}

auto application::init_textures(void) -> void
{
	auto * low_poly_texture = textures.init_texture("texture.low_poly");
	textures.load_texture_png("res/textures/low_poly.png", low_poly_texture, GL_LINEAR);

	auto * test_gui_texture = textures.init_texture("texture.gui_test");
	textures.load_texture_png("res/textures/gui_test.png", test_gui_texture, GL_NEAREST, flip_vertically);

	auto * player_texture = textures.init_texture("texture.player");
	textures.load_texture_png("res/textures/player_color.png", player_texture, GL_LINEAR, flip_vertically);

	auto * sky_texture = textures.init_texture("texture.sky");
	textures.load_3D_texture_png("res/textures/sky", sky_texture);

	auto * scene_color = textures.init_texture("texture.scene_color");
	create_color_texture(*scene_color, display.pixel_width(), display.pixel_height(), nullptr, GL_LINEAR);

	auto * hblur1 = textures.init_texture("texture.hblur1");
	create_color_texture(*hblur1, display.pixel_width() / 2, display.pixel_height() / 2, nullptr, GL_LINEAR);

	auto * vblur1 = textures.init_texture("texture.vblur1");
	create_color_texture(*vblur1, display.pixel_width() / 2, display.pixel_height() / 2, nullptr, GL_LINEAR);

	auto * shadow_map_texture = textures.init_texture("texture.shadow_map");
	create_depth_texture(*shadow_map_texture, shadow_handler::get_shadow_map_size(), shadow_handler::get_shadow_map_size());
}

auto application::init_fonts(void) -> void
{
	guis.init_font_type(textures, "comic", "res/font/comic");
	guis.init_font_type(textures, "droid", "res/font/droid");
}

auto application::init_pipeline(void) -> void
{
	u32 display_w = display.pixel_width();
	u32 display_h = display.pixel_height();


	/* create shadow stage */
	render_stage_create_info info;
	info.width = shadow_handler::get_shadow_map_size();
	info.height = shadow_handler::get_shadow_map_size();
	info.color_name = "";
	info.depth_name = "texture.shadow_map";
	info.create_flags = RENDER_STAGE_CREATE_INFO_COLOR_NONE
		| RENDER_STAGE_CREATE_INFO_DEPTH_TEXTURE;

	render_pipeline.add_render_stage<render_stage3D>("render_stage.shadow_pass", &materials, &shadows.get_shadow_camera());
	render_pipeline.create_render_stage("render_stage.shadow_pass", info, renderbuffers, textures);

	auto depth = renderbuffers.add_renderbuffer("renderbuffer.first_stage");
	create_depth_renderbuffer(*depth, display_w, display_h);

	info.width = display_w;
	info.height = display_h;
	info.color_name = "texture.scene_color";
	info.depth_name = "renderbuffer.scene_depth";
	info.create_flags = RENDER_STAGE_CREATE_INFO_COLOR_TEXTURE
		| RENDER_STAGE_CREATE_INFO_DEPTH_RENDERBUFFER;

	render_pipeline.add_render_stage<render_stage3D>("render_stage.init", &materials, &world.get_scene_camera());
	render_pipeline.create_render_stage("render_stage.init", info, renderbuffers, textures);

	auto depth_vblur1 = renderbuffers.add_renderbuffer("renderbuffer.vblur1");
	create_depth_renderbuffer(*depth_vblur1, display_w / 2, display_h / 2);

	info.width = display_w / 2;
	info.height = display_h / 2;
	info.color_name = "texture.vblur1";
	info.depth_name = "renderbuffer.vblur1_depth";
	info.create_flags = RENDER_STAGE_CREATE_INFO_COLOR_TEXTURE
		| RENDER_STAGE_CREATE_INFO_DEPTH_RENDERBUFFER;

	auto vblur1_stage = render_pipeline.add_render_stage<render_stage2D>("render_stage.vblur1", shaders[shader_handle("shader.vertical_blur")], &guis);
	render_pipeline.create_render_stage("render_stage.vblur1", info, renderbuffers, textures);

	vblur1_stage->add_texture2D_bind(TEXTURE2D_BINDING_PREVIOUS_OUTPUT);
	vblur1_stage->add_uniform_command(new uniform_float("target_height", DISPLAY_HEIGHT / 2));

	auto depth_hblur1 = renderbuffers.add_renderbuffer("renderbuffer.hblur1");
	create_depth_renderbuffer(*depth_hblur1, display_w / 2, display_h / 2);

	info.width = display_w / 2;
	info.height = display_h / 2;
	info.color_name = "texture.hblur1";
	info.depth_name = "renderbuffer.hblur1_depth";
	info.create_flags = RENDER_STAGE_CREATE_INFO_COLOR_TEXTURE
		| RENDER_STAGE_CREATE_INFO_DEPTH_RENDERBUFFER;

	auto hblur1_stage = render_pipeline.add_render_stage<render_stage2D>("render_stage.hblur1", shaders[shader_handle("shader.horizontal_blur")], &guis);
	render_pipeline.create_render_stage("render_stage.hblur1", info, renderbuffers, textures);
	
	hblur1_stage->add_texture2D_bind(TEXTURE2D_BINDING_PREVIOUS_OUTPUT);
	hblur1_stage->add_uniform_command(new uniform_float("target_width", DISPLAY_WIDTH / 2));

	auto final_stage = render_pipeline.add_render_stage<render_stage2D>("render_stage.final", nullptr, &guis);
	final_stage->set_to_default(display_w, display_h);
	final_stage->add_texture2D_bind(textures.get_texture("texture.scene_color"));
}