#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

#include <iostream>

namespace hexagon {

World::World(Camera& camera)
	: _camera(camera)
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
		ExtendSurface();
		SpawnRandomObstacleSet();
	}
	TryToCutPositionOffset();
}

void World::Draw(sf::RenderWindow& window) const
{
	auto obstaclesAndPlayerColor = sf::Color(255, 255, 255, 255);
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw(window, _camera, obstaclesAndPlayerColor);
	}
	_player.Draw(window, _camera, obstaclesAndPlayerColor);
}

bool World::ShouldSpawnAnotherObstacleSet() const
{
	return _surfaceEnd - _camera.GetPosition() < _camera.GetVirtualWidth() * SURFACE_WIDTH_RATIO;
}

void World::SpawnRandomObstacleSet() const
{
	// TODO
}

void World::ExtendSurface()
{
	auto virtualCameraSize = _camera.GetVirtualProportions();
	auto surfaceWidth = SURFACE_WIDTH_RATIO * virtualCameraSize.x;
	auto surfaceHeight = SURFACE_HEIGHT_RATIO * virtualCameraSize.y;
	AddObstacle(std::make_unique<Platform>(_surfaceEnd, virtualCameraSize.y - surfaceHeight, surfaceWidth, surfaceHeight));
	_surfaceEnd += surfaceWidth;
}

void World::TryToCutPositionOffset()
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

}