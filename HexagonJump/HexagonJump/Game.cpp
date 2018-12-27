#include "Game.hpp"

#include <cmath>

namespace hexagon {

Game::Game(Camera& camera, BeatUnitManager& manager, Difficulty difficulty)
	: _world(camera, manager)
{
	camera.SetSpeed(CameraSpeedAccordingToDifficulty(camera.GetVirtualProportions().x, difficulty));
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
		player.StopMoving();
		player.StartMoving(camera.GetSpeed() * 2.f, false);
		break;
	case PLAYER_KEY_MOVE_RIGHT:
		player.StopMoving();
		player.StartMoving(camera.GetSpeed() * 2.f, true);
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
		player.StopMoving();
	default:
		break;
	}
}

void Game::Update(float deltaTime)
{
	_world.Update(deltaTime);
}

void Game::Draw(sf::RenderWindow& window) const
{
	_world.Draw(window);
}

float Game::CameraSpeedAccordingToDifficulty(float virtualWidth, Difficulty dif)
{
	switch (dif) {
	case Difficulty::EASY:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY;
	case Difficulty::MEDIUM:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * CAMERA_SPEED_DIFFICULTY_MULTIPLIER;
	case Difficulty::HARD:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * std::pow(CAMERA_SPEED_DIFFICULTY_MULTIPLIER, 2);
	case Difficulty::PRO:
		return virtualWidth * CAMERA_SPEED_LOWEST_DIFFICULTY * std::pow(CAMERA_SPEED_DIFFICULTY_MULTIPLIER, 3);
	}
	return 0.f;
}

}