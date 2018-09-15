#include "render_func.h"
#include "application.h"
#include "detail.h"
#include "renderbuffer.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#define PLANE_RAD 50.0f

application::application(i32 w, i32 h)
	: appl_window(w, h, "landscaper"), resources(""), 
	scene_platform(glm::vec3(-PLANE_RAD, 0, PLANE_RAD), glm::vec3(-PLANE_RAD, 0, -PLANE_RAD), 
		glm::vec3(PLANE_RAD, 0, PLANE_RAD), glm::vec3(PLANE_RAD, 0, -PLANE_RAD)), a_cube(2),
	light_position(-50000.0f, 10000.0f, -50000.0f)
{
	projection_matrix = glm::perspective(glm::radians(60.0f), (float)w / h, 0.1f, 1000.0f);
}

auto application::init(void) -> void
{
	glEnable(GL_DEPTH_TEST);

	time.start();
	a_cube.create(resources);
	scene_platform.create(resources);
	gui_quad.create(resources);
	entities.create(appl_window.user_inputs());
	shadows.create(glm::vec3(-1, -1, -1));
	create_test_fbo();
	unbind_all_framebuffers(appl_window.pixel_width(), appl_window.pixel_height());

	guis.create(resources);
	guis.push(glm::vec2(-0.5f, -0.5f), 0.3f);
	guis.push(glm::vec2(-0.5f, 0.5f), 0.3f);

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

auto application::render(void) -> void
{
	glCullFace(GL_BACK);

	clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 1.0f, 1.0f, 1.0f);

	glm::vec3 color{ 0.2f };
	glm::vec3 color2{ 0.07, 0.07f, 0.4f };

	quad_3D_shaders.use();
	auto view_matrix = glm::lookAt(entities.cam().pos(), entities.cam().pos() + entities.cam().dir(), detail::up);

	shadows.get_depth_map().bind(GL_TEXTURE_2D, 0);

	quad_3D_shaders.uniform_3f(&entities.cam().pos()[0], 6);
	quad_3D_shaders.uniform_mat4(&shadow_bias[0][0], 5);
	quad_3D_shaders.uniform_mat4(&projection_matrix[0][0], 0);
	quad_3D_shaders.uniform_mat4(&view_matrix[0][0], 1);

	quad_3D_shaders.uniform_3f(&color[0], 2);

	glm::mat4 translation = glm::translate( glm::vec3(0.0f, 2.f, 0.0f) );
	quad_3D_shaders.uniform_mat4(&translation[0][0], 3);
	render_model(a_cube, GL_TRIANGLES);

	glm::mat4 translation1 = glm::translate(glm::vec3(-30.0f, 2.f, 30.0f)) * glm::rotate(90.0f, glm::vec3(0, 1, 0));
	quad_3D_shaders.uniform_mat4(&translation1[0][0], 3);
	render_model(a_cube, GL_TRIANGLES);

	glm::mat4 translation2 = detail::identity_matrix;
	quad_3D_shaders.uniform_3f(&color2[0], 2);
	quad_3D_shaders.uniform_mat4(&translation2[0][0], 3);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

	render_depth();
	//render_color();

	//render_depth_gui();
}

auto application::update(void) -> void
{
	appl_window.refresh();
	entities.update(time.elapsed());
	auto & cam = entities.cam();

	f32 aspect = (f32)appl_window.pixel_width() / appl_window.pixel_height();
	shadows.update(0.1f, 100.0f, aspect, 60.0f, cam.pos(), cam.dir());

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

	glm::mat4 cube_translation = mv * glm::translate(glm::vec3(0, 2., 0));
	shaders.uniform_mat4(&cube_translation[0][0], 0);
	render_model(a_cube, GL_TRIANGLES);

	glm::mat4 second_translation = mv * glm::translate(glm::vec3(-30.0f, 2.f, 30.0f)) * glm::rotate(90.0f, glm::vec3(0, 1, 0));
	quad_3D_shaders.uniform_mat4(&second_translation[0][0], 3);
	render_model(a_cube, GL_TRIANGLES);

	glm::mat4 plat_translation = mv;
	shaders.uniform_mat4(&plat_translation[0][0], 0);
	render_model(scene_platform, GL_TRIANGLE_STRIP);

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
	auto & tex1 = shadows.get_depth_map();
	tex1.bind(GL_TEXTURE_2D, 0);
	guis.render();

	guis.prepare(gui_slot::DEBUG_1, 1);
	auto & tex2 = test_tex;
	tex2.bind(GL_TEXTURE_2D, 0);
	guis.render();

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