#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

namespace hexagon {

World::World(Camera& camera, BeatUnitManager& manager)
	: _camera(camera)
	, _beatUnitManager(manager)
{
	CreateSurface();
}

void World::Update(float deltaTime, bool skipObstacles)
{
	_player.Update(deltaTime, GRAVITY, *this);
	_particleSystem.Update(deltaTime);
	_backgroundStripeManager.Update(_camera, deltaTime);
	_obstacleManager.RemoveObstaclesPassedCamera(_camera);

	auto skipTime = _beatUnitManager.CurrentHighestBeatRatio() * COLOR_CHANGE_HIGHEST_BEAT_RATIO;
	_colorPaletteChanger.Update(deltaTime, skipTime);

	UpdateObstaclesAndSurface(deltaTime, skipObstacles);
	TryToCutPositionAllElements();
}

void World::Draw(sf::RenderWindow& window) const
{
	DrawBackground(window);
	DrawForeground(window);
	DrawBeatFlash(window);
}

void World::TryToCutPositionAllElements()
{
	auto cutOffset = _camera.GetVirtualWidth() * POSITION_CUT_RATIO;
	
	if (_camera.GetPosition() < cutOffset) {
		return; // no cut
	}
	_camera.Move(-cutOffset);
	_obstacleSetEnd -= cutOffset;
	_backgroundStripeManager.Move(-cutOffset);
	_obstacleManager.Move(-cutOffset);
	_player.CutPosition(-cutOffset);
}

void World::UpdateObstaclesAndSurface(float deltaTime, bool skipObstacles)
{
	_surface->get().Move(deltaTime * _camera.GetVelocity());

	if (_obstacleSetEnd - _camera.GetPosition() < SURFACE_WIDTH) {
		_obstacleSetEnd += SURFACE_WIDTH;
		if (!skipObstacles) {
			SpawnAnotherObstacleSet();
		}
	}
}

void World::SpawnAnotherObstacleSet()
{
	auto setArea = sf::FloatRect(_obstacleSetEnd, 0.f,
		SURFACE_WIDTH,
		_camera.GetVirtualHeight() - SURFACE_HEIGHT);

	_worldSetCreator.SpawnRandomSet(*this, setArea);
}

void World::CreateSurface()
{
	auto surface = std::make_unique<Platform>(0.f,
		_camera.GetVirtualHeight() - SURFACE_HEIGHT,
		SURFACE_WIDTH,
		SURFACE_HEIGHT);

	_surface = dynamic_cast<Platform&>(*_obstacleManager.GetObstaclePool().Add(std::move(surface)));
}

void World::DrawBeatFlash(sf::RenderWindow& window) const
{
	auto colorRatio = _beatUnitManager.CurrentHighestBeatRatio();
	auto color = _colorPaletteChanger.GetActiveColor(ColorEntity::FLASH);
	auto cameraSize = _camera.GetVirtualProportions();

	color.r = static_cast<uint8_t>(color.r * colorRatio);
	color.g = static_cast<uint8_t>(color.g * colorRatio);
	color.b = static_cast<uint8_t>(color.b * colorRatio);

	DrawRectangle(window, { 0.f, 0.f, cameraSize.x, cameraSize.y }, color);
}

void World::DrawBackground(sf::RenderWindow& window) const
{
	auto cameraSize = _camera.GetVirtualProportions();
	auto color = _colorPaletteChanger.GetActiveColor(ColorEntity::BACKGROUND);

	DrawRectangle(window, { 0.f, 0.f, cameraSize.x, cameraSize.y }, color);

	_backgroundStripeManager.Draw(window,
		_camera, 
		_colorPaletteChanger.GetActiveColor(ColorEntity::STRIPE));
}

void World::DrawForeground(sf::RenderWindow& window) const
{
	_obstacleManager.Draw(window, 
		_camera, 
		_colorPaletteChanger.GetActiveColor(ColorEntity::OBSTACLE));
	_player.Draw(window, _camera);
	_particleSystem.Draw(window, _camera);
}

}