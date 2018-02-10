#include <iostream>
#include "window/window.h"
#include <GL/glew.h>
#include <chrono>
#include <array>
#include <vector>

int main(int argc, char* argv[])
{
	Window window(1920, 1080, "robo");
	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	return 0;
}

// 3673 lines of code !!!