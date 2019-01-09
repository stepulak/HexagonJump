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
	static constexpr size_t PLAYLIST_NUM_ELEMENTS = 1u;
	static constexpr auto COLOR_PALETTE = ColorPalette::BLUE;

	static constexpr float FONT_SIZE = 40.f;
	static constexpr float TITLE_SIZE = FONT_SIZE * 2.f;
	static constexpr float TITLE_VERTICAL_POSITION = 40.f;
	static constexpr float GUI_HORIZONTAL_POSITION = 100.f;
	static constexpr float SCORE_LABEL_VERTICAL_POSITION = 60.f;
	static constexpr float PLAYLIST_VERTICAL_POSITION = 160.f;
	static constexpr float OPEN_MUSIC_BUTTON_VERTICAL_OFFSET = 50.f;

	void CreateMainLevelGUI();
	void CreatePlaylistLevelGUI();
	void CreateControlsLevelGUI();

	void CreateScoreLabel();
	void CreateAndFillPlaylist();

	void StartGame(const std::string& musicName);
	void ShowScore(const std::string& musicName);
	void AddMusic(const std::string& musicPath);

	gui::GuiManager& GetActiveGUI() { return *_menuLevelsGuiManagers.at(_activeMenuLevel); }
	const gui::GuiManager& GetActiveGUI() const { return *_menuLevelsGuiManagers.at(_activeMenuLevel); }

	const sf::Font& _font;
	Camera& _camera;
	BackgroundStripeManager _stripeManager;
	MusicVisulizationManager _musicVisualizationManager;
	std::unique_ptr<Game> _game;
	std::unordered_map<MenuLevel, std::unique_ptr<gui::GuiManager>> _menuLevelsGuiManagers;
	std::optional<std::reference_wrapper<gui::Label>> _scoreLabel;
	std::optional<std::reference_wrapper<gui::ListBox>> _playlist;
	MenuLevel _activeMenuLevel = MenuLevel::MAIN;
	bool _quit = false;
};

}