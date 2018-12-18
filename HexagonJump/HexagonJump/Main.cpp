#include "Utils.hpp"
#include "Camera.hpp"
#include "MusicVisualization.hpp"
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

	sf::SoundBuffer buffer;
	buffer.loadFromFile("C:\\Users\\stepan\\Documents\\HexagonJump\\HexagonJump\\x64\\Debug\\DontBeSoShy.ogg");
	std::cout << buffer.getDuration().asSeconds() << std::endl;
	std::cout << buffer.getSampleCount() << std::endl;
	std::cout << buffer.getSampleRate() << std::endl;

	auto start = std::chrono::system_clock::now();
	auto data = CountMusicVisualizationData(buffer, 1 / 20.f, 8);
	auto stop = std::chrono::system_clock::now();
	std::cout << "done" << std::endl;
	std::cout << "time elapsed " << (std::chrono::duration<double>(stop - start)).count() << std::endl;

	while (window.isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
				}
				if (event.key.code == sf::Keyboard::C) {
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


		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}