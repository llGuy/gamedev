#include "texture.h"

auto unbind_all_textures(void) -> void
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

auto texture::create(void) -> void
{
	glGenTextures(1, &id);
}
auto texture::bind(uint32_t slot) -> void
{
	if (slot == -1)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}
auto texture::fill(GLenum internal_format, uint32_t w, uint32_t h, GLenum format, GLenum type, void const * data) -> void
{
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, type, data);
}
auto texture::float_param(GLenum mode, float factor) -> void
{
	glTexParameterf(GL_TEXTURE_2D, mode, factor);
}
auto texture::int_param(GLenum mode, GLenum factor) -> void
{
	glTexParameteri(GL_TEXTURE_2D, mode, factor);
}
auto texture::enable_mipmap(void) -> void
{
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
}
