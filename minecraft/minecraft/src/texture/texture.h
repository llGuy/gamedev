#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>
#include <GL/glew.h>

typedef uint32_t tex_id;

class Texture
{	
public:
	explicit Texture(void) = default;
	explicit Texture(const std::string& file);
	void Init(void);
	void Bind(int unit);

	Texture& operator=(const Texture& t);
private:
	struct ImageData
	{
		uint8_t* data;
		uint32_t w;
		uint32_t h;
	};
	ImageData LoadImageData(const std::string& file);
	void SendData(ImageData id);
private:
	tex_id m_texture;
	std::string m_file;
};

#endif