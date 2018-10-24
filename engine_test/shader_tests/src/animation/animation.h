#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "../utils/types.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct joint_transform
{
	glm::quat rotation;
	glm::vec3 translation;

	auto get_local_transform(void) -> glm::mat4 
	{
		glm::mat4 res = glm::translate(translation) * glm::toMat4(rotation);
		return res;
	}
};

static auto interpolate(glm::vec3 const & a, glm::vec3 const & b, f32 progression) -> glm::vec3
{
	return a + (b - a) * progression;
}

static auto interpolate(joint_transform const & a, joint_transform const & b, f32 progression) -> joint_transform
{
	glm::vec3 pos = interpolate(a.translation, b.translation, progression);

	glm::quat rot = glm::mix(a.rotation, b.rotation, progression);

	return { rot, pos };
}

struct key_frame
{
	f32 time_stamp;
	std::unordered_map<std::string, joint_transform> pose;

	key_frame(void) = default;
};

class animation
{
private:
	f32 time;
	std::vector<key_frame> key_frames;
public:
	animation(void) = default;

	animation(f32 time, std::vector<key_frame> const & frames)
		: time(time), key_frames(std::move(frames))
	{
	}

	auto get_length(void) -> f32 & { return time; }
	auto get_key_frames(void) -> std::vector<key_frame> & { return key_frames; }
};