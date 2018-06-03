#ifndef _TEXTURE_HEADER_
#define _TEXTURE_HEADER_

#include <string>
#include <iostream>

namespace mulgame {

	struct ImageData
	{
		uint8_t* data;
		int32_t width;
		int32_t height;
	};

	class Texture
	{
	public:
		Texture(void) = default;
		Texture(const std::string& dir);
		void Bind(int32_t unit);
	private:
		ImageData ExtractImage(const std::string& dir);
		void SendImageData(const ImageData& data);
	private:
		uint32_t m_textureID;
	};

}

#endif