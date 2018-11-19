#include "BeatUnitManager.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace hexagon;

int main()
{
	BeatUnitManager m(1);
	auto& unit = m.GetUnit(0);

	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	
	sf::Clock deltaClock;
	float time = 0.f;
	// run the program as long as the window is open
	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);
		auto triangle = utils::CountTriangleCoords(100, unit.Height() * 100, utils::Direction::UP);
		triangle.setFillColor(sf::Color(255, 255, 255));
		triangle.setPosition(sf::Vector2f(100, 100));
		window.draw(triangle);

		time += dt;
		std::cout << time << std::endl;
		if (time > 2.f) {
			std::cout << "NOW" << std::endl;
			time = 0.f;
			unit.SetHeight(1.0);
		}
		m.UpdateUnits(dt);

		// end the current frame
		window.display();
	}

	return 0;
}