#pragma once

#include "AppInfo.hpp"
#include "Game.hpp"

namespace hexagon {

class MainMenu : public Runnable {
public:

	MainMenu(const sf::Font& font, Camera& camera);

	bool WantToQuit() const override { return _wantToQuit; }

	void KeyPressed(const sf::Keyboard::Key& key) override;
	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) const override;

private:

	enum class MenuLevel {
		MAIN,
		PLAYLIST,
		CONTROLS
	};

	static constexpr size_t STRIPE_MANAGER_INIT_ITERATIONS = 300u;
	static constexpr size_t PLAYLIST_NUM_ELEMENTS = 10u;
	static constexpr auto COLOR_PALETTE = ColorPalette::BLUE;

	static constexpr float FONT_SIZE = 40.f;

	static constexpr auto TITLE = APP_NAME;
	static constexpr float TITLE_SIZE = FONT_SIZE * 2.f;
	static constexpr float TITLE_VERTICAL_POSITION = 40.f;
	
	static constexpr float GUI_HORIZONTAL_POSITION = 100.f;
	static constexpr float SELECT_MUSIC_LABEL_VERTICAL_POSITION = 60.f;
	static constexpr float OPEN_MUSIC_BUTTON_VERTICAL_OFFSET = 100.f;

	static constexpr auto PLAYLIST_NAME = "playlist";
	static constexpr float PLAYLIST_VERTICAL_POSITION = 160.f;

	static constexpr auto PROGRESS_BAR_NAME = "progress_bar";
	static constexpr float PROGRESS_BAR_WIDTH = 250.f;
	static constexpr float PROGRESS_BAR_HEIGHT = 150.f;
	
	static constexpr auto PLAY_BUTTON_TEXT = "Play";
	static constexpr auto CONTROLS_BUTTON_TEXT = "Controls";
	static constexpr auto QUIT_BUTTON_TEXT = "Quit";
	static constexpr auto ADD_MUSIC_BUTTON_TEXT = "Add Music";
	static constexpr auto SELECT_MUSIC_LABEL_TEXT = "Select Music:";

	gui::GuiManager& GetActiveGUI() { 
		return *_guiManagers.at(_activeMenuLevel);
	}

	const gui::GuiManager& GetActiveGUI() const {
		return *_guiManagers.at(_activeMenuLevel);
	}

	gui::ThreadSafeProgressBar& GetProgressBar() {
		auto& gui = _guiManagers.at(MenuLevel::PLAYLIST);
		return gui->GetGuiElement<gui::ThreadSafeProgressBar>(PROGRESS_BAR_NAME);
	}

	gui::ListBox& GetPlaylist() {
		auto& gui = _guiManagers.at(MenuLevel::PLAYLIST);
		return gui->GetGuiElement<gui::ListBox>(PLAYLIST_NAME);
	}

	void CreateMainLevelGUI();
	void CreatePlaylistLevelGUI();
	void CreateControlsLevelGUI();

	void CreateAndFillPlaylist();
	void CreateProgressBar();

	void TryHandleConversionResult();
	void StartGame(const std::string& musicName);
	void AddMusic(const std::string& musicPath);

	const sf::Font& _font;
	Camera& _camera;
	Game::Ptr _game;
	BackgroundStripeManager _stripeManager;
	MusicVisulizationManager _musicVisualizationManager;
	MusicVisulizationManager::ConvertResult _conversionResult;
	std::unordered_map<MenuLevel, gui::GuiManager::Ptr> _guiManagers;
	MenuLevel _activeMenuLevel = MenuLevel::MAIN;
	bool _wantToQuit = false;
};

}