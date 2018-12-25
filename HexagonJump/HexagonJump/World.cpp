#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

namespace hexagon {


World::World(Camera& camera)
	: _camera(camera)
{
	ExtendSurface();
}

void World::Update(float deltaTime)
{
	_particleSystem.Update(deltaTime);

	if (ShouldSpawnAnotherObstacleSet()) {
		ExtendSurface();
		SpawnRandomObstacleSet();
	}
	TryToCutPositionOffset();
}

void World::Draw(sf::RenderWindow& window) const
{
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw(window, _camera, sf::Color(255, 255, 255, 255));
	}
}

bool World::ShouldSpawnAnotherObstacleSet() const
{
	return _surfaceEnd - _camera.GetPosition() > _camera.GetVirtualProportions().x * 1.5f;
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
	auto offset = _camera.GetVirtualProportions().x * POSITION_CUT_RATIO;

	if (_surfaceEnd < offset) {
		return;
	}
	_camera.Move(-offset);
	_surfaceEnd -= offset;
	
	for (auto& obstacle : _obstacles) {
		obstacle->Move(-offset);
	}
}

}