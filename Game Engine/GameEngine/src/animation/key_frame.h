#pragma once

#include "joint.h"
#include <unordered_map>

class key_frame
{
private:
	f32 time_stamp;

	std::unordered_map<std::string, joint_transform> joint_transforms;
public:
	key_frame(f32 time_stamp)
		: time_stamp(time_stamp)
	{
	}

	auto get_time_stamp(void) -> f32
	{
		return time_stamp;
	}

	auto operator[](std::string const & name) -> joint_transform &
	{
		return joint_transforms[name];
	}
};