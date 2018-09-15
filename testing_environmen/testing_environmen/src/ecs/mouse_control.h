#pragma once

#include "ecs.h"
#include "../detail.h"
#include "../camera.h"
#include "../input_handler.h"

template <> class component <struct mouse_control> : public icomponent
{
private: 
	camera * cam;
	input_handler * mouse;
	glm::vec2 previous_cursor;
public:
	component(entity &, i32 entity_index, input_handler & ih, camera & cam) 
		: mouse(&ih), previous_cursor(-1.0f), cam(&cam), icomponent::icomponent(entity_index)
	{
	}
	component(void) = default;
	auto operator=(component &)->component & = default;
	auto update(f32 td, vec_dd<entity> & entities, entity_cs & ecs) -> void override
	{
		entity_data & ent = entities[entity_index].get_data();

		detail::debug(mouse->got_mouse_button(GLFW_MOUSE_BUTTON_3));

		if (glm::all(glm::lessThan(glm::abs(previous_cursor - glm::vec2(-1.0f)), glm::vec2(0.0000001f))))
			previous_cursor = mouse->cursor_position();
		else if (mouse->cursor_moved())
		{
			glm::vec2 difference = mouse->cursor_position() - previous_cursor;
			previous_cursor = mouse->cursor_position();
			ent.dir = look_fps(difference, ent.dir, td);
			auto[pitch, yaw] = pitch_yaw({ cam->get_pitch(), cam->get_yaw() }, difference);
			cam->get_pitch() = pitch;
			cam->get_yaw() = yaw;
		}
	}
};