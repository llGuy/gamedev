#pragma once

#include <al.h>
#include <alc.h>
#include <string>
#include "../types.h"

namespace audio {

	class audio_handler
	{
	public:
		auto create_buffer(std::string const & file) -> u32
		{
			u32 new_buffer;
			alGenBuffers(1, &new_buffer);

		}
	};

}