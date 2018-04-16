#define GL_STATIC

#include <iostream>
#include "glfwwindow.h"

int main(int argc, char* argv[])
{
	Window window(2000, 1400, "Graphics Pad");
	while(window.IsWindowOpen())
	{
		window.Draw();
		window.Update();
	}

	//std::cin.get();

	return 0;
}