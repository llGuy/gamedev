#ifndef _GAME_STATE_
#define _GAME_STATE_

#include "buffer.h"
#include "vao.h"

class timer;
class input_handler;

class game_state
{
public:
	game_state(void) = default;
	virtual ~game_state(void) = default;

	virtual auto render(void) -> void = 0;
	// if at some point something happens (crash, disconnected...),
	// may want to return to main menu or something
	virtual auto update(input_handler &, timer &) -> game_state * = 0;

	auto render_model(vertex_array & vao, buffer & index_buffer, uint32_t count) -> void
	{
		vao.bind();
		index_buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
		index_buffer.unbind(GL_ELEMENT_ARRAY_BUFFER);
		vao.unbind();
	}
};

#endif