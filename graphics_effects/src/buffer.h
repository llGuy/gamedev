#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <GL/glew.h>
#include <iostream>	

class buffer
{
public:
	buffer(void) = default;
	
	auto create(void) -> uint32_t;
	auto destroy(void) -> void;
	auto bind(GLenum binding_point) -> void;
	auto unbind(GLenum binding_point) -> void;

	template <typename T>
	auto fill(uint32_t size, T * data, GLenum usage, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferData(binding_point, size, data, usage);
	}

	template <typename T>
	auto partial_fill(uint32_t offset, uint32_t size, T * data, GLenum binding_point) -> void
	{
		bind(binding_point);
		glBufferSubData(binding_point, offset, size, data);
	}
private:
	uint32_t id;
};

#endif