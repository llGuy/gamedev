#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <string>
#include <fstream>

#include "resource_data.h"

//typedef unsigned char stbi_uc;
//extern "C" auto stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels) -> stbi_uc *;

class resource_handler
{
public:
	resource_handler(std::string const & default_dir)
		: default_directory(default_dir)
	{
	}

	template <resource R>
	auto load(std::string const & dir) -> typename data <R>::type;
	/*
	template <>
	auto load <image>(std::string const & dir) -> typename data <image>::type
	{
		int32_t w, h, num_comp;
		auto * data = stbi_load((default_directory + dir).c_str(), &w, &h, &num_comp, 4);
		return { data, w, h };
	}*/
private:
	std::string default_directory;
};

#endif
