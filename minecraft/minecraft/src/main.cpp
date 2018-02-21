#include <iostream>
#include "window/window.h"
#include "engine/engine.h"

int main(int argc, char* argv[])
{
	Window window(1700, 1000, "minecraft");
	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	window.Destroy();
	return 0;
}