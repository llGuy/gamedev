#pragma once

#include <string>
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


class resource_handler
{
public:
	resource_handler(std::string const & default_dir)
		: default_directory(default_dir)
	{
	}

	template <resource R>
	auto load(std::string const & dir) -> typename data <R>::type;
private:
	std::string default_directory;
};