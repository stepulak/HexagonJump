#include "MainMenu.hpp"

#include <chrono>
#include <thread>

using namespace hexagon;
using namespace hexagon::gui;

static constexpr float STABLE_FPS = 60.f;
static constexpr auto FONT_FILENAME = "font\\font.ttf";
static constexpr auto CAMERA_VIRT_WIDTH = 960.f;
static constexpr auto CAMERA_VIRT_HEIGHT = 540.f;
static constexpr auto CAMERA_REAL_WIDTH = 1200.f;
static constexpr auto CAMERA_REAL_HEIGHT = 678.f;

static std::unique_ptr<sf::Font> LoadFont() {
	auto font = std::make_unique<sf::Font>();
	if (!font->loadFromFile(FONT_FILENAME)) {
		throw std::runtime_error("Font not found");
	}
	return font;
}

static std::unique_ptr<sf::RenderWindow> CreateWindow(const Camera& camera) {
	static const auto title = APP_NAME + std::string(" (c) ") + DEVELOPER_NAME + " " + RELEASE_YEAR;

	sf::VideoMode videoMode(static_cast<unsigned>(camera.GetVirtualWidth()),
		static_cast<unsigned>(camera.GetVirtualHeight()));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	auto window = std::make_unique<sf::RenderWindow>(videoMode, title, sf::Style::Default, settings);
	window->setVerticalSyncEnabled(true);
	return window;
}

int main()
{
	Camera camera(CAMERA_VIRT_WIDTH, CAMERA_VIRT_HEIGHT, CAMERA_REAL_WIDTH, CAMERA_REAL_HEIGHT);
	auto renderWindow = CreateWindow(camera);
	auto font = LoadFont();
	auto menu = std::make_unique<MainMenu>(*font, camera);

	sf::Clock deltaClock;

	while (renderWindow->isOpen())
	{
		float dt = deltaClock.restart().asMilliseconds() / 1000.f;

		for (sf::Event event; renderWindow->pollEvent(event);)
		{
			if (event.type == sf::Event::Closed) {
				renderWindow->close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				menu->KeyPressed(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased) {
				menu->KeyReleased(event.key.code);
			}
		}
		if (menu->WantToQuit()) {
			return 1;
		}
		renderWindow->clear(sf::Color::Black);

		menu->Update(dt);
		menu->Draw(*renderWindow);
		
		renderWindow->setView(camera.GetVirtualView());
		renderWindow->display();

		// Sleep
		if (dt < 1 / STABLE_FPS) {
			auto sleepTime = static_cast<size_t>(1 / STABLE_FPS - dt);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}

	return 0;
}