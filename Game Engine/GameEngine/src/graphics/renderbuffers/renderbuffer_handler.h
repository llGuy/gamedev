#pragma once

#include <vector>
#include <unordered_map>
#include "../../api/api.h"

class renderbuffer_handler
{
public:
	renderbuffer_handler(void) = default;

	auto add_renderbuffer(std::string const & name) -> renderbuffer *;

	auto get_renderbuffer_id(std::string const & name) -> u32;

	auto operator[](std::string const & name) -> renderbuffer *;
	auto operator[](u32 id) -> renderbuffer * ;
private:
	std::vector<renderbuffer *> renderbuffers;

	std::unordered_map<std::string, u32> renderbuffer_indices;
};