#include "Utils.hpp"
#include "MusicVisualizationManager.hpp"
#include "BeatUnitManager.hpp"
#include "Game.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace hexagon;

static constexpr auto MUSIC_FILENAME = "D:\\Git\\HexagonJump\\HexagonJump\\x64\\Debug\\DontBeSoShy.ogg";
static constexpr size_t NUM_COLUMNS = 8u;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Default, settings);

	Camera camera(960.f, 540.f, 800, 600);
	
	sf::SoundBuffer buffer;
	buffer.loadFromFile(MUSIC_FILENAME);
	
	sf::Music music;
	music.openFromFile(MUSIC_FILENAME);

	MusicVisulizationManager manager(NUM_COLUMNS);
	for (const auto& [f, s] : manager.GetMusicList()) {
		std::cout << f << std::endl;
	}

	Game game(music, manager.LoadMusic("DontBeSoShy").visulization, NUM_COLUMNS, camera);

	sf::Clock deltaClock;
	game.Start();

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