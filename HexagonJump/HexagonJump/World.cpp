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
	_particleSystem.Draw(window, _camera);
	auto obstaclesAndPlayerColor = sf::Color(255, 255, 255, 255);
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw(window, _camera, obstaclesAndPlayerColor);
	}
	_player.Draw(window, _camera, obstaclesAndPlayerColor);
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

}