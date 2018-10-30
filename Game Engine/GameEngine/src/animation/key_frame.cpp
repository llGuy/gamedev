#include "key_frame.h"

key_frame::key_frame(f32 time_stamp)
	: time_stamp(time_stamp)
{
}

auto key_frame::get_time_stamp(void) -> f32
{
	return time_stamp;
}

auto key_frame::operator[](std::string const & name) -> joint_transform &
{
	return joint_transforms[name];
}