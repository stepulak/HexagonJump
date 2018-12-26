#include "Utils.hpp"
#include "MusicVisualization.hpp"
#include "BeatUnitManager.hpp"
#include "Game.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace hexagon;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	Camera camera(960.f, 540.f, 800, 600);

	sf::Clock deltaClock;
	float time = 0.f;
	bool keyleft = false;
	bool keyright = false;
	
	BeatUnitManager manager(MusicVisualizationData(1, MusicVisualizationColumnData(8)), 1.0);
	Game game(camera, Game::Difficulty::EASY);

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				game.KeyPressed(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased) {
				game.KeyReleased(event.key.code);
			}
		}
		window.clear(sf::Color::Black);

		game.Update(dt);
		game.Draw(window);

		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}