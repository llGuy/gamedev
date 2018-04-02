#include "engine.h"

namespace dawn
{
	DawnEngine::DawnEngine(const int32_t& width, const int32_t& height) noexcept
	{
	}

	void DawnEngine::Init(void)
	{

	}

	void DawnEngine::Render(void)
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(+0.5f, -0.5f);
		glVertex2f(+0.5f, +0.5f);
		glVertex2f(-0.5f, +0.5f);
		glEnd();
	}

}