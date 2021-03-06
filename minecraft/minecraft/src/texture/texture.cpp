#include "texture.h"

#include <stb-master/stb_image.h>

Texture::Texture(const std::string& file)
	: m_file(file)
{
}
void Texture::Init(void)
{
	// all image loading
	ImageData id = LoadImageData(m_file);
	SendData(id);
}
void Texture::Bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
Texture::ImageData Texture::LoadImageData(const std::string& file)
{
	int w, h, numComp;
	uint8_t* data = stbi_load(file.c_str(), &w, &h, &numComp, 4);
	return { data, static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
}
void Texture::SendData(ImageData id)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, id.w, id.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, id.data);
}
Texture& Texture::operator=(const Texture& t)
{
	m_texture = t.m_texture;
	m_file = t.m_file;

	return *this;
}