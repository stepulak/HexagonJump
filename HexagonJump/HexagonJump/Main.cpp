#include "Utils.hpp"
#include "MusicVisualizationManager.hpp"
#include "BeatUnitManager.hpp"
#include "Game.hpp"
#include "GuiManager.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <SFML/Audio.hpp>

using namespace hexagon;
using namespace hexagon::gui;

static constexpr auto MUSIC_FILENAME = "D:\\Git\\HexagonJump\\HexagonJump\\x64\\Debug\\DontBeSoShy.ogg";
static constexpr auto FONT_FILENAME = "font\\font.ttf";
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

	//Game game(music, manager.LoadMusic("DontBeSoShy").visulization, NUM_COLUMNS, camera);

	sf::Clock deltaClock;
	//game.Start();

	GuiManager gui(FONT_FILENAME);
	/*const sf::Vector2f& position,
		float fontSize,
		size_t numElementsScroll,
		const Callback& pressCallback*/
	gui.AddGuiElement(std::make_unique<BlinkingLabel>("Hello2.5", sf::Color{ 255, 255, 255 }, sf::Vector2f{ 100, 200 }, 50));
	gui.AddGuiElement(std::make_unique<Button>("Hello1", sf::Vector2f{ 100, 100 }, 50, [] { std::cout << "clicked1" << std::endl; }));
	gui.AddGuiElement(std::make_unique<Button>("Hello2", sf::Vector2f{ 100, 150 }, 50, [] { std::cout << "clicked2" << std::endl; }));
	gui.AddGuiElement(std::make_unique<Button>("Hello3", sf::Vector2f{ 100, 250 }, 50, [] { std::cout << "clicked3" << std::endl; }));
	auto listbox = std::make_unique<ListBox>(sf::Vector2f{ 400, 100 }, 30, 5, [](const auto& string) { std::cout << string << std::endl; });
	listbox->AddElement("option1");
	listbox->AddElement("option2");
	listbox->AddElement("option3");
	listbox->AddElement("option4");
	listbox->AddElement("option5");
	listbox->AddElement("option6");
	listbox->AddElement("option7");
	gui.AddGuiElement(std::move(listbox));

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
				//game.KeyPressed(event.key.code);
				gui.KeyPressed(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased) {
				//game.KeyReleased(event.key.code);
			}
		}
		window.clear(sf::Color::Black);

		//game.Update(dt);
		//game.Draw(window);
		gui.Update(dt);
		gui.Draw(window);
		
		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}