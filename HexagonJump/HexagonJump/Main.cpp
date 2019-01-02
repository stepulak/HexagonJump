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
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	Camera camera(960.f, 540.f, 800, 600);
	
	sf::SoundBuffer buffer;
	buffer.loadFromFile(MUSIC_FILENAME);
	
	sf::Music music;
	music.openFromFile(MUSIC_FILENAME);

	std::cout << "Loading started" << std::endl;
	MusicVisulizationManager manager;
	//auto musicData = CountMusicVisualizationData(buffer, Game::TIMERATE, NUM_COLUMNS);
	std::cout << "Loading ended" << std::endl;

	/*std::fstream file("save.txt", std::ios_base::out);
	file << musicData.second << std::endl;
	for (auto& column : musicData.first) {
		for (auto& val : column) {
			file << val << ',';
		}
		file << std::endl;
	}
	file.close();*/

	Game game(music, MusicVisualization{}, NUM_COLUMNS, camera, Game::Difficulty::EASY);

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