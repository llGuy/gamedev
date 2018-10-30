#pragma once

#include "key_frame.h"

class animation
{
private:
	f32 animation_length;

	std::vector<key_frame> key_frames;
public:
	animation(std::vector<key_frame> const & frames, f32 length);

	auto get_length(void) -> f32;

	auto operator[](u32 index) -> key_frame &;

	auto get_frame_count(void) -> u32;
};