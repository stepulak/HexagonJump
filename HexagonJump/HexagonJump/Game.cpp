#include "Game.hpp"
#include "Controls.hpp"

#include <cmath>

namespace hexagon {

Game::Game(const sf::Font& font,
	Camera& camera,
	const std::string& musicName,
	MusicVisulizationManager& manager)
	: _font(font)
	, _camera(camera)
	, _musicVisualizationManager(manager)
	, _musicName(musicName)
{
	auto musicData = manager.OpenMusic(musicName);

	_beatUnitManager = std::make_unique<BeatUnitManager>(std::move(musicData.visualization),
		NUM_BEAT_UNITS, 
		TIMERATE);
	_music.openFromFile(musicData.path);
	camera.SetVelocity(CAMERA_VELOCITY);

	Reset();
}

void Game::Start()
{
	_stopped = false;
	_beatUnitManager->Start();
	_music.play();
}

void Game::Stop()
{
	_stopped = true;
	_beatUnitManager->Stop();
	_music.pause();
}

void Game::KeyPressed(const sf::Keyboard::Key& key)
{
	if (_guiManager->KeyPressed(key)) {
		return; // handled by gui
	}
	auto& player = _world->GetPlayer();

	switch (key)
	{
	case controls::PLAYER_JUMP_KEY:
		player.TryToJump();
		break;
	case controls::PLAYER_FALL_DOWN_FAST_KEY:
		player.TryToFallDownFast();
		break;
	case controls::PAUSE_KEY:
		if (_stopped) {
			Start();
		}
		else {
			Stop();
		}
		break;
	case controls::RESTART_KEY:
		Reset();
	default:
		break;
	}
}

void Game::Update(float deltaTime)
{
	if (_firstUpdateIteration) {
		deltaTime = std::max(deltaTime, MAX_DELTA_TIME);
		_firstUpdateIteration = false;
	}

	UpdateGUI(deltaTime);

	if (!_stopped && !_gameEnded) {
		UpdatePlayerAndCamera(deltaTime);
		_world->Update(deltaTime, !ShouldSpawnObstacles());
		_beatUnitManager->Update(deltaTime);
		SyncMusicAndBeatManager(deltaTime);
		TestIfPlayerWon();
	}
}

void Game::Draw(sf::RenderWindow& window) const
{
	_world->Draw(window);
	_guiManager->Draw(window);
}

float Game::GetMusicTime() const
{
	if (MusicEnded()) {
		return _music.getDuration().asSeconds();
	}
	return _music.getPlayingOffset().asSeconds();
}

void Game::Reset()
{
	// Music
	_music.stop();
	_beatUnitManager->Reset();

	// Stats
	_camera.SetPosition(0.f);
	_musicBeatManagerSyncTimer = 0.f;
	_playerDiedWaitTimer = 0.f;
	_gameEnded = false;

	// World
	_world = std::make_unique<World>(_camera, *_beatUnitManager);

	CreateGUI();
	Start();
}

void Game::CreateGUI()
{
	_guiManager = std::make_unique<gui::GuiManager>(_font);
	_guiManager->AddGuiElement(TIME_HUD_NAME, std::make_unique<gui::GameStatsHUD>());
	CreateQuitDialog();
}

void Game::CreateQuitDialog()
{
	auto dialog = std::make_unique<gui::YesNoDialog>(QUIT_DIALOG_TEXT,
		sf::FloatRect {
			_camera.GetVirtualWidth() / 2.f - QUIT_DIALOG_WIDTH / 2.f,
			_camera.GetVirtualHeight() / 2.f - QUIT_DIALOG_HEIGHT / 2.f,
			QUIT_DIALOG_WIDTH,
			QUIT_DIALOG_HEIGHT
		},
		FONT_SIZE, 
		[&] { _wantToQuit = true; });

	_guiManager->AddGuiElement(QUIT_DIALOG_NAME, std::move(dialog));
}

void Game::CreateEndScreenHUD(const std::string& resultMessage)
{
	auto hud = std::make_unique<gui::EndScreenHUD>(_camera,
		static_cast<size_t>(GetMusicTime()),
		resultMessage,
		controls::RESTART_KEY_STR);

	_guiManager->AddGuiElement(END_SCREEN_HUD_NAME, std::move(hud));
}

void Game::UpdateGUI(float deltaTime)
{
	if (!_world->PlayerDied()) {
		GetTimeHUD().UpdateTime(static_cast<size_t>(GetMusicTime()));
	}
	_guiManager->Update(deltaTime);
}

void Game::TestIfPlayerWon()
{
	if (MusicEnded() && !_world->PlayerDied()) {
		GameEnded(PLAYER_WON_RESULT_MESSAGE);
	}
}

void Game::UpdatePlayerAndCamera(float deltaTime)
{
	auto cameraVelocity = _camera.GetVelocity();
	_camera.Move(cameraVelocity * deltaTime);

	// Dead XOR alive?
	if (_world->PlayerDied()) {
		UpdatePlayerDeath(deltaTime);
	}
	else {
		_world->GetPlayer().StartMoving(cameraVelocity, true);
	}
}

void Game::UpdatePlayerDeath(float deltaTime)
{
	_playerDiedWaitTimer += deltaTime;
	if (_playerDiedWaitTimer >= PLAYER_DIED_WAIT_TIME) {
		GameEnded(PLAYER_DIED_RESULT_MESSAGE);
	}
}

void Game::SyncMusicAndBeatManager(float deltaTime)
{
	_musicBeatManagerSyncTimer += deltaTime;
	if (_musicBeatManagerSyncTimer >= MUSIC_BEAT_MANAGER_SYNC_TIME) {
		_musicBeatManagerSyncTimer -= MUSIC_BEAT_MANAGER_SYNC_TIME;
		_beatUnitManager->SyncTimingWithMusic(static_cast<float>(GetMusicTime()));
	}
}

void Game::GameEnded(const std::string& resultMessage)
{
	_gameEnded = true;
	CreateEndScreenHUD(resultMessage);
}

bool Game::ShouldSpawnObstacles() const
{
	if (_music.getStatus() == sf::Music::Playing) {
		auto duration = _music.getDuration().asSeconds();
		return GetMusicTime() < duration - LAST_SECONDS_WITHOUT_OBSTACLES;
	}
	return false;
}

}