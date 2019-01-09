#include "MainMenu.hpp"
#include "AppInfo.hpp"
#include "Controls.hpp"

namespace hexagon {

MainMenu::MainMenu(const sf::Font& font, Camera& camera)
	: _font(font)
	, _camera(camera)
	, _musicVisualizationManager(Game::NUM_BEAT_UNITS)
{
	InitGUIMainLevel();
	InitGUIControlsLevel();
	InitGUIPlaylistLevel();

	for (size_t i = 0u; i < STRIPE_MANAGER_INIT_ITERATIONS; i++) {
		_stripeManager.Update(_camera, Game::TIMERATE, true);
	}
}

void MainMenu::KeyPressed(sf::Keyboard::Key key)
{
	GetActiveGUI().KeyPressed(key);
}

void MainMenu::Update(float deltaTime)
{
	_stripeManager.Update(_camera, deltaTime, true);
	GetActiveGUI().Update(deltaTime);
}

void MainMenu::Draw(sf::RenderWindow& window) const
{
	auto color = GetColor(COLOR_PALETTE, ColorEntity::STRIPE);
	DrawRectangle(window, _camera.GetVirtualViewRect(), color);
	_stripeManager.Draw(window, _camera, color);
	GetActiveGUI().Draw(window);
}

void MainMenu::InitGUIMainLevel()
{
	auto& gui = (_menuLevelsGuiManagers[MenuLevel::MAIN] = std::make_unique<gui::GuiManager>(_font));

	sf::Vector2f titlePosition = {
		GUI_HORIZONTAL_OFFSET,
		TITLE_VERTICAL_OFFSET
	};

	gui->AddGuiElement(std::make_unique<gui::Label>(APP_NAME,
		gui::Button::COLOR, // same color as other buttons
		titlePosition,
		TITLE_SIZE));

	sf::Vector2f playButtonPosition = { 
		GUI_HORIZONTAL_OFFSET, 
		_camera.GetVirtualHeight() / 2.f - TEXT_SIZE * 2.f
	};
	sf::Vector2f controlsButtonPosition = {
		GUI_HORIZONTAL_OFFSET,
		_camera.GetVirtualHeight() / 2.f
	};
	sf::Vector2f quitButtonPosition = {
		GUI_HORIZONTAL_OFFSET,
		_camera.GetVirtualHeight() / 2.f + TEXT_SIZE * 4.f
	};

	gui->AddGuiElement(std::make_unique<gui::Button>("Play", 
		playButtonPosition, 
		TEXT_SIZE, 
		[&] { _activeMenuLevel = MenuLevel::PLAYLIST; }));

	gui->AddGuiElement(std::make_unique<gui::Button>("Controls",
		controlsButtonPosition,
		TEXT_SIZE,
		[&] { _activeMenuLevel = MenuLevel::CONTROLS; }));

	gui->AddGuiElement(std::make_unique<gui::Button>("Quit",
		quitButtonPosition,
		TEXT_SIZE,
		[&] { _quit = true; }));
}

void MainMenu::InitGUIPlaylistLevel()
{
}

void MainMenu::InitGUIControlsLevel()
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
		_camera.GetVirtualHeight() / 2.f - TEXT_SIZE * controls.size() / 2.f
	};

	for (auto[key, action] : controls) {
		auto text = key + " - " + action;
		auto label = std::make_unique<gui::Label>(text, gui::Button::COLOR, position, TEXT_SIZE, true);
		gui->AddGuiElement(std::move(label));
		position.y += TEXT_SIZE;
	}

	// Back button
	gui->AddGuiElement(std::make_unique<gui::Button>("Back",
		position,
		TEXT_SIZE,
		[&] { _activeMenuLevel = MenuLevel::MAIN; }));
}

}