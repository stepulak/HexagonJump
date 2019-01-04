#pragma once

#include "BeatUnitManager.hpp"
#include "BackgroundStripeManager.hpp"
#include "ObstacleManager.hpp"
#include "ParticleSystem.hpp"
#include "Player.hpp"
#include "WorldColorPalette.hpp"
#include "WorldSetCreator.hpp"

#include <vector>

namespace hexagon {

class World {
public:
	
	static constexpr float GRAVITY = 2.f * 1962.f;

	World(Camera& camera, BeatUnitManager& manager);

	ObstacleManager& GetObstacleManager() { return _obstacleManager; }
	const ObstacleManager& GetObstacleManager() const { return _obstacleManager; }

	ParticleSystem& GetParticleSystem() { return _particleSystem; }
	const ParticleSystem& GetParticleSystem() const { return _particleSystem; }

	Player& GetPlayer() { return _player; }
	const Player& GetPlayer() const { return _player; }

	Camera& GetCamera() { return _camera; }
	const Camera& GetCamera() const { return _camera; }

	BeatUnitManager& GetBeatUnitManager() { return _beatUnitManager; }
	const BeatUnitManager& GetBeatUnitManager() const { return _beatUnitManager; }

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	static constexpr float SURFACE_HEIGHT_RATIO = 0.1f;
	static constexpr float SURFACE_WIDTH_RATIO = 1.5f;
	static constexpr float POSITION_CUT_RATIO = 10.f;
	static constexpr float PLAYER_SPAWN_POS_X = 200.f;
	static constexpr float PLAYER_SPAWN_POS_Y = 400.f;
	static constexpr float COLOR_PALETTE_CHANGE_TIME = 10.f;
	static constexpr float COLOR_CHANGE_TIME_BEAT_MULTIPLIER = 10.f;
	static const sf::Color PLAYER_COLOR;

	float GetSurfaceWidth() const { return _camera.GetVirtualWidth() * SURFACE_WIDTH_RATIO; }
	float GetSurfaceHeight() const { return _camera.GetVirtualHeight() * SURFACE_HEIGHT_RATIO; }
	bool ShouldSpawnAnotherObstacleSet() const { return _surfaceEnd - _camera.GetPosition() < GetSurfaceWidth(); }

	sf::Color GetActualColor(ColorEntity entity) const;

	void ExtendSurface();
	void TryToCutPositionAllElements();
	void ProcessColorPaletteChange(float deltaTime);

	void DrawBeatFlash(sf::RenderWindow& window) const;
	void DrawBackground(sf::RenderWindow& window) const;
	void DrawForeground(sf::RenderWindow& window) const;

	Camera& _camera;
	BeatUnitManager& _beatUnitManager;
	BackgroundStripeManager _backgroundStripeManager;
	ObstacleManager _obstacleManager;
	ParticleSystem _particleSystem;
	Player _player;
	WorldSetCreator _worldSetCreator;
	float _surfaceEnd = 0.f;
	ColorPalette _colorPalette;
	std::optional<ColorPalette> _nextColorPalette;
	float _nextColorPaletteTimer = 0.f;
	float _nextColorPaletteRatio = 0.f;
};

}