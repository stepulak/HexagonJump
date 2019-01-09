#pragma once

#include "Game.hpp"

namespace hexagon {

class MainMenu : public Runnable {
public:

	MainMenu(const sf::Font& font, Camera& camera);

	bool WantToQuit() const override { return _quit; }

	void KeyPressed(sf::Keyboard::Key key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;

private:

	enum class MenuLevel {
		MAIN,
		PLAYLIST,
		CONTROLS
	};

	static constexpr size_t STRIPE_MANAGER_INIT_ITERATIONS = 300u;

	static constexpr float TEXT_SIZE = 40.f;
	static constexpr float TITLE_SIZE = TEXT_SIZE * 2.f;

	static constexpr float TITLE_VERTICAL_OFFSET = 40.f;
	static constexpr float GUI_HORIZONTAL_OFFSET = 100.f;

	static constexpr auto COLOR_PALETTE = ColorPalette::BLUE;

	void InitGUIMainLevel();
	void InitGUIPlaylistLevel();
	void InitGUIControlsLevel();

	gui::GuiManager& GetActiveGUI() { return *_menuLevelsGuiManagers.at(_activeMenuLevel); }
	const gui::GuiManager& GetActiveGUI() const { return *_menuLevelsGuiManagers.at(_activeMenuLevel); }

	const sf::Font& _font;
	Camera& _camera;
	BackgroundStripeManager _stripeManager;
	MusicVisulizationManager _musicVisualizationManager;
	std::unique_ptr<Game> _game;
	std::unordered_map<MenuLevel, std::unique_ptr<gui::GuiManager>> _menuLevelsGuiManagers;
	MenuLevel _activeMenuLevel = MenuLevel::MAIN;
	bool _quit = false;
};

}