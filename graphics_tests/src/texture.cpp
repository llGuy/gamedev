#include "texture.h"

auto unbind_all_textures(GLenum target) -> void
{
	glBindTexture(target, 0);
}

auto texture::create(void) -> void
{
	glGenTextures(1, &id);
}
auto texture::bind(GLenum target, uint32_t slot) -> void
{
	if (slot == -1)
	{
		glBindTexture(target, id);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(target, id);
}
auto texture::fill(GLenum target, GLenum internal_format, uint32_t w, uint32_t h, GLenum format, GLenum type, void const * data) -> void
{
	glTexImage2D(target, 0, internal_format, w, h, 0, format, type, data);
}
auto texture::float_param(GLenum target, GLenum mode, float factor) -> void
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
