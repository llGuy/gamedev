#define GLM_ENABLE_EXPERMINENTAL

#include <iostream>
#include <GL/glew.h>
#include "window.h"
#include "engine.h"
#include "init.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
	std::cerr << "need to specify server of client mode : -s -c" << std::endl;
	return 0;
    }

    int8_t arg = argv[1][1];
    
    GLFWInit();
    Window window("multiplayer game", 1200, 800);
    window.Init();
    GLEWInit();

    using mulgame::MULGEngine;
    MULGEngine engine(1200, 800, arg);
    window.EForwarder(engine.EForwarder());

    while(window.Open())
    {
		// draw calls
		engine.Update();
		engine.Render();
		window.Update();
    }

    return 0;
}
