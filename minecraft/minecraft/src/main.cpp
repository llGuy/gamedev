#include <iostream>
#include "window/window.h"
#include "engine/engine.h"

int main(int argc, char* argv[])
{
	Window window(3000, 1900, "minecraft");
	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	window.Destroy();
	return 0;
}