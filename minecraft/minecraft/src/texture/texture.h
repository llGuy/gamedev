#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>
#include <GL/glew.h>

typedef unsigned int tex_id;

class Texture
{	
public:
	explicit Texture(void) = default;
	explicit Texture(const std::string& file);
	void Init(void);
	void Bind(int unit);
private:
	struct ImageData
	{
		unsigned char* data;
		unsigned int w;
		unsigned int h;
	};
	ImageData LoadImageData(const std::string& file);
	void SendData(ImageData id);
private:
	tex_id m_texture;
	std::string m_file;
};

#endif