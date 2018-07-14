#ifndef _VAO_H_
#define _VAO_H_

#include <iostream>
#include <GL/glew.h>

class vertex_array
{
public:
	vertex_array(void) = default;

	auto create(void) -> uint32_t;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto unbind(void) -> void;
	auto enable(uint32_t loc) -> void;
	auto divisor(uint32_t loc, uint32_t div) -> void;

	template <typename T>
	auto push(uint32_t index, uint32_t size, uint32_t stride, void * ptr) -> void
	{
		//		static_assert(false);
	}

	/*	template<>
	auto push<float>(uint32_t index, uint32_t size, uint32_t stride, void * ptr) -> void
	{
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr);
	}*/

private:
	uint32_t id;
};

#endif
