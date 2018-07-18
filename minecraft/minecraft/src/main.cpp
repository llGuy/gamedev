// FUTURE LUC : IF THE CHUNKS ARE NOT LOADING, IT'S CAUSE THE BLOCK FSH 
// HAS A 'F' AT THE BEGINNING OF THE FILE


#include <iostream>
#include "window/window.h"
#include "engine/engine.h"

int main(int argc, char* argv[])
{
	Window window(1900, 1100, "minecraft");
	while (window.WindowOpen())
	{
		window.Draw();
		window.Update();
	}
	window.Destroy();
	return 0;
}