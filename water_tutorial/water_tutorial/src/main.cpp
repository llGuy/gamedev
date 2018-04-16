#include <iostream>
#include "window/window.h"

int main(void)
{
	Window window(3000, 2100, "water tutorial");

	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	window.Destroy();
}