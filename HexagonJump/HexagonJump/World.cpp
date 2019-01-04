#include "World.hpp"
#include "Platform.hpp"
#include "Spike.hpp"

#include <iostream>

namespace hexagon {

const sf::Color World::PLAYER_COLOR = { 255, 255, 255 };

World::World(Camera& camera, BeatUnitManager& manager)
	: _camera(camera)
	, _beatUnitManager(manager)
	, _player(PLAYER_SPAWN_POS_X, PLAYER_SPAWN_POS_Y)
{
	ExtendSurface();
	_colorPalette = GetRandomColorPalette();
}

void World::Update(float deltaTime)
{
	_camera.Move(_camera.GetVelocity() * deltaTime);
	_player.Update(deltaTime, GRAVITY, *this);
	_particleSystem.Update(deltaTime);
	_backgroundStripeManager.Update(_camera, deltaTime);
	_obstacleManager.RemoveObstaclesPassedCamera(_camera);

	if (ShouldSpawnAnotherObstacleSet()) {
		auto setArea = sf::FloatRect(_surfaceEnd, 0.f, GetSurfaceWidth(), _camera.GetVirtualHeight() - GetSurfaceHeight());
		_worldSetCreator.CreateRandomSet(*this, setArea);
		ExtendSurface();
	}
	TryToCutPositionAllElements();
	ProcessColorPaletteChange(deltaTime);
}

void World::Draw(sf::RenderWindow& window) const
{
	DrawBackground(window);
	DrawForeground(window);
	DrawBeatFlash(window);
}

sf::Color World::GetActualColor(ColorEntity entity) const
{
	auto activeColor = GetColor(_colorPalette, entity);
	if (_nextColorPalette) {
		return MixColors(GetColor(_nextColorPalette.value(), entity), activeColor, _nextColorPaletteRatio);
	}
	return activeColor;
}

void World::ExtendSurface()
{
	auto surfaceWidth = GetSurfaceWidth();
	auto surfaceHeight = GetSurfaceHeight();
	_obstacleManager.GetObstaclePool().Add(std::make_unique<Platform>(_surfaceEnd, _camera.GetVirtualHeight() - surfaceHeight, surfaceWidth, surfaceHeight));
	_surfaceEnd += surfaceWidth;
}

void World::TryToCutPositionAllElements()
{
	auto cutOffset = _camera.GetVirtualWidth() * POSITION_CUT_RATIO;
	
	if (_camera.GetPosition() < cutOffset) {
		return; // no cut
	}
	_camera.Move(-cutOffset);
	_surfaceEnd -= cutOffset;
	_player.Move(-cutOffset, 0.f);
	_backgroundStripeManager.Move(-cutOffset);
	_obstacleManager.Move(-cutOffset);
}

void World::ProcessColorPaletteChange(float deltaTime)
{
	if (_nextColorPalette) {
		_nextColorPaletteRatio += deltaTime;
		if (_nextColorPaletteRatio >= 1.f) {
			_nextColorPaletteRatio = 0.f;
			_colorPalette = _nextColorPalette.value();
			_nextColorPalette.reset();
		}
	}
	else {
		_nextColorPaletteTimer += deltaTime;
		_nextColorPaletteTimer += _beatUnitManager.CurrentHighestBeatRatio() * deltaTime;

		if (_nextColorPaletteTimer >= COLOR_PALETTE_CHANGE_TIME) {
			_nextColorPaletteTimer = 0.f;
			_nextColorPalette = GetRandomColorPalette();
		}
	}
}

void World::DrawBeatFlash(sf::RenderWindow& window) const
{
	auto color = GetActualColor(ColorEntity::FLASH);
	auto colorRatio = _beatUnitManager.CurrentHighestBeatRatio();
	color.r *= colorRatio;
	color.g *= colorRatio;
	color.b *= colorRatio;

	sf::RectangleShape flash(_camera.GetVirtualProportions());
	flash.setFillColor(color);
	window.draw(flash);
}

void World::DrawBackground(sf::RenderWindow& window) const
{
	sf::RectangleShape background(_camera.GetVirtualProportions());
	background.setFillColor(GetActualColor(ColorEntity::BACKGROUND));
	window.draw(background);

	_backgroundStripeManager.Draw(window, _camera, GetActualColor(ColorEntity::STRIPE));
}

void World::DrawForeground(sf::RenderWindow& window) const
{
	_particleSystem.Draw(window, _camera);
	_obstacleManager.Draw(window, _camera, GetActualColor(ColorEntity::OBSTACLE));
	_player.Draw(window, _camera, PLAYER_COLOR);
}

}