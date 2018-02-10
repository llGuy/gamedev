#ifndef HEIGHTMAP_HEADER
#define HEIGHTMAP_HEADER

#include <iostream>

class Heightmap
{
public:
	explicit Heightmap(const char* hmDir);
	void ComputeHeights(float* yVals, 
		unsigned int numVertsWidth, 
		unsigned int numVertsHeight,
		unsigned int maxHeight);
private:
	void ImageData(void);
	void ComputeStride(unsigned int numVertsWidth, unsigned int numVertsHeight);
private:
	typedef unsigned int uint32;
	typedef unsigned char uint8;

	std::string m_hmDir;
	uint8* m_data;

	int m_width;
	int m_height;
	int m_imageSize;

	unsigned int m_widthStride;
	unsigned int m_heightStride;
};

#endif