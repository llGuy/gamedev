#ifndef _GUI_H_
#define _GUI_H_

#include <utility>
#include <GL/glew.h>
#include <glm/glm.hpp>	

#include "vao.h"
#include "buffer.h"
#include "program.h"

class quad_2D
{
public:
	quad_2D(glm::vec2 const &, glm::vec2 const &);

	auto create(void) -> void;
	auto vao(void) -> vertex_array &;
	auto quad_buffer(void) -> buffer &;
private:
	vertex_array buffer_layout;
	buffer vertex_buffer;

	glm::vec2 corner_min;
	glm::vec2 max_offset;
};

class gui_handler
{
public:
	gui_handler(void);

	auto create(void) -> void;
	auto push(glm::vec2 const & translation, float scale) -> void;

	auto shaders(void) -> program &;
	auto quad(void) -> quad_2D &;
	auto prepare_render(uint32_t = 0) -> void;
private:
	quad_2D gui_quad;
	program gui_program;
	
	glm::mat4 projection_matrix;
	std::vector<glm::mat4> quads;
};

#endif