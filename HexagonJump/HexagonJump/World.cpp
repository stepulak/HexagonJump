#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

#include <iostream>

namespace hexagon {

World::World(Camera& camera, BeatUnitManager& manager)
	: _camera(camera)
	, _beatUnitManager(manager)
	, _player(camera.GetVirtualWidth() * PLAYER_SPAWN_POS_X_RATIO, camera.GetVirtualHeight() * PLAYER_SPAWN_POS_Y_RATIO)
{
	ExtendSurface();
	_nextBackgroundColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

void World::Update(float deltaTime)
{
	_camera.Move(_camera.GetSpeed() * deltaTime);
	_player.Update(deltaTime, GRAVITY, *this);
	_particleSystem.Update(deltaTime);

	if (ShouldSpawnAnotherObstacleSet()) {
		auto setArea = sf::FloatRect(_surfaceEnd, 0.f, GetSurfaceWidth(), _camera.GetVirtualHeight() - GetSurfaceHeight());
		_worldSetCreator.CreateRandomSet(*this, setArea);
		ExtendSurface();
	}
	TryToCutObstaclesPosition();
	RemoveObstaclesPassedCamera();
}

void World::Draw(sf::RenderWindow& window) const
{
	DrawBackground(window);
	DrawForeground(window);
}

void World::ExtendSurface()
{
	auto surfaceWidth = GetSurfaceWidth();
	auto surfaceHeight = GetSurfaceHeight();
	AddObstacle(std::make_unique<Platform>(_surfaceEnd, _camera.GetVirtualHeight() - surfaceHeight, surfaceWidth, surfaceHeight));
	_surfaceEnd += surfaceWidth;
}

void World::TryToCutObstaclesPosition()
{
	auto cutOffset = _camera.GetVirtualWidth() * 2.f;

	if (_camera.GetPosition() < cutOffset) {
		return;
	}
	_camera.Move(-cutOffset);
	_surfaceEnd -= cutOffset;
	
	for (auto& obstacle : _obstacles) {
		obstacle->Move(-cutOffset);
	}
	_player.Move(-cutOffset, 0.f);
}

void World::RemoveObstaclesPassedCamera()
{
	_obstacles.erase(std::remove_if(_obstacles.begin(), _obstacles.end(), [&](auto& obstacle) {
		return obstacle->PassedCamera(_camera);
	}), _obstacles.end());
}

void World::ProcessBackgroundColorChange(float deltaTime)
{
	if (_nextBackgroundColor) {
		_nextBackgroundColorRatio += deltaTime;
		if (_nextBackgroundColorRatio >= 1.f) {
			_nextBackgroundColorRatio = 0.f;
			_backgroundColor = _nextBackgroundColor.value();
			_nextBackgroundColor.reset();
		}
	}
	else {
		_backgroundColorChangeTimer += deltaTime;

		// TODO BEAT

		if (_backgroundColorChangeTimer >= BACKGROUND_COLOR_CHANGE_TIME) {
			_backgroundColorChangeTimer = 0.f;
			_nextBackgroundColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
		}
	}
}

void World::DrawBackground(sf::RenderWindow& window) const
{
	sf::RectangleShape background(_camera.GetVirtualProportions());
	background.setFillColor(MixColors(_nextBackgroundColor.value(), _backgroundColor, _nextBackgroundColorRatio));
	window.draw(background);
}

void World::DrawForeground(sf::RenderWindow& window) const
{
	_particleSystem.Draw(window, _camera);
	auto obstaclesAndPlayerColor = sf::Color(255, 255, 255, 255);
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw(window, _camera, obstaclesAndPlayerColor);
	}
	_player.Draw(window, _camera, obstaclesAndPlayerColor);
}

}