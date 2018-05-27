#ifndef _TEXTURE_HEADER_
#define _TEXTURE_HEADER_

#include <string>
#include <iostream>

namespace mulgame {

	class Texture
	{
	public:
		Texture(const std::string& dir);
	private:
		
	private:
		uint32_t m_textureID;
	};

}

#endif