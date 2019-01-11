#include "AppInfo.hpp"
#include "BeatUnitManager.hpp"
#include "GuiManager.hpp"
#include "MusicVisualizationManager.hpp"
#include "MainMenu.hpp"
#include "Utils.hpp"
#include "OpenFileDialog.hpp"

#include <thread>
#include <chrono>

using namespace hexagon;
using namespace hexagon::gui;

static constexpr auto FONT_FILENAME = "font\\font.ttf";

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	auto title = APP_NAME + std::string(" (c) ") + DEVELOPER_NAME + " " + RELEASE_YEAR;

	Camera camera(960.f, 540.f, 800, 600);
	sf::RenderWindow window(sf::VideoMode(camera.GetVirtualWidth(), camera.GetVirtualHeight()), title, sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::Font font;
	font.loadFromFile(FONT_FILENAME);

	MainMenu menu(font, camera);

	sf::Clock deltaClock;

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
				menu.KeyPressed(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased) {
				menu.KeyReleased(event.key.code);
			}
		}
		window.clear(sf::Color::Black);

		if (menu.WantToQuit()) {
			return 0;
		}

		menu.Update(dt);
		menu.Draw(window);
		
		window.setView(camera.GetVirtualView());
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}