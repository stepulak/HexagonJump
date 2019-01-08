#include "Game.hpp"

#include <cmath>
#include <iostream>

namespace hexagon {

Game::Game(const sf::Font& font, 
	sf::Music& music,
	MusicVisualization&& visualization,
	Camera& camera,
	size_t numBeatUnits)
	: _font(font)
	, _music(music)
	, _beatUnitManager(std::move(visualization), numBeatUnits, TIMERATE)
	, _camera(camera)
{
	camera.SetVelocity(CAMERA_VELOCITY);
	Reset();
}

void Game::Start()
{
	_stopped = false;
	_beatUnitManager.Start();
	_music.play();
}

void Game::Stop()
{
	_stopped = true;
	_beatUnitManager.Stop();
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
	case PLAYER_KEY_JUMP:
		player.TryToJump();
		break;
	case PLAYER_KEY_FALL_DOWN_FAST:
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
	default:
		break;
	}
}

void Game::Update(float deltaTime)
{
	if (_stopped) {
		return; // skip
	}
	UpdatePlayerAndCamera(deltaTime);
	UpdateGui(deltaTime);
	_world->Update(deltaTime, !ShouldSpawnObstacles());
	_beatUnitManager.Update(deltaTime);
	SyncMusicAndBeatManager(deltaTime);
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
	_beatUnitManager.Reset();

	// Stats
	_camera.SetPosition(0.f);
	_musicBeatManagerSyncTimer = 0.f;
	_playerDeathWaitTimer = 0.f;

	// World
	_world.reset();
	_world = std::make_unique<World>(_camera, _beatUnitManager);

	// GUI
	_guiManager.reset();
	_guiManager = std::make_unique<gui::GuiManager>(_font);

	auto& statsHud = _guiManager->AddGuiElement(std::make_unique<gui::GameStatsHUD>());
	_statsHUD = std::ref(dynamic_cast<gui::GameStatsHUD&>(*statsHud));

	Start();
}

void Game::UpdatePlayerAndCamera(float deltaTime)
{
	auto cameraVelocity = _camera.GetVelocity();

	_camera.Move(cameraVelocity * deltaTime);

	if (_world->PlayerDied()) {
		UpdatePlayerDeath(deltaTime);
	}
	else {
		_world->GetPlayer().StartMoving(cameraVelocity, true);
	}
}

void Game::UpdatePlayerDeath(float deltaTime)
{
	_playerDeathWaitTimer += deltaTime;
	if (_playerDeathWaitTimer >= PLAYER_DEATH_WAIT_TIME) {
		Reset(); // Reset game completely
	}
}

void Game::UpdateGui(float deltaTime)
{
	_guiManager->Update(deltaTime);
	if (!_world->PlayerDied()) {
		_statsHUD->get().UpdateStats(static_cast<size_t>(_music.getPlayingOffset().asSeconds()));
	}
}

void Game::SyncMusicAndBeatManager(float deltaTime)
{
	_musicBeatManagerSyncTimer += deltaTime;
	if (_musicBeatManagerSyncTimer >= MUSIC_WITH_BEAT_MANAGER_SYNC_TIME) {
		_musicBeatManagerSyncTimer -= MUSIC_WITH_BEAT_MANAGER_SYNC_TIME;
		_beatUnitManager.SyncTimingWithMusic(static_cast<float>(_music.getPlayingOffset().asMilliseconds()));
	}
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