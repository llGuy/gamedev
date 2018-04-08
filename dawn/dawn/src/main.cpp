#include <iostream>
#include <chrono>
#include "utils/memory/memory_manager.h"
#include "utils/vecout/vecout.h"
#include "engine/engine.h"
#include "engine/window/window.h"

int main(int argc, char* argv[])
{
	dawn::Window display("version 0.1", 2560, 1440);
	dawn::DawnEngine engine(display.Width(), display.Height());
	display.Init(&engine);
	engine.Init(display.CursorPosition());

	while (display.Open())
	{
		engine.Render();
		display.Update();
	}

	display.Destroy();

	return 0;
}