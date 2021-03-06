#include "texture.h"

auto unbind_all_textures(GLenum target) -> void
{
	glBindTexture(target, 0);
}

auto create_color_texture(texture & t, int32_t w, int32_t h, void * data, GLenum magmin_filter, GLenum internal_type) -> void
{
	if (t.get_id() > 0xffffffff)
		t.create();
	t.bind(GL_TEXTURE_2D);
	t.fill(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, internal_type, data, w, h);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magmin_filter);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magmin_filter);
	//t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto create_color_texture_info(texture & t, int32_t w, int32_t h, void * data, GLenum magmin_filter) -> void
{
	t.create();
	t.bind(GL_TEXTURE_2D);
	t.fill(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT, data, w, h);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magmin_filter);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magmin_filter);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto create_depth_texture(texture & t, int32_t w, int32_t h) -> void
{
	t.create();
	t.bind(GL_TEXTURE_2D);
	t.fill(GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, w, h);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	t.int_param(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

auto texture::create(void) -> void
{
	glGenTextures(1, &id);
}

auto texture::bind(GLenum target, u32 slot) -> void
{
	u32 number = slot;

	if (slot == -1)
	{
		glBindTexture(target, id);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + number);
	glBindTexture(target, id);
}

auto texture::fill(GLenum target, GLenum internal_format, GLenum format, GLenum type, void const * data, int32_t w, int32_t h) -> void
{
	glTexImage2D(target, 0, internal_format, w, h, 0, format, type, data);

	this->w = w;
	this->h = h;
}
auto texture::float_param(GLenum target, GLenum mode, f32 factor) -> void
{
	glTexParameterf(target, mode, factor);
}
auto texture::int_param(GLenum target, GLenum mode, GLenum factor) -> void
{
	glTexParameteri(target, mode, factor);
}
auto texture::enable_mipmap(GLenum target) -> void
{
	glGenerateMipmap(target);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(target, GL_TEXTURE_LOD_BIAS, -1);
}

auto texture::clean_up(void) -> void
{
	glDeleteTextures(1, &id);
}

auto texture::get_width(void)->i32
{
	return w;
}

auto texture::get_height(void)->i32
{
	return h;
}

auto texture::get_id(void)->u32
{
	return id;
}