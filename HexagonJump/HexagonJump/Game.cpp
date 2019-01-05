#include "Game.hpp"

#include <cmath>
#include <iostream>

namespace hexagon {

Game::Game(sf::Music& music, MusicVisualization&& visualization, size_t numBeatUnits, Camera& camera)
	: _music(music)
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
	auto& player = _world->GetPlayer();

	switch (key)
	{
	case PLAYER_KEY_JUMP:
		player.TryToJump();
		break;
	case PLAYER_KEY_FALL_DOWN_FAST:
		player.TryToFallDownFast();
		break;
	case PLAYER_KEY_MOVE_LEFT:
		//player.StopMoving();
		//player.StartMoving(camera.GetSpeed() * 2.f, false);
		break;
	case PLAYER_KEY_MOVE_RIGHT:
		//player.StopMoving();
		//player.StartMoving(camera.GetSpeed() * 2.f, true);
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

void Game::KeyReleased(sf::Keyboard::Key key)
{
	auto& player = _world->GetPlayer();

	switch (key)
	{
	case PLAYER_KEY_MOVE_LEFT:
	case PLAYER_KEY_MOVE_RIGHT:
		//player.StopMoving();
	default:
		break;
	}
}

void Game::Update(float deltaTime)
{
	if (_stopped) {
		return; // skip
	}
	if (_world->PlayerDied()) {
		ResolvePlayerDeath(deltaTime);
	}
	else {
		MoveCameraAndPlayer(deltaTime);
	}
	_world->Update(deltaTime, false);
	_beatUnitManager.Update(deltaTime);
	SyncMusicAndBeatManager(deltaTime);
}

void Game::Draw(sf::RenderWindow& window) const
{
	_world->Draw(window);
}

void Game::Reset()
{
	_music.stop();
	_musicBeatManagerSyncTimer = 0.f;
	_playerDeathWaitTimer = 0.f;
	_world.reset();
	_world = std::make_unique<World>(_camera, _beatUnitManager);
	_camera.SetPosition(0.f);
	_beatUnitManager.Reset();
	Start();
}

void Game::ResolvePlayerDeath(float deltaTime)
{
	_playerDeathWaitTimer += deltaTime;
	if (_playerDeathWaitTimer >= PLAYER_DEATH_WAIT_TIME) {
		Reset();
	}
}

void Game::MoveCameraAndPlayer(float deltaTime)
{
	auto cameraVelocity = _camera.GetVelocity();
	_camera.Move(cameraVelocity * deltaTime);
	_world->GetPlayer().StartMoving(cameraVelocity, true);
}

void Game::SyncMusicAndBeatManager(float deltaTime)
{
	_musicBeatManagerSyncTimer += deltaTime;
	if (_musicBeatManagerSyncTimer >= MUSIC_WITH_BEAT_MANAGER_SYNC_TIME) {
		_musicBeatManagerSyncTimer -= MUSIC_WITH_BEAT_MANAGER_SYNC_TIME;
		_beatUnitManager.SyncTimingWithMusic(static_cast<float>(_music.getPlayingOffset().asMilliseconds()));
	}
}

}