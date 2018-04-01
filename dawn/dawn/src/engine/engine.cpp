#include "engine.h"

namespace dawn
{
	DawnEngine::DawnEngine(const char* vs, const int32_t& width, const int32_t& height) noexcept
		: m_display(vs, width, height)
	{
		m_display.Init();
	}

	void DawnEngine::Run(void)
	{
		while (m_display.Open())
		{
			Render();
		}
		m_display.Destroy();
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

		m_display.Update();
	}
}