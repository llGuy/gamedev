#include <iostream>
#include "window/window.h"

int main(void)
{
	Window window(1200, 700, "water tutorial");

	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	window.Destroy();
}