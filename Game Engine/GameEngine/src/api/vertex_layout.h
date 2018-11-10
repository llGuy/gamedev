#pragma once

#include <optional>
#include <GL/glew.h>
#include "buffer.h"

extern auto unbind_vertex_layouts(void) -> void;

struct attribute
{
	attribute(void) = default;
	attribute(u32 n, u32 type, GLenum size,
		GLenum normalized, u32 stride, void * ptr, std::optional<i32> d = {})
		: number(n), type(type), size(size), normalized(normalized), stride(stride), pointer(ptr), divisor(d)
	{
	}

	u32 number;
	u32 type;
	GLenum size;
	GLenum normalized;
	u32 stride;
	void * pointer;
	std::optional<i32> divisor;
};

class vertex_layout
{
public:
	vertex_layout(void) = default;

	auto create(void)->u32;
	auto destroy(void) -> void;
	auto bind(void) -> void;
	auto in_plane_attrib(buffer & buff, u32 i, attribute att) -> void;

	template <typename ... T> auto attach(buffer & buff, T ... attribs) -> void
	{
		buff.bind(GL_ARRAY_BUFFER);

		int8_t imps[]{ (add_attrib(attribs), static_cast<int8_t>(0)) ... };
	}

	auto counter(void)->u32;

	auto add_attrib(attribute a) -> void
	{
		glEnableVertexAttribArray(a.number);
		if (a.divisor.has_value())
			glVertexAttribDivisor(a.number, a.divisor.value());
		if (a.type == GL_INT)
		{
			glVertexAttribIPointer(a.number, a.size, a.type, a.stride, a.pointer);
		}
		else
		{
			glVertexAttribPointer(a.number, a.size, a.type, a.normalized, a.stride, a.pointer);
		}
	};
private:
	u32 count;
	u32 id;
};
