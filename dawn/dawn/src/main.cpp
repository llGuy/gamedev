#include <iostream>
#include <chrono>
#include "utils/memory/memory_manager.h"
#include "utils/vecout/vecout.h"
#include "engine/engine.h"

int main(int argc, char* argv[])
{
	dawn::DawnEngine engine("0.1", 1400, 900);
	engine.Run();

	return 0;
}