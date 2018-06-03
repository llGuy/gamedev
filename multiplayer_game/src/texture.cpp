#include "texture.h"

#include <GL/glew.h>
#include <stb_image.h>

namespace mulgame {

	Texture::Texture(const std::string& dir)
	{
		auto data = ExtractImage(dir);
		SendImageData(data);
	}

	ImageData Texture::ExtractImage(const std::string& dir)
	{
		int32_t width, height, numComp;
		uint8_t* data = stbi_load(dir.c_str(), &width, &height, &numComp, 4);
		return { data, width, height };
	}

	void Texture::Bind(int32_t unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::SendImageData(const ImageData& data)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
	}

}