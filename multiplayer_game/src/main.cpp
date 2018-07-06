#define GLM_ENABLE_EXPERMINENTAL

#include <iostream>
#include <GL/glew.h>
#include "window.h"
#include "engine.h"
#include "init.h"

int main(int argc, char* argv[])
{/*
    GLFWInit();
    Window window("multiplayer game", 1200, 800);
    window.Init();
    GLEWInit();
	*/
   /* using mulgame::MULGEngine;
    MULGEngine engine(1200, 800, 0);
    //window.EForwarder(engine.EForwarder());

    while(window.Open())
    {
		// draw calls
		engine.Update();
		engine.Render();
		window.Update();
    }*/

	mulgame::MULGEngine engine(2500, 800, 0);
	while (engine.Running())
	{
		engine.Update();
		engine.Render();
	}

    return 0;
}
