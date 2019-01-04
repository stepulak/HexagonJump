#include "Game.hpp"

#include <cmath>
#include <iostream>

namespace hexagon {

Game::Game(sf::Music& music, MusicVisualization&& visualization, size_t numBeatUnits, Camera& camera)
	: _music(music)
	, _beatUnitManager(std::move(visualization), numBeatUnits, TIMERATE)
	, _world(camera, _beatUnitManager)
{
	camera.SetVelocity(CAMERA_VELOCITY);
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
	auto& player = _world.GetPlayer();
	auto& camera = _world.GetCamera();

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
	auto& player = _world.GetPlayer();

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
	// TODO simplify
	auto& player = _world.GetPlayer();
	auto& camera = _world.GetCamera();
	player.StartMoving(camera.GetVelocity(), true);

	_world.Update(deltaTime);
	_beatUnitManager.Update(deltaTime);
	SyncMusicAndBeatManager(deltaTime);
}

void Game::Draw(sf::RenderWindow& window) const
{
	_world.Draw(window);
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