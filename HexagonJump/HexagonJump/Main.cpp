#include "Utils.hpp"
#include "Camera.hpp"
#include "MusicVisualization.hpp"
#include "BeatUnitManager.hpp"
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

	auto path = "D:\\Git\\HexagonJump\\HexagonJump\\x64\\Debug\\DontBeSoShy.ogg";

	sf::SoundBuffer buffer;
	buffer.loadFromFile(path);
	std::cout << buffer.getDuration().asSeconds() << std::endl;
	std::cout << buffer.getSampleCount() << std::endl;
	std::cout << buffer.getSampleRate() << std::endl;

	auto start = std::chrono::system_clock::now();
	auto data = CountMusicVisualizationData(buffer, 1 / 20.f, 8);
	auto stop = std::chrono::system_clock::now();
	std::cout << "done" << std::endl;
	std::cout << "time elapsed " << (std::chrono::duration<double>(stop - start)).count() << std::endl;

	BeatUnitManager manager(data.first, data.second);
	
	sf::Music music;
	music.openFromFile(path);
	
	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					manager.Start();
					auto delay = 0.3;
					for (double tick = 0; tick < delay; tick += 1 / 20.f) manager.Update(tick, 1 / 20.f);
					std::this_thread::sleep_for(std::chrono::milliseconds((long)delay * 1000));
					music.play();
					dt = 1 / 60.0;
				}
				if (event.key.code == sf::Keyboard::C) {
					music.stop();
					manager.Stop();
				}
				if (event.key.code == sf::Keyboard::A) {
					keyleft = true;
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = true;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					keyleft = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					keyright = false;
				}
			}
		}
		window.clear(sf::Color::Black);
		manager.Update(dt, 1 / 20.f);
		
		for (size_t i = 0u; i < manager.NumberOfBeatUnits(); i++) {
			auto height = manager.GetUnit(i).Height();
			auto shape = CountTriangleCoords(50, height * 100, Direction::DOWN);
			shape.setPosition(sf::Vector2f(i * 50 + 25, 50));
			window.draw(shape);
		}
		
		
		double time = (double)music.getPlayingOffset().asMilliseconds() / music.getDuration().asMilliseconds();
		size_t pos = (size_t)(time * data.first.size());

		std::cout << "Data index is: " << manager.VisualizationDataIndex() << " and should be: " << pos << std::endl;

		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}