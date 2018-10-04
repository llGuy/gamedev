#pragma once

#include "buffer.h"
#include "vao.h"

#include "shader_program.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct tracer
{
	glm::vec3 p1;
	glm::vec3 color1;

	glm::vec3 p2;
	glm::vec3 color2;
};

class tracer_handler
{
private:
	std::vector<tracer> traces;

	vertex_layout layout;
	buffer gpu_traces_buffer;
	program shaders;
public:
	auto create(void) -> void
	{
		shaders.attach(shader(GL_VERTEX_SHADER, "lines/line_vsh.shader"));
		shaders.attach(shader(GL_FRAGMENT_SHADER, "lines/line_fsh.shader"));
		shaders.link("vertex_position", "vertex_color");

		gpu_traces_buffer.create();
		gpu_traces_buffer.bind(GL_ARRAY_BUFFER);
		gpu_traces_buffer.fill<void>(sizeof(tracer) * 10, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);

		layout.create();
		layout.bind();

		attribute vertex{ GL_FLOAT, 3, GL_FALSE, 2 * sizeof(glm::vec3), nullptr };
		attribute color{ GL_FLOAT, 3, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)) };

		layout.attach(gpu_traces_buffer, vertex, color);

		unbind_vertex_layouts();
		unbind_buffers(GL_ARRAY_BUFFER);
	}
	auto render(glm::mat4 & projection, glm::mat4 & view) -> void
	{
		if (traces.size() > 0)
		{
			gpu_traces_buffer.partial_fill(0, traces.size() * sizeof(tracer), traces.data(), GL_ARRAY_BUFFER);

			shaders.bind();
			shaders.send_uniform_mat4("projection", glm::value_ptr(projection), 1);
			shaders.send_uniform_mat4("view", glm::value_ptr(view), 1);

			layout.bind();
			glDrawArrays(GL_LINES, 0, traces.size() * 2);
			unbind_vertex_layouts();
		}
	}
	auto push(glm::vec3 const & color, glm::vec3 const & p1, glm::vec3 const & p2) -> void
	{
		traces.push_back(tracer{ p1, color, p2, color });
	}
	auto clear(void) -> void
	{
		traces.clear();
	}
};