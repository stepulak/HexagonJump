#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

#include <iostream>

namespace hexagon {

const sf::Color World::PLAYER_COLOR = { 255, 255, 255 };

World::World(Camera& camera, BeatUnitManager& manager)
	: _camera(camera)
	, _beatUnitManager(manager)
{
	ExtendSurface();
}

void World::Update(float deltaTime, bool skipObstacles)
{
	_player.Update(deltaTime, GRAVITY, *this);
	_particleSystem.Update(deltaTime);
	_backgroundStripeManager.Update(_camera, deltaTime);
	_obstacleManager.RemoveObstaclesPassedCamera(_camera);

	auto skipTime = _beatUnitManager.CurrentHighestBeatRatio() * COLOR_CHANGE_HIGHEST_BEAT_MULTIPLIER;
	_colorPaletteChanger.Update(deltaTime, skipTime);

	UpdateObstaclesAndSurface(skipObstacles);
	TryToCutPositionAllElements();

	//std::cout << _player.GetPosition().x << " " << _player.GetPosition().y << std::endl;
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
	_surfaceEnd -= cutOffset;
	_backgroundStripeManager.Move(-cutOffset);
	_obstacleManager.Move(-cutOffset);
	_player.CutPosition(-cutOffset);
}

void World::UpdateObstaclesAndSurface(bool skipObstacles)
{
	if (_surfaceEnd - _camera.GetPosition() < GetSurfaceWidth()) {
		ExtendSurface();
		if (!skipObstacles) {
			SpawnAnotherObstacleSet();
		}
	}
}

void World::SpawnAnotherObstacleSet()
{
	auto setArea = sf::FloatRect(_surfaceEnd, 0.f,
		GetSurfaceWidth(),
		_camera.GetVirtualHeight() - GetSurfaceHeight());

	_worldSetCreator.SpawnRandomSet(*this, setArea);
}

void World::ExtendSurface()
{
	auto surfaceWidth = GetSurfaceWidth();
	auto surfaceHeight = GetSurfaceHeight();
	auto surface = std::make_unique<Platform>(_surfaceEnd,
		_camera.GetVirtualHeight() - surfaceHeight,
		surfaceWidth,
		surfaceHeight);

	_obstacleManager.GetObstaclePool().Add(std::move(surface));
	_surfaceEnd += surfaceWidth;
}

void World::DrawBeatFlash(sf::RenderWindow& window) const
{
	auto colorRatio = _beatUnitManager.CurrentHighestBeatRatio();
	auto color = _colorPaletteChanger.GetActiveColor(ColorEntity::FLASH);

	color.r = static_cast<uint8_t>(color.r * colorRatio);
	color.g = static_cast<uint8_t>(color.g * colorRatio);
	color.b = static_cast<uint8_t>(color.b * colorRatio);

	auto cameraSize = _camera.GetVirtualProportions();

	DrawRectangle(window, { 0.f, 0.f, cameraSize.x, cameraSize.y }, color);
}

void World::DrawBackground(sf::RenderWindow& window) const
{
	auto cameraSize = _camera.GetVirtualProportions();
	auto color = _colorPaletteChanger.GetActiveColor(ColorEntity::BACKGROUND);

	DrawRectangle(window, { 0.f, 0.f, cameraSize.x, cameraSize.y }, color);
	_backgroundStripeManager.Draw(window, _camera, _colorPaletteChanger.GetActiveColor(ColorEntity::STRIPE));
}

void World::DrawForeground(sf::RenderWindow& window) const
{
	_particleSystem.Draw(window, _camera);
	_obstacleManager.Draw(window, _camera, _colorPaletteChanger.GetActiveColor(ColorEntity::OBSTACLE));
	_player.Draw(window, _camera, PLAYER_COLOR);
}

}