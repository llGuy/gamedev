#define GLM_ENABLE_EXPERMINENTAL

#include <iostream>
#include <GL/glew.h>
#include "window.h"
#include "engine.h"
#include "init.h"

int main(int argc, char* argv[])
{
    GLFWInit();
    Window window("multiplayer game", 2800, 1500);
    window.Init();
    GLEWInit();

    using mulgame::MULGEngine;
    MULGEngine engine(2800, 1500);
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
