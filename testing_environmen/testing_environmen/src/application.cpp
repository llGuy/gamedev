#include <thread>

#include "render_func.h"
#include "application.h"
#include "detail.h"
#include "renderbuffer.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "noise_generator.h"
#include "mesh.h"

#include <time.h>

#include "ecs/graphics.h"
#include "ecs/depth_render.h"

#define PLANE_RAD 80.0f

application::application(i32 w, i32 h)
	: appl_window(w, h, "testing"), resources(""), 
	scene_platform(glm::vec3(-PLANE_RAD, 0, PLANE_RAD), glm::vec3(-PLANE_RAD, 0, -PLANE_RAD), 
		glm::vec3(PLANE_RAD, 0, PLANE_RAD), glm::vec3(PLANE_RAD, 0, -PLANE_RAD)), 
	light_position(10000.0f, 40000.0f, 10000.0f),
	blur_stages{ blur_stage{1, 2}, blur_stage{4, 6} }, max_fps(120.0f)
{
	srand(14398);
	projection_matrix = glm::perspective(glm::radians(60.0f), (float)w / h, 0.1f, 100000.0f);
}

auto application::init(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	try
	{

		time_handler.start();
		default_target.create(appl_window.pixel_width(), appl_window.pixel_height());
		scene_platform.create(resources);

		for (auto & blur_stage : blur_stages)
		{
			blur_stage.v.create(appl_window.pixel_width(), appl_window.pixel_height());
			blur_stage.h.create(appl_window.pixel_width(), appl_window.pixel_height());
		}

		render_quad.create(resources);

		traces.create();
		shadows.create(light_position);
		create_test_fbo();

		dof_stage.create(appl_window.pixel_width(), appl_window.pixel_height());

		unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());

		guis.create(resources);
		guis.push(glm::vec2(-0.5f, -0.5f), 0.3f);
		guis.push(glm::vec2(-0.5f, 0.5f), 0.3f);
		guis.push(glm::vec2(0.0f), 1.0f);

		glm::vec3 color{ 1, 1, 1 };

		quad_3D_shaders.attach(shader(GL_VERTEX_SHADER, "quad_3D_vsh.shader"));
		quad_3D_shaders.attach(shader(GL_GEOMETRY_SHADER, "quad_3D_gsh.shader"));
		quad_3D_shaders.attach(shader(GL_FRAGMENT_SHADER, "quad_3D_fsh.shader"));
		quad_3D_shaders.link("vertex_position", "vertex_color");

		quad_3D_shaders.bind();
		quad_3D_shaders.send_uniform_mat4("projection_matrix", glm::value_ptr(projection_matrix), 1);
		quad_3D_shaders.send_uniform_vec3("light_position", glm::value_ptr(light_position), 1);

		model_loader.init(projection_matrix, light_position, shadows.get_shadow_bias());

		model_loader.create_model("tree1");
		model_loader.create_model("tree2");
		model_loader.create_model("tree3");
		model_loader.create_model("tree4");

		model_loader.create_model("rock1");
		model_loader.create_model("rock2");

		model_loader.create_model("sphere");
		model_loader.create_model("mesh");
		model_loader.create_model("2D quad");

		model_loader.load_model("res/models/tree.obj", "tree1");
		model_loader.load_model("res/models/tree2.obj", "tree2");
		model_loader.load_model("res/models/tree3.obj", "tree3");
		model_loader.load_model("res/models/tree4.obj", "tree4");
		model_loader.load_model("res/models/rock.obj", "rock1");
		model_loader.load_model("res/models/rock2.obj", "rock2");
		model_loader.load_model("res/models/round.obj", "sphere");

		terrain_dimensions = 20;

		//model_loader.load_static_model(shape<mesh, perlin_noise_generator>(terrain_dimensions, terrain_dimensions, 
			//perlin_noise_generator(terrain_dimensions, terrain_dimensions, 5.f, 1.0f)), "mesh");

		model_loader.load_static_model(shape<mesh, static_noise_generator>(terrain_dimensions, terrain_dimensions,
			static_noise_generator()), "mesh");

		model_loader.load_static_model(shape<textured_quad_2D>(), "2D quad");

		entities.create(appl_window.user_inputs(), shadows.get_shaders(), "sphere", traces, puffs, model_loader);

		/* trees */
		add_entity(glm::vec3(30.0f, 0.0f, 30.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(10.0f), "tree1");
		add_entity(glm::vec3(80.0f, 0.0f, -65.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f), "tree3");
		add_entity(glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(4.0f), "tree3");
		add_entity(glm::vec3(-50.0f, 0.0f, 55.0f), glm::vec3(0.2f, 0.0f, 1.0f), glm::vec3(15.0f), "tree1");
		/* even more trees */
		/*add_entity(glm::vec3(-100.0f, 0.0f, -80.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(10.0f), "tree1");
		add_entity(glm::vec3(80.0f, 0.0f, 95.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(4.0f), "tree4");
		add_entity(glm::vec3(-50.0f, 0.0f, 90.0f), glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(4.0f), "tree3");
		add_entity(glm::vec3(-50.0f, 0.0f, -95.0f), glm::vec3(0.2f, 0.0f, 1.0f), glm::vec3(6.0f), "tree4");

		add_entity(glm::vec3(-8.0f, 0.0f, 7.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f), "rock1");
		add_entity(glm::vec3(8.0f, 0.0f, -7.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(3.0f), "rock2");
		add_entity(glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(4.0f, 3.0f, 3.0f), "rock2");
		add_entity(glm::vec3(-15.0f, 0.0f, -11.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f), "rock2");

		add_entity(glm::vec3(-28.0f, 0.0f, 27.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(6.0f), "rock1");
		add_entity(glm::vec3(28.0f, 0.0f, -27.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(8.0f), "rock2");
		add_entity(glm::vec3(-22.0f, 0.0f, -12.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(10.0f, 3.0f, 3.0f), "rock2");
		add_entity(glm::vec3(-95.0f, 0.0f, -12.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(15.0f), "rock1");

		add_entity(glm::vec3(-48.0f, 0.0f, 47.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(6.0f, 4.0f, 5.0f), "rock1");
		add_entity(glm::vec3(48.0f, 0.0f, -47.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(8.0f, 4.0f, 6.0f), "rock2");
		add_entity(glm::vec3(-22.0f, 0.0f, -52.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(10.0f, 5.0f, 7.0f), "rock2");
		add_entity(glm::vec3(+95.0f, 0.0f, -12.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(15.0f), "rock2");*/

		create_textures();

		running = true;

	}
	catch (xcp::gl_xcp const & exception)
	{
		std::cout << std::endl;

		std::cout << exception.what() << std::endl;

		appl_window.destroy();

		glfw_terminate();

		running = false;
	}
}

