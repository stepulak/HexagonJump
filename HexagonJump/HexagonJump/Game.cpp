#include "Game.hpp"

#include <cmath>
#include <iostream>

namespace hexagon {

Game::Game(const sf::Font& font,
	Camera& camera,
	const std::string& musicName,
	MusicVisulizationManager& manager,
	size_t numBeatUnits)
	: _font(font)
	, _camera(camera)
	, _musicVisualizationManager(manager)
	, _musicName(musicName)
{
	camera.SetVelocity(CAMERA_VELOCITY);
	
	auto musicData = manager.LoadMusic(musicName);
	_beatUnitManager = std::make_unique<BeatUnitManager>(std::move(musicData.visualization), numBeatUnits);
	
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

void Game::KeyPressed(sf::Keyboard::Key key)
{
	if (_guiManager->KeyPressed(key)) {
		return; // handled by gui
	}

	auto& player = _world->GetPlayer();

	switch (key)
	{
	case PLAYER_JUMP_KEY:
		player.TryToJump();
		break;
	case PLAYER_FALL_DOWN_FAST_KEY:
		player.TryToFallDownFast();
		break;
	case PAUSE_KEY:
		if (_stopped) {
			Start();
		}
		else {
			Stop();
		}
		break;
	case RESTART_KEY:
		Reset();
	default:
		break;
	}
}

void Game::Update(float deltaTime)
{
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
	_world = std::make_unique<World>(_camera, _beatUnitManager);

	CreateGUI();
	Start();
}

void Game::CreateGUI()
{
	_guiManager = std::make_unique<gui::GuiManager>(_font);
	
	CreateQuitDialog();

	auto& statsHud = _guiManager->AddGuiElement(std::make_unique<gui::GameStatsHUD>());
	_statsHUD = std::ref(dynamic_cast<gui::GameStatsHUD&>(*statsHud));
}

void Game::CreateQuitDialog()
{
	auto quitDialogRect = sf::FloatRect{
		_camera.GetVirtualWidth() - QUIT_DIALOG_WIDTH / 2.f,
		_camera.GetVirtualHeight() - QUIT_DIALOG_HEIGHT / 2.f,
		QUIT_DIALOG_WIDTH,
		QUIT_DIALOG_HEIGHT
	};

	_guiManager->AddGuiElement(std::make_unique<gui::YesNoDialog>(QUIT_DIALOG_TEXT,
		quitDialogRect, 40,
		[&](bool yesPressed) {
			if (yesPressed) {
				SaveAndQuit();
			}
	}));
}

void Game::CreateEndScreenHUD(const std::string& resultMessage)
{
	_guiManager->AddGuiElement(std::make_unique<gui::EndScreenHUD>(_camera,
		static_cast<size_t>(GetMusicTime()),
		resultMessage,
		RESTART_KEY_STR));
}

void Game::UpdateGUI(float deltaTime)
{
	_guiManager->Update(deltaTime);
	if (!_world->PlayerDied()) {
		_statsHUD->get().UpdateStats(static_cast<size_t>(GetMusicTime()));
	}
}

void Game::TestIfPlayerWon()
{
	if (_music.getStatus() == sf::SoundSource::Stopped && !_world->PlayerDied()) {
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
	SaveAndQuit();
}

void Game::SaveAndQuit()
{
	_musicVisualizationManager.UpdateScoreIfBetter(_musicName, static_cast<unsigned>(GetMusicTime()));
	_quit = true;
}

bool Game::ShouldSpawnObstacles() const
{
	if (_music.getStatus() == sf::Music::Playing) {
		auto duration = _music.getDuration().asSeconds();
		auto time = _music.getPlayingOffset().asSeconds();
		return time < duration - LAST_SECONDS_WITHOUT_OBSTACLES;
	}
	return false;
}

}