#pragma once

#include <unordered_map>
#include <string>
#include "../api/api.h"
#include "al_buffer.h"

class audio_object
{

};

class audio_handler
{
private:

	std::unordered_map<std::string, u32> indices;
	std::vector<audio_object> objects;

public:

	auto load_wav(std::string const & file) -> void
	{
		al::buffer audio_buffer;
		audio_buffer.create();
	}

};