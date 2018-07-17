#ifndef _RESOURCE_DATA_H_
#define _RESOURCE_DATA_H_

#include <fstream>

enum resource
{
	file, image
};

template <resource R>
struct data;

template <>
struct data <file>
{
	using type = std::fstream;
};

template <>
struct data <image>
{
	using type = struct { void * data; int32_t w; int32_t h; };
};

#endif