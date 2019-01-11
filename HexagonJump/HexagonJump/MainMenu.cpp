#include "MainMenu.hpp"
#include "Controls.hpp"
#include "OpenFileDialog.hpp"

namespace hexagon {

namespace {

const std::vector<std::pair<std::string, std::string>> Controls = {
		{ controls::PLAYER_JUMP_KEY_STR, "Jump" },
		{ controls::PLAYER_FALL_DOWN_FAST_KEY_STR, "Fall down" },
		{ controls::PAUSE_KEY_STR, "Pause" },
		{ controls::RESTART_KEY_STR, "Restart" }
};

}

MainMenu::MainMenu(const sf::Font& font, Camera& camera)
	: _font(font)
	, _camera(camera)
	, _musicVisualizationManager(Game::NUM_BEAT_UNITS)
{
	CreateMainLevelGUI();
	CreateControlsLevelGUI();
	CreatePlaylistLevelGUI();

	// Make sure that stripes are already equally spawned on screen
	for (size_t i = 0u; i < STRIPE_MANAGER_INIT_ITERATIONS; i++) {
		_stripeManager.Update(_camera, Game::TIMERATE, true);
	}
}

void MainMenu::KeyPressed(const sf::Keyboard::Key& key)
{
	if (_game) {
		_game->KeyPressed(key);
		return;
	}
	if (key == sf::Keyboard::Escape) {
		if (_activeGuiLevel == GuiLevel::MAIN) {
			_wantToQuit = true;
		}
		else {
			_activeGuiLevel = GuiLevel::MAIN;
		}
		return;
	}
	GetActiveGUI().KeyPressed(key);
}

void MainMenu::Update(float deltaTime)
{
	if (_game) {
		_game->Update(deltaTime);

		if (_game->WantToQuit()) {
			_game.reset();
			_camera.SetPosition(0.f);
		}
		return;
	}
	_stripeManager.Update(_camera, deltaTime, true);
	GetActiveGUI().Update(deltaTime);
	TryHandleConversionResult();
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
	auto& gui = (_guiManagers[GuiLevel::MAIN] = std::make_unique<gui::GuiManager>(_font));

	gui->AddGuiElement(TITLE, std::make_unique<gui::Label>(TITLE,
		gui::Button::COLOR, // unify colors
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			TITLE_VERTICAL_POSITION
		},
		TITLE_SIZE));

	auto playButton = std::make_unique<gui::Button>(PLAY_BUTTON_TEXT,
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			_camera.GetVirtualHeight() / 2.f - FONT_SIZE * 2.f
		},
		FONT_SIZE,
		[&] { _activeGuiLevel = GuiLevel::PLAYLIST; });

	gui->AddGuiElement(PLAY_BUTTON_TEXT, std::move(playButton));

	auto controlButton = std::make_unique<gui::Button>(CONTROLS_BUTTON_TEXT,
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			_camera.GetVirtualHeight() / 2.f
		},
		FONT_SIZE,
		[&] { _activeGuiLevel = GuiLevel::CONTROLS; });

	gui->AddGuiElement(CONTROLS_BUTTON_TEXT, std::move(controlButton));

	auto quitButton = std::make_unique<gui::Button>(QUIT_BUTTON_TEXT,
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			_camera.GetVirtualHeight() / 2.f + FONT_SIZE * 4.f
		},
		FONT_SIZE,
		[&] { _wantToQuit = true; });
	
	gui->AddGuiElement(QUIT_BUTTON_TEXT, std::move(quitButton));
}

void MainMenu::CreatePlaylistLevelGUI()
{
	auto& gui = (_guiManagers[GuiLevel::PLAYLIST] = std::make_unique<gui::GuiManager>(_font));
	
	CreateAndFillPlaylist();
	CreateProgressBar();

	auto addMusicButton = std::make_unique<gui::Button>(ADD_MUSIC_BUTTON_TEXT,
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			_camera.GetVirtualHeight() - OPEN_MUSIC_BUTTON_VERTICAL_OFFSET
		},
		FONT_SIZE,
		[&] { AddMusic(RunOpenFileDialog()); });

	gui->AddGuiElement(ADD_MUSIC_BUTTON_TEXT, std::move(addMusicButton));

	auto selectMusicLabel = std::make_unique<gui::Label>(SELECT_MUSIC_LABEL_TEXT,
		gui::Button::COLOR, // unify colors
		sf::Vector2f{
			GUI_HORIZONTAL_POSITION,
			SELECT_MUSIC_LABEL_VERTICAL_POSITION
		},
		FONT_SIZE);

	gui->AddGuiElement(SELECT_MUSIC_LABEL_TEXT, std::move(selectMusicLabel));
}

void MainMenu::CreateControlsLevelGUI()
{
	auto& gui = (_guiManagers[GuiLevel::CONTROLS] = std::make_unique<gui::GuiManager>(_font));

	sf::Vector2f position = {
		_camera.GetVirtualWidth() / 2.f,
		_camera.GetVirtualHeight() / 2.f - FONT_SIZE * Controls.size() / 2.f
	};

	for (auto[key, action] : Controls) {
		auto text = key + " - " + action;
		auto label = std::make_unique<gui::Label>(text, gui::Button::COLOR, position, FONT_SIZE, true);
		gui->AddGuiElement(text, std::move(label));
		position.y += FONT_SIZE;
	}
}

void MainMenu::CreateAndFillPlaylist()
{
	auto& gui = _guiManagers[GuiLevel::PLAYLIST];

	auto playlistListBox = std::make_unique<gui::ListBox>(
		sf::Vector2f{ 
			GUI_HORIZONTAL_POSITION, 
			PLAYLIST_VERTICAL_POSITION 
		},
		FONT_SIZE,
		PLAYLIST_NUM_ELEMENTS,
		[&](const auto& elem) { StartGame(elem); });

	gui->AddGuiElement(PLAYLIST_NAME, std::move(playlistListBox));

	auto& playlist = GetPlaylist();

	// Fill it with available music
	for (const auto& musicName : _musicVisualizationManager.GetMusicList()) {
		playlist.AddElement(musicName);
	}
}

void MainMenu::CreateProgressBar()
{
	auto& gui = _guiManagers[GuiLevel::PLAYLIST];

	auto bar = std::make_unique<gui::ThreadSafeProgressBar>(
		sf::FloatRect {
			_camera.GetVirtualWidth() / 2.f - PROGRESS_BAR_WIDTH / 2.f,
			_camera.GetVirtualHeight() / 2.f - PROGRESS_BAR_HEIGHT / 2.f,
			PROGRESS_BAR_WIDTH,
			PROGRESS_BAR_HEIGHT
		}, 
		FONT_SIZE);

	gui->AddGuiElement(PROGRESS_BAR_NAME, std::move(bar));
}

void MainMenu::StartGame(const std::string& musicName)
{
	_game = std::make_unique<Game>(_font, _camera, _musicVisualizationManager, musicName);
}

void MainMenu::AddMusic(const std::string& musicPath)
{
	auto& progressBar = GetProgressBar();
	progressBar.Invoke();

	_conversionResult = _musicVisualizationManager.ConvertMusicAsync(
		musicPath,
		Game::TIMERATE, 
		progressBar);
}

void MainMenu::TryHandleConversionResult()
{
	if (_conversionResult.valid() && std::future_status::ready ==
		_conversionResult.wait_for(std::chrono::seconds(0))) {

		if (auto musicName = _conversionResult.get()) {
			GetPlaylist().AddElement(*musicName);
		}
		GetProgressBar().Close();
	}
}

}