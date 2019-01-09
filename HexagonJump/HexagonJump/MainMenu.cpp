#include "MainMenu.hpp"
#include "AppInfo.hpp"
#include "Controls.hpp"
#include "OpenFileDialog.hpp"

#include <iostream>

namespace hexagon {

MainMenu::MainMenu(const sf::Font& font, Camera& camera)
	: _font(font)
	, _camera(camera)
	, _musicVisualizationManager(Game::NUM_BEAT_UNITS)
{
	CreateMainLevelGUI();
	CreateControlsLevelGUI();
	CreatePlaylistLevelGUI();

	for (size_t i = 0u; i < STRIPE_MANAGER_INIT_ITERATIONS; i++) {
		_stripeManager.Update(_camera, Game::TIMERATE, true);
	}
}

void MainMenu::KeyPressed(sf::Keyboard::Key key)
{
	if (_game) {
		_game->KeyPressed(key);
		return;
	}
	if (key == sf::Keyboard::Escape) {
		if (_activeMenuLevel == MenuLevel::MAIN) {
			_quit = true;
		}
		else {
			_activeMenuLevel = MenuLevel::MAIN;
		}
	}
	else {
		GetActiveGUI().KeyPressed(key);
	}
}

void MainMenu::Update(float deltaTime)
{
	if (_game) {
		_game->Update(deltaTime);
		return;
	}
	_stripeManager.Update(_camera, deltaTime, true);
	GetActiveGUI().Update(deltaTime);
	if (HasConversionFinished()) {
		_progressBar->get().Close();
		auto musicName = _conversionResult.get();
		_playlist->get().AddElement(musicName);
		if (_scoreLabel->get().GetText().empty()) {
			ShowScore(musicName);
		}
	}
}

void MainMenu::Draw(sf::RenderWindow& window) const
{
	if (_game) {
		_game->Draw(window);
		return;
	}
	auto color = GetColor(COLOR_PALETTE, ColorEntity::STRIPE);
	DrawRectangle(window, _camera.GetVirtualViewRect(), color);
	_stripeManager.Draw(window, _camera, color);
	GetActiveGUI().Draw(window);
}

void MainMenu::CreateMainLevelGUI()
{
	auto& gui = (_menuLevelsGuiManagers[MenuLevel::MAIN] = std::make_unique<gui::GuiManager>(_font));

	sf::Vector2f titlePosition = {
		GUI_HORIZONTAL_POSITION,
		TITLE_VERTICAL_POSITION
	};

	gui->AddGuiElement(std::make_unique<gui::Label>(APP_NAME,
		gui::Button::COLOR, // same color as other buttons
		titlePosition,
		TITLE_SIZE));

	sf::Vector2f playButtonPosition = { 
		GUI_HORIZONTAL_POSITION,
		_camera.GetVirtualHeight() / 2.f - FONT_SIZE * 2.f
	};
	sf::Vector2f controlsButtonPosition = {
		GUI_HORIZONTAL_POSITION,
		_camera.GetVirtualHeight() / 2.f
	};
	sf::Vector2f quitButtonPosition = {
		GUI_HORIZONTAL_POSITION,
		_camera.GetVirtualHeight() / 2.f + FONT_SIZE * 4.f
	};

	gui->AddGuiElement(std::make_unique<gui::Button>("Play", 
		playButtonPosition, 
		FONT_SIZE, 
		[&] { _activeMenuLevel = MenuLevel::PLAYLIST; }));

	gui->AddGuiElement(std::make_unique<gui::Button>("Controls",
		controlsButtonPosition,
		FONT_SIZE,
		[&] { _activeMenuLevel = MenuLevel::CONTROLS; }));

	gui->AddGuiElement(std::make_unique<gui::Button>("Quit",
		quitButtonPosition,
		FONT_SIZE,
		[&] { _quit = true; }));
}

void MainMenu::CreatePlaylistLevelGUI()
{
	auto& gui = (_menuLevelsGuiManagers[MenuLevel::PLAYLIST] = std::make_unique<gui::GuiManager>(_font));
	
	CreateScoreLabel();
	CreateAndFillPlaylist();
	CreateProgressBar();

	sf::Vector2f openMusicButtonPosition = {
		GUI_HORIZONTAL_POSITION,
		_camera.GetVirtualHeight() - OPEN_MUSIC_BUTTON_VERTICAL_OFFSET
	};
	gui->AddGuiElement(std::make_unique<gui::Button>("Open music",
		openMusicButtonPosition,
		FONT_SIZE,
		[&] { AddMusic(RunOpenFileDialog()); }));
}

void MainMenu::CreateControlsLevelGUI()
{
	static const std::vector<std::pair<std::string, std::string>> controls = {
		{ controls::PLAYER_JUMP_KEY_STR, "Jump" },
		{ controls::PLAYER_FALL_DOWN_FAST_KEY_STR, "Fall down" },
		{ controls::PAUSE_KEY_STR, "Pause" },
		{ controls::RESTART_KEY_STR, "Jump" }
	};

	auto& gui = (_menuLevelsGuiManagers[MenuLevel::CONTROLS] = std::make_unique<gui::GuiManager>(_font));

	sf::Vector2f position = {
		_camera.GetVirtualWidth() / 2.f,
		_camera.GetVirtualHeight() / 2.f - FONT_SIZE * controls.size() / 2.f
	};

	for (auto[key, action] : controls) {
		auto text = key + " - " + action;
		auto label = std::make_unique<gui::Label>(text, gui::Button::COLOR, position, FONT_SIZE, true);
		gui->AddGuiElement(std::move(label));
		position.y += FONT_SIZE;
	}
}

void MainMenu::CreateScoreLabel()
{
	auto& gui = _menuLevelsGuiManagers[MenuLevel::PLAYLIST];
	sf::Vector2f scorePosition = { GUI_HORIZONTAL_POSITION, SCORE_LABEL_VERTICAL_POSITION };

	auto& scoreLabel = gui->AddGuiElement(std::make_unique<gui::Label>("",
		gui::Button::COLOR,
		scorePosition,
		FONT_SIZE));

	_scoreLabel = dynamic_cast<gui::Label&>(*scoreLabel);
}

void MainMenu::CreateAndFillPlaylist()
{
	auto& gui = _menuLevelsGuiManagers[MenuLevel::PLAYLIST];
	sf::Vector2f playlistPosition = { GUI_HORIZONTAL_POSITION, PLAYLIST_VERTICAL_POSITION };

	auto& playlist = *gui->AddGuiElement(std::make_unique<gui::ListBox>(playlistPosition,
		FONT_SIZE,
		PLAYLIST_NUM_ELEMENTS,
		[&](const auto& elem) { StartGame(elem); },
		[&](const auto& elem) { ShowScore(elem); }));

	_playlist = dynamic_cast<gui::ListBox&>(playlist);

	// Fill it with available music
	for (const auto& [musicName, _] : _musicVisualizationManager.GetMusicList()) {
		_playlist->get().AddElement(musicName);
	}
	auto activeelem = _playlist->get().GetActiveElement();
}

void MainMenu::CreateProgressBar()
{
	auto& gui = _menuLevelsGuiManagers[MenuLevel::PLAYLIST];
	sf::FloatRect barArea = {
		_camera.GetVirtualWidth() / 2.f - PROGRESS_BAR_WIDTH / 2.f,
		_camera.GetVirtualHeight() / 2.f - PROGRESS_BAR_HEIGHT / 2.f,
		PROGRESS_BAR_WIDTH,
		PROGRESS_BAR_HEIGHT
	};

	auto& progressBar = gui->AddGuiElement(std::make_unique<gui::ThreadSafeProgressBar>(barArea, FONT_SIZE));
	_progressBar = dynamic_cast<gui::ThreadSafeProgressBar&>(*progressBar);
}

void MainMenu::StartGame(const std::string& musicName)
{
	_game = std::make_unique<Game>(_font, _camera, musicName, _musicVisualizationManager);
}

void MainMenu::ShowScore(const std::string& musicName)
{
	auto score = _musicVisualizationManager.GetMusicList().at(musicName);
	_scoreLabel->get().SetText("Best score: " + std::to_string(score));
}

void MainMenu::AddMusic(const std::string& musicPath)
{
	auto& progressBar = _progressBar->get();
	progressBar.Invoke();

	_conversionResult = _musicVisualizationManager.ConvertNewMusicAsync(musicPath,
		Game::TIMERATE, 
		progressBar);
}

}