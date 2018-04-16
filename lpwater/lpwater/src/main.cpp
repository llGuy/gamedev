#include <iostream>
#include "window.h"
#include "engine.h"

int main(int argc, char* argv[])
{
	Window window(1700, 1200, "water");

	Engine engine(1700, 1200);
	window.m_camera = engine.Cam();
	while (window.Open())
	{
		engine.Render();
		window.Update();
	}
}