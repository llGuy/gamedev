#include "render_func.h"
#include "application.h"
#include "detail.h"
#include "renderbuffer.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ecs/graphics.h"
#include "ecs/depth_render.h"

#define PLANE_RAD 50.0f

application::application(i32 w, i32 h)
	: appl_window(w, h, "testing"), resources(""), 
	scene_platform(glm::vec3(-PLANE_RAD, 0, PLANE_RAD), glm::vec3(-PLANE_RAD, 0, -PLANE_RAD), 
		glm::vec3(PLANE_RAD, 0, PLANE_RAD), glm::vec3(PLANE_RAD, 0, -PLANE_RAD)), a_cube(2),
	light_position(-20000.0f, 50000.0f, -20000.0f),
	blur_stages{ blur_stage{2, 4}, blur_stage{8, 10} }
{
	projection_matrix = glm::perspective(glm::radians(60.0f), (float)w / h, 0.1f, 1000.0f);
}

auto application::init(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	appl_window.window_hint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	time.start();
	default_target.create(appl_window.pixel_width(), appl_window.pixel_height());
	a_cube.create(resources);
	scene_platform.create(resources);

	for (auto & blur_stage : blur_stages)
	{
		blur_stage.v.create(appl_window.pixel_width(), appl_window.pixel_height());
		blur_stage.h.create(appl_window.pixel_width(), appl_window.pixel_height());
	}

	entities.create(appl_window.user_inputs(), shadows.get_shaders(), a_cube, traces, puffs);
	add_entity(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	add_entity(glm::vec3(-30.0f, 0.0f, 30.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	add_entity(glm::vec3(16.0f, 0.0f, 30.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
	add_entity(glm::vec3(8.0f, 0.0f, -10.0f), glm::vec3(-1.0f, 0.0f, -0.5f));
	add_entity(glm::vec3(19.0f, 0.0f, -8.0f), glm::vec3(-0.5f, 0.0f, 1.0f));
	add_entity(glm::vec3(-19.0f, 0.0f, -10.0f), glm::vec3(-1.0f, 0.0f, -0.5f));
	add_entity(glm::vec3(-20.0f, 0.0f, -30.0f), glm::vec3(-1.0f, 0.0f, -0.5f));

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

	quad_3D_shaders.create_shader(GL_VERTEX_SHADER, "quad_3D_vsh.shader");
	quad_3D_shaders.create_shader(GL_GEOMETRY_SHADER, "quad_3D_gsh.shader");
	quad_3D_shaders.create_shader(GL_FRAGMENT_SHADER, "quad_3D_fsh.shader");
	quad_3D_shaders.link_shaders("vertex_position");
	quad_3D_shaders.get_uniform_locations("projection_matrix", "view_matrix", "color", "model_matrix", "light_position", "shadow_bias", "camera_pos");
	quad_3D_shaders.use();
	quad_3D_shaders.uniform_mat4(&projection_matrix[0][0], 0);
	quad_3D_shaders.uniform_3f(&light_position[0], 4);
}

auto application::init_window(void) -> void
{
	appl_window.window_hint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	appl_window.window_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
	appl_window.window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	appl_window.init();
	appl_window.launch_input_handler();
	appl_window.set_window_input_mode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/*auto application::render_scene(render_params & params) -> void
{
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.0f, 1.0f, 1.0f);

	glm::vec3 color{ 0.3f, 0.6f, 0.0f };
	glm::vec3 color2 = glm::vec3(0.6, 0.3f, 0.0f);

	params.shaders.use();
	auto view_matrix = glm::lookAt(entities.cam().pos(), entities.cam().pos() + entities.cam().dir(), detail::up);

	shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

	params.shaders.uniform_3f(&light_position[0], 4);
	params.shaders.uniform_3f(&entities.cam().pos()[0], 6);
	params.shaders.uniform_mat4(&shadow_bias[0][0], 5);
	params.shaders.uniform_mat4(&projection_matrix[0][0], 0);
	params.shaders.uniform_mat4(&view_matrix[0][0], 1);

	quad_3D_shaders.uniform_3f(&color[0], 2);

	entities.update_only<graphics>();

	glm::mat4 translation2 = detail::identity_matrix;
	quad_3D_shaders.uniform_3f(&color2[0], 2);
	quad_3D_shaders.uniform_mat4(&translation2[0][0], 3);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

	traces.render(projection_matrix, view_matrix);

	glm::vec3 red(0.7f, 0.0f, 0.0f);
	//quad_3D_shaders.uniform_3f(&red[0], 2);
	puffs.render(quad_3D_shaders, 3, 2, a_cube);
}*/

auto application::render(void) -> void
{
	default_target.bind();

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.0f, 1.0f, 1.0f);

	glm::vec3 color{ 0.3f, 0.6f, 0.0f };
	glm::vec3 color2 = glm::vec3(0.6, 0.3f, 0.0f);

	quad_3D_shaders.use();
	auto view_matrix = glm::lookAt(entities.cam().pos(), entities.cam().pos() + entities.cam().dir(), detail::up);

	shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

	quad_3D_shaders.uniform_3f(&light_position[0], 4);
	quad_3D_shaders.uniform_3f(&entities.cam().pos()[0], 6);
	quad_3D_shaders.uniform_mat4(&shadow_bias[0][0], 5);
	quad_3D_shaders.uniform_mat4(&projection_matrix[0][0], 0);
	quad_3D_shaders.uniform_mat4(&view_matrix[0][0], 1);

	quad_3D_shaders.uniform_3f(&color[0], 2);

	entities.update_only<graphics>();

	glm::mat4 translation2 = detail::identity_matrix;
	quad_3D_shaders.uniform_3f(&color2[0], 2);
	quad_3D_shaders.uniform_mat4(&translation2[0][0], 3);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

	traces.render(projection_matrix, view_matrix);

	glm::vec3 red(0.7f, 0.0f, 0.0f);
	//quad_3D_shaders.uniform_3f(&red[0], 2);
	puffs.render(quad_3D_shaders, 3, 2, a_cube);

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


	blur_stages[0].v.output().bind(GL_TEXTURE_2D, 2);
	default_target.depth_out().bind(GL_TEXTURE_2D, 1);

	dof_stage.render(render_quad, default_target.output(), window_width, window_height);


	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1, 1, 1);

	glDisable(GL_DEPTH_TEST);
	guis.prepare((gui_slot)2, 1);
	auto & tex2 = dof_stage.output();
	tex2.bind(GL_TEXTURE_2D, 0);
	guis.render();

	glEnable(GL_DEPTH_TEST);

}

auto application::update(void) -> void
{
	traces.clear();
	appl_window.refresh();
	entities.update(time.elapsed());
	puffs.update(time.elapsed());
	auto & cam = entities.cam();

	f32 aspect = (f32)appl_window.pixel_width() / appl_window.pixel_height();
	shadows.update(0.1f, 80.0f, aspect, 60.0f, cam.pos(), cam.dir());

	f32 elapsed = time.elapsed();

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

	time.reset();
}

auto application::running(void) -> bool
{
	return appl_window.is_open();
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

	shaders.use();
	auto & view = entities.cam().matrix();

	glm::mat4 mv = shadows.get_projection() * shadows.get_light_view();

	shadow_bias = shadows.get_shadow_bias() * mv;

	shaders.uniform_mat4(&mv[0][0], 0);

	entities.update_only<depth>();

	glm::mat4 plat_translation = glm::mat4(1);
	shaders.uniform_mat4(&plat_translation[0][0], 1);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

	traces.render(shadows.get_projection(), shadows.get_light_view());

	quad_3D_shaders.use();
	quad_3D_shaders.uniform_mat4(&shadows.get_projection()[0][0], 0);
	quad_3D_shaders.uniform_mat4(&shadows.get_light_view()[0][0], 1);
	puffs.render(quad_3D_shaders, 3, 2, a_cube);

	unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());
}

auto application::render_color(void) -> void
{
	auto & fbo = test_fbo;
	fbo.bind();
	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1, 1, 1);

	glm::vec3 color{ 0.2f };
	glm::vec3 color2{ 0.1f, 0.1f, 0.5f };

	quad_3D_shaders.use();
	auto view_matrix = shadows.get_light_view();

	shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

	quad_3D_shaders.uniform_mat4(&shadow_bias[0][0], 5);
	quad_3D_shaders.uniform_mat4(&shadows.get_projection()[0][0], 0);
	quad_3D_shaders.uniform_mat4(&view_matrix[0][0], 1);
	quad_3D_shaders.uniform_3f(&color[0], 2);

	glm::vec3 translation(0.0f, 1.0f, 0.0f);
	quad_3D_shaders.uniform_3f(&translation[0], 3);
	render_model(a_cube, GL_TRIANGLES);

	glm::vec3 translation1(-50.0f, 1.0f, 30.0f);
	quad_3D_shaders.uniform_3f(&translation1[0], 3);
	render_model(a_cube, GL_TRIANGLES);

	glm::vec3 translation2(0.0f, 0.0f, 0.0f);
	quad_3D_shaders.uniform_3f(&color2[0], 2);
	quad_3D_shaders.uniform_3f(&translation2[0], 3);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

	unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());
}

auto application::render_depth_gui(void) -> void
{
	glDisable(GL_DEPTH_TEST);

	guis.prepare(gui_slot::DEBUG_0, 0);
	auto & tex1 = default_target.depth_out();
	tex1.bind(GL_TEXTURE_2D, 0);
	guis.render();

/*	guis.prepare(gui_slot::DEBUG_1, 1);
	auto & tex2 = test_tex;
	tex2.bind(GL_TEXTURE_2D, 0);
	guis.render();*/

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

auto application::add_entity(glm::vec3 const & p, glm::vec3 const & d) -> void
{
	entities.add_entity(p, d, a_cube, quad_3D_shaders, shadows.get_shaders());
}
