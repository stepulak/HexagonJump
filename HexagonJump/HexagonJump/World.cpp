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
	_backgroundColor = sf::Color(Random(0, 255), Random(0, 255), Random(0, 255));
}

void World::Update(float deltaTime)
{
	_camera.Move(_camera.GetSpeed() * deltaTime);
	_player.Update(deltaTime, GRAVITY, *this);
	_particleSystem.Update(deltaTime);
	_backgroundStripeManager.Update(_camera, deltaTime);
	
	if (ShouldSpawnAnotherObstacleSet()) {
		auto setArea = sf::FloatRect(_surfaceEnd, 0.f, GetSurfaceWidth(), _camera.GetVirtualHeight() - GetSurfaceHeight());
		_worldSetCreator.CreateRandomSet(*this, setArea);
		ExtendSurface();
	}
	TryToCutPositionAllElements();
	RemoveObstaclesPassedCamera();
	ProcessBackgroundColorChange(deltaTime);
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
	
	for (auto& obstacle : _obstacles) {
		obstacle->Move(-cutOffset);
	}
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
		_backgroundColorChangeTimer += _beatUnitManager.CurrentHighestBeatRatio() * deltaTime;

		if (_backgroundColorChangeTimer >= BACKGROUND_COLOR_CHANGE_TIME) {
			_backgroundColorChangeTimer = 0.f;
			_nextBackgroundColor = sf::Color(Random(0, 255), Random(0, 255), Random(0, 255));
		}
	}
}

void World::DrawBeatFlash(sf::RenderWindow& window) const
{
}

void World::DrawBackground(sf::RenderWindow& window) const
{
	sf::RectangleShape background(_camera.GetVirtualProportions());

	if (_nextBackgroundColor) {
		background.setFillColor(MixColors(_nextBackgroundColor.value(), _backgroundColor, _nextBackgroundColorRatio));
	}
	else {
		background.setFillColor(_backgroundColor);
	}
	window.draw(background);

	_backgroundStripeManager.Draw(window, _camera, sf::Color(255, 255, 255, 64));
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