#pragma once

#include <al.h>
#include "../utils/types.h"

namespace al {

	class buffer
	{
	private:

		u32 id;

	public:

		buffer(void) = default;

		auto create(void) -> void
		{
			alGenBuffers(1, &id);
		}

		template <typename T> auto fill(ALenum format, T * data, u32 size, u32 frequency) -> void
		{
			alBufferData(id, format, data, size, frequency);
		}

	};

}