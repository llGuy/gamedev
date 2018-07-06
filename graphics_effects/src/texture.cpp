#include "texture.h"

auto texture::create(void) -> void
{
	glGenTextures(1, &obj);
}
auto texture::bind(uint32_t slot) -> void
{
	if (slot == -1)
	{
		glBindTexture(GL_TEXTURE_2D, obj);
		return;
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, obj);
}
auto texture::fill(GLenum internal_format, uint32_t w, uint32_t h, GLenum format, GLenum type, void const * data) -> void
{
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, type, data);
}
auto texture::float_param(GLenum mode, GLenum factor) -> void
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
}