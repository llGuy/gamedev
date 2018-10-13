#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "../framebuffer.h"
#include "../shader_program.h"

struct objects
{
	std::string model_name;
	std::vector<glm::mat4> matrices;

	std::shared_ptr<program> shaders;
};

class target
{
private:
	framebuffer render_target;

	std::vector<objects> objects_list;
public:
	target(void) = default;


};