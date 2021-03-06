#include "water.h"

auto water::create(resource_handler & rh) -> void
{
	water_quad.create(rh);

	water_shaders.create_shader(GL_VERTEX_SHADER, "water_vsh.shader");
	water_shaders.create_shader(GL_FRAGMENT_SHADER, "water_fsh.shader");
	water_shaders.link_shaders("vertex_position");
	water_shaders.get_uniform_locations("projection_matrix", "view_matrix", "camera_position", "move_factor",
		"reflection_texture", "refraction_texture", "dudv_texture", "normal_map", "depth_texture");
	water_shaders.use();
	water_shaders.uniform_1i(0, 4);
	water_shaders.uniform_1i(1, 5);
	water_shaders.uniform_1i(2, 6);
	water_shaders.uniform_1i(3, 7);
	water_shaders.uniform_1i(4, 8);

	create_reflection_fbo(reflection_width, reflection_height);
	create_refraction_fbo(refraction_width, refraction_height);

#ifdef _win32
	std::string path = "src/";
#else
	std::string path = "";
#endif

	auto img_dudv = rh.load<image>(path + "waterDUDV.png");
	dudv_texture.create();
	dudv_texture.bind(GL_TEXTURE_2D);
	dudv_texture.fill(GL_TEXTURE_2D, GL_RGBA, img_dudv.w, img_dudv.h, GL_RGBA, GL_UNSIGNED_BYTE, img_dudv.data);
   
	dudv_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	dudv_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	dudv_texture.float_param(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);

	auto img_norm = rh.load<image>(path + "normal.png");
	normal_map_texture.create();
	normal_map_texture.bind(GL_TEXTURE_2D);
	normal_map_texture.fill(GL_TEXTURE_2D, GL_RGBA, img_norm.w, img_norm.h, GL_RGBA, GL_UNSIGNED_BYTE, img_norm.data);
	normal_map_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	normal_map_texture.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	normal_map_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	normal_map_texture.float_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

auto water::bind_framebuffer(framebuffer & fbo, int32_t w, int32_t h) -> void
{
	unbind_all_textures(GL_TEXTURE_2D);
	fbo.bind();
	glViewport(0, 0, w, h);
}

auto water::prepare(glm::mat4 & proj, glm::mat4 & view, glm::vec3 & camera_pos, float elapsed) -> void
{
	rand();

	static float move_factor = 0;
	move_factor += 0.02f * elapsed;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	water_shaders.use();
	water_shaders.uniform_mat4(&proj[0][0], 0);
	water_shaders.uniform_mat4(&view[0][0], 1);
	water_shaders.uniform_3f(&camera_pos[0], 2);
	water_shaders.uniform_1f(move_factor, 3);
	refl_color.bind(GL_TEXTURE_2D, 0);
	refr_color.bind(GL_TEXTURE_2D, 1);
	dudv_texture.bind(GL_TEXTURE_2D, 2);
	normal_map_texture.bind(GL_TEXTURE_2D, 3);
	refr_depth.bind(GL_TEXTURE_2D, 4);
}

auto water::quad(void) -> quad_3D &
{
	return water_quad;
}

auto water::create_reflection_fbo(int32_t w, int32_t h) -> void
{
	water_reflection_fbo.create();
	water_reflection_fbo.bind();

	create_color_texture_attachment(refl_color, water_reflection_fbo, w, h);
	create_depth_buffer_attachment(refl_depth, water_reflection_fbo, w, h);
}

auto water::create_refraction_fbo(int32_t w, int32_t h) -> void
{
	water_refraction_fbo.create();
	water_refraction_fbo.bind();

	create_color_texture_attachment(refr_color, water_refraction_fbo, w, h);
	create_depth_texture_attachment(refr_depth, water_refraction_fbo, w, h);
}

auto water::create_color_texture_attachment(texture & t, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	t.create();
	t.bind(GL_TEXTURE_2D);
	t.fill(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	fbo.attach(t, GL_COLOR_ATTACHMENT0, 0);
}

// needs to pass a depth_texture type
auto water::create_depth_texture_attachment(depth_texture & t, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	t.set_dimensions(w, h);
	t.create();
	fbo.attach(t, GL_DEPTH_ATTACHMENT, 0);
}

auto water::create_depth_buffer_attachment(renderbuffer & r, framebuffer & fbo, int32_t w, int32_t h) -> void
{
	r.create();
	r.bind();
	r.set_storage(GL_DEPTH_COMPONENT, w, h);
	fbo.attach(r, GL_DEPTH_ATTACHMENT);
}

auto water::bind_refl(void) -> void
{
	bind_framebuffer(water_reflection_fbo, reflection_width, reflection_height);
}

auto water::bind_refr(void) -> void
{
	bind_framebuffer(water_refraction_fbo, refraction_width, refraction_height);
}

auto water::unbind_framebuffers(int32_t w, int32_t h) -> void
{
	unbind_all_framebuffers();
	glViewport(0, 0, w, h);
}

auto water::refl_texture(void) -> texture &
{
	return refl_color;
}

auto water::refr_texture(void) -> texture &
{
	return refr_color;
}

auto water::refr_depth_texture(void) -> texture &
{
	return refr_depth;
}