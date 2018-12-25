#include "Utils.hpp"
#include "Camera.hpp"
#include "MusicVisualization.hpp"
#include "BeatUnitManager.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

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
	Player player(120, 150, 30);
	World world(camera);
	world.AddObstacle(std::make_unique<Platform>(0, 500, 1440, 10));

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					player.TryToJump();
				}
				if (event.key.code == sf::Keyboard::C) {
					player.TryToFallDownFast();
				}
				if (event.key.code == sf::Keyboard::A) {
					keyleft = true;
					player.StartMoving(500, false);
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = true;
					player.StartMoving(500, true);
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					keyleft = false;
					player.StopMoving();
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = false;
					player.StopMoving();
				}
			}
		}
		window.clear(sf::Color::Black);

		camera.Move(7.f);

		player.Update(dt, 9.81f * 100, world);
		player.Draw(window, camera);

		world.Draw(window);

		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}