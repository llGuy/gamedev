#include "animation.h"

animation::animation(std::vector<key_frame> const & frames, f32 length)
	: key_frames(frames), animation_length(length)
{
}

auto animation::get_length(void)->f32
{
	return animation_length;
}

auto animation::operator[](u32 index)->key_frame &
{
	return key_frames[index];
}

auto animation::get_frame_count(void) -> u32
{
	return key_frames.size();
}