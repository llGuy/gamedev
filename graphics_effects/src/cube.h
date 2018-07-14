#ifndef _CUBE_H_
#define _CUBE_H_

#include <glm/glm.hpp>

#include "renderable_3D.h"

enum color { red, blue, green };

template <color C>
struct color_val;

template <> struct color_val <red> { static constexpr glm::vec3 value { 1.0f, 0.0f, 0.0f }; };
template <> struct color_val <blue> { static constexpr glm::vec3 value { 0.0f, 0.0f, 1.0f }; };
template <> struct color_val <green> { static constexpr glm::vec3 value { 0.0f, 1.0f, 0.0f }; };

class cube
: public renderable_3D
{
public:
	cube(float rad, glm::vec3 const & col);

	auto create(resource_handler &) -> void override;
	auto destroy(void) -> void override;
	auto element_buffer(void) -> buffer & override;
private:
	auto create_buffers(void) -> void;
	auto create_vao(void) -> void;
private:
	buffer vertex_buffer;
	buffer index_buffer;

	float radius;
	glm::vec3 cube_color;
};

#endif
