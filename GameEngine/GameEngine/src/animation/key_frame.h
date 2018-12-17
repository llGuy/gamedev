#pragma once

#include "joint.h"
#include <unordered_map>

class key_frame
{
private:
	f32 time_stamp;

	std::unordered_map<std::string, joint_transform> joint_transforms;
public:
	key_frame(f32 time_stamp);

	auto get_time_stamp(void) -> f32;

	auto operator[](std::string const & name) -> joint_transform &;

	auto begin(void)
	{
		return joint_transforms.begin();
	}

	auto end(void)
	{
		return joint_transforms.end();
	}
};