auto application::init_window(void) -> void
{
	appl_window.window_hint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
	appl_window.window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	appl_window.window_hint(GLFW_SAMPLES, 4);

	appl_window.init(false);
	appl_window.launch_input_handler();
	appl_window.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto application::render(void) -> void
{
	try
	{
		default_target.bind_aa();

		clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.0f, 1.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		glm::vec3 color{ 0.3f, 0.6f, 0.0f };
		glm::vec3 color2 = glm::vec3(0.6, 0.3f, 0.0f);

		quad_3D_shaders.bind();
		auto view_matrix = glm::lookAt(entities.cam().pos(), entities.cam().pos() + entities.cam().dir(), detail::up);

		shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

		quad_3D_shaders.send_uniform_vec3("light_position", glm::value_ptr(light_position), 1);
		quad_3D_shaders.send_uniform_vec3("camera_pos", glm::value_ptr(entities.cam().pos()), 1);
		quad_3D_shaders.send_uniform_mat4("shadow_bias", glm::value_ptr(shadow_bias), 1);
		quad_3D_shaders.send_uniform_mat4("projection_matrix", glm::value_ptr(projection_matrix), 1);
		quad_3D_shaders.send_uniform_mat4("view_matrix", glm::value_ptr(view_matrix), 1);

		f32 scale = 15.0f;

		glm::vec3 cam_pos = entities.cam().pos();

		glm::mat4 translation2 = glm::translate(glm::vec3(-terrain_dimensions, 0.0f, -terrain_dimensions) / 2.0f * scale) *
			glm::scale(glm::vec3(scale, scale, scale));
		quad_3D_shaders.send_uniform_vec3("color", glm::value_ptr(color2), 1);
		quad_3D_shaders.send_uniform_mat4("model_matrix", glm::value_ptr(translation2), 1);

		model_loader.render("mesh");

		traces.render(projection_matrix, view_matrix);

		glm::vec3 red(0.7f, 0.0f, 0.0f);
		puffs.render(quad_3D_shaders, "model_matrix", "color", "sphere", model_loader);

		render_pass_data data{ projection_matrix, view_matrix, shadow_bias,
			light_position, entities.cam().pos(), shadows.get_depth_map(), low_poly_map };

		model_loader.prepare(data);

		entities.update_only<graphics>();

		model_loader.batch_render_all();

		default_target.blit();
		default_target.bind();

		render_depth();

		/* rendering the blur stages */
		i32 window_width = appl_window.pixel_width();
		i32 window_height = appl_window.pixel_height();
		blur_stages[0].h.render(render_quad, default_target.output(), window_width, window_height);
		blur_stages[0].v.render(render_quad, blur_stages[0].h.output(), window_width, window_height);

		for (u32 i = 1; i < blur_stages.size(); ++i)
		{
			blur_stages[i].h.render(render_quad, blur_stages[i - 1].v.output(), window_width, window_height);
			blur_stages[i].v.render(render_quad, blur_stages[i].h.output(), window_width, window_height);
		}

		/* render scene to texture */
		unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());


		blur_stages[1].v.output().bind(GL_TEXTURE_2D, 2);
		default_target.depth_out().bind(GL_TEXTURE_2D, 1);

		dof_stage.render(render_quad, default_target.output(), window_width, window_height);


		clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1, 1, 1);

		glDisable(GL_DEPTH_TEST);
		guis.prepare((gui_slot)2, 1);
		auto & tex2 = dof_stage.output();
		tex2.bind(GL_TEXTURE_2D, 0);
		guis.render(model_loader);

		//render_depth_gui();

		glEnable(GL_DEPTH_TEST);

		model_loader.batch_flush_all();
	}
	catch (xcp::gl_xcp const & exception)
	{
		std::cout << std::endl;

		std::cout << exception.what() << std::endl;

		appl_window.destroy();

		glfw_terminate();

		running = false;
	}
}

auto application::render_color(void) -> void
{
	test_fbo.bind();

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = shadows.get_projection();
	glm::mat4 view = shadows.get_light_view();

	glm::vec3 color{ 0.3f, 0.6f, 0.0f };
	glm::vec3 color2 = glm::vec3(0.6, 0.3f, 0.0f);

	quad_3D_shaders.bind();
	auto view_matrix = glm::lookAt(entities.cam().pos(), entities.cam().pos() + entities.cam().dir(), detail::up);

	shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

	quad_3D_shaders.send_uniform_vec3("light_position", glm::value_ptr(light_position), 1);
	quad_3D_shaders.send_uniform_vec3("camera_pos", glm::value_ptr(entities.cam().pos()), 1);
	quad_3D_shaders.send_uniform_mat4("shadow_bias", glm::value_ptr(shadow_bias), 1);
	quad_3D_shaders.send_uniform_mat4("projection_matrix", glm::value_ptr(projection), 1);
	quad_3D_shaders.send_uniform_mat4("view_matrix", glm::value_ptr(view), 1);

	quad_3D_shaders.send_uniform_vec3("color", glm::value_ptr(color), 1);

	f32 scale = 10.0f;

	glm::vec3 cam_pos = entities.cam().pos();

	glm::mat4 translation2 = glm::translate(glm::vec3(-terrain_dimensions, 0.0f, -terrain_dimensions) / 2.0f * scale) *
		glm::scale(glm::vec3(scale, scale, scale));

	quad_3D_shaders.send_uniform_vec3("color", glm::value_ptr(color2), 1);
	quad_3D_shaders.send_uniform_mat4("model_matrix", glm::value_ptr(translation2), 1);
	model_loader.render("mesh");

	traces.render(projection_matrix, view);

	glm::vec3 red(0.7f, 0.0f, 0.0f);
	puffs.render(quad_3D_shaders, "model_matrix", "color", "sphere", model_loader);

	render_pass_data data{ projection, view, shadow_bias,
		light_position, entities.cam().pos(), shadows.get_depth_map(), low_poly_map };
	model_loader.prepare(data);

	entities.render<graphics>(true);

	unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());
}

auto application::update(void) -> void
{
	f32 elapsed = time_handler.elapsed();

	traces.clear();
	appl_window.refresh();
	puffs.update(elapsed);
	auto & cam = entities.cam();

	f32 aspect = (f32)appl_window.pixel_width() / appl_window.pixel_height();
	shadows.update(200.0f, 1.0f, aspect, 60.0f, cam.pos(), cam.dir());

	/* update light position */
	auto & inputs = appl_window.user_inputs();
	glm::vec3 lateral_player_direction = entities.main_player_lateral_direction();
	if (inputs.got_key(GLFW_KEY_UP)) light_position += lateral_player_direction * elapsed * 5000.0f;
	if (inputs.got_key(GLFW_KEY_DOWN)) light_position -= lateral_player_direction * elapsed * 5000.0f;
	if (inputs.got_key(GLFW_KEY_RIGHT)) light_position += glm::cross(lateral_player_direction, detail::up) * elapsed * 5000.0f;
	if (inputs.got_key(GLFW_KEY_LEFT)) light_position -= glm::cross(lateral_player_direction, detail::up) * elapsed * 5000.0f;
	if (inputs.got_key(GLFW_KEY_HOME)) light_position += detail::up * elapsed * 5000.0f;
	if (inputs.got_key(GLFW_KEY_END)) 
		light_position -= detail::up * elapsed * 5000.0f;

	shadows.update_light_view(light_position);

	if (inputs.win_resized())
	{
		i32 w = appl_window.pixel_width() = (i32)inputs.window_size().x;
		i32 h = appl_window.pixel_height() = (i32)inputs.window_size().y;

		default_target.reset(w, h);
		for (auto & bstage : blur_stages)
		{
			bstage.v.reset(w, h);
			bstage.h.reset(w, h);
		}
		dof_stage.reset(w, h);
	}

	if (running) running = appl_window.is_open();
	
	//wait(time_handler.elapsed());

	if (inputs.got_key(GLFW_KEY_G))
		printf("%f\n", 1.0f / time_handler.elapsed());

	entities.update(time_handler.elapsed());

	time_handler.reset();
}

auto application::wait(f32 elapsed) -> void
{
	f32 min_time_per_fps = 1.0f / max_fps;

	if (elapsed < min_time_per_fps)
	{
		f32 seconds_to_wait = min_time_per_fps - elapsed;

		std::this_thread::sleep_for(std::chrono::duration<double>(seconds_to_wait));
	}
}

auto application::is_running(void) -> bool
{
	return running;
}

auto application::destroy(void) -> void
{
	appl_window.destroy();
} 

auto application::render_depth(void) -> void
{
	auto & fbo = shadows.get_fbo();
	auto shaders = shadows.get_shaders();
	fbo.bind();

	clear(GL_DEPTH_BUFFER_BIT, 1, 1, 1);

	shaders.bind();
	auto & view = entities.cam().matrix();

	glm::mat4 mv = shadows.get_projection() * shadows.get_light_view();

	shadow_bias = shadows.get_shadow_bias() * mv;

	shaders.send_uniform_mat4("vp", glm::value_ptr(mv), 1);

	/* submit depth of the main players model */
	entities.render<depth>(true);
	model_loader.batch_render_all_raw();

	f32 scale = 10.0f;
//	glm::mat4 translation2 = glm::translate(glm::vec3(-terrain_dimensions, 0.0f, -terrain_dimensions) / 2.0f * scale) *
//		glm::scale(glm::vec3(scale, 1.0f, scale));
//	shaders.send_uniform_mat4();
//	model_loader.render("mesh");

	unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());
}

auto application::render_depth_gui(void) -> void
{
	glDisable(GL_DEPTH_TEST);

	guis.prepare(gui_slot::DEBUG_0, 0);
	auto & tex1 = shadows.get_depth_map();
	tex1.bind(GL_TEXTURE_2D, 0);
	guis.render(model_loader);

	guis.prepare(gui_slot::DEBUG_1, 1);
	auto & tex2 = test_tex;
	tex2.bind(GL_TEXTURE_2D, 0);
	guis.render(model_loader);

	glEnable(GL_DEPTH_TEST);
}

auto application::create_test_fbo(void) -> void
{
	test_fbo.create(2048, 2048);
	test_fbo.bind();

	test_tex.create();
	test_tex.bind(GL_TEXTURE_2D);
	test_tex.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT, nullptr, 2048, 2048);
	test_tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	test_tex.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	test_fbo.attach(test_tex, GL_COLOR_ATTACHMENT0, 0);

	renderbuffer rnd;
	rnd.create();
	rnd.bind();
	rnd.set_storage(GL_DEPTH_COMPONENT, 2048, 2048);

	test_fbo.attach(rnd, GL_DEPTH_ATTACHMENT);
}

auto application::add_entity(glm::vec3 const & p, glm::vec3 const & d, glm::vec3 const & scale, std::string const & model) -> void
{
	entities.add_entity(p, d, model, quad_3D_shaders, shadows.get_shaders(), scale, model_loader);
}

auto application::create_textures(void) -> void
{
	low_poly_map.create();
	low_poly_map.bind(GL_TEXTURE_2D);

	auto img = resources.load<image>("res/models/low_poly.png");

	low_poly_map.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, img.data, img.w, img.h);

	low_poly_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	low_poly_map.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

auto application::set_max_framerate(f32 fps) -> void
{
	max_fps = fps;
}