#include <stb-master/stb_image.h>

#include "heightmap.h"

Heightmap::Heightmap(const char* hmDir)
	: m_hmDir(hmDir)
{
	ImageData();
}
void Heightmap::ComputeHeights(float* yVals, unsigned int nvw, unsigned int nvh, unsigned int mh)
{
	unsigned int imageWidthIndex;
	unsigned int imageHeightIndex;
	unsigned int imageTotalIndex;

	ComputeStride(nvw, nvh);

	for (unsigned int col = 0; col < nvh; ++col)
	{
		for (unsigned int row = 0; row < nvw; ++row)
		{
			imageWidthIndex = m_widthStride * row;
			imageHeightIndex = m_heightStride * col * (m_width * 3);
			imageTotalIndex = imageWidthIndex + imageHeightIndex;

			uint8 grayScale = m_data[imageTotalIndex];
			float height = (float)grayScale / 0b11111111 * mh;
			yVals[row + col * nvh] = height;
		}
	}
}
void Heightmap::ImageData(void)
{
	int nrChannels;
	m_data = stbi_load(m_hmDir.c_str(), &m_width, &m_height, &nrChannels, 0);
	m_imageSize = m_width * m_height;
}
void Heightmap::ComputeStride(unsigned int nvw, unsigned int nvh)
{
	m_widthStride = m_width * 3 / nvw;
	m_heightStride = m_height / nvh;
}