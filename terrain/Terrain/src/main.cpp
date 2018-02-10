#include <iostream>

#include "window/window.h"
#include "terrain/terrain.h"

int main(int argc, char* argv[])
{
	Window window(1920, 1080, "terrain");
	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}

	return 0;
}