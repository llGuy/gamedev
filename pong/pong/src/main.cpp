#include "window\window.h"
#include "misc.h"

#include <iostream>

int main(int argc, char* argv[])
{
	Log("welcome to Pong!!! Press enter to play : ");
	std::cin.get();

	Window window(2000, 1500, "pong");

	while (window.WindowIsOpen())
	{
		window.Draw();
		window.Update();
	}

	return 0;
}