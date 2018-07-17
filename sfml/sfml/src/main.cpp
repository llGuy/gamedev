#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "grid.h"
#include "application.h"

auto main(int32_t argc, char * argv[]) -> int32_t
{
	/*sf::RenderWindow window(sf::VideoMode(1500, 900), "window");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			window.clear();
			window.display();
		}
	}*/
	application simulation(1500, 900);
	simulation.init();

	while (simulation.running())
	{
		simulation.update();
	}

	std::cin.get();

	return 0;
}

