#pragma once

#include "BackgroundStripeManager.hpp"
#include "BeatUnitManager.hpp"
#include "ColorPaletteChanger.hpp"
#include "ObstacleManager.hpp"
#include "ParticleSystem.hpp"
#include "Player.hpp"
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

	static constexpr float POSITION_CUT_RATIO = 10.f;
	static constexpr float PLAYER_SPAWN_POS_X = 200.f;
	static constexpr float PLAYER_SPAWN_POS_Y = 400.f;
	static constexpr float COLOR_CHANGE_TIME = 10.f;
	static constexpr float COLOR_CHANGE_HIGHEST_BEAT_MULTIPLIER = 10.f;
	static const sf::Color PLAYER_COLOR;

	static constexpr float GetSurfaceWidth() { return 1440.f; }
	static constexpr float GetSurfaceHeight() { return 54.f; }

	void ExtendSurface();
	void TryToCutPositionAllElements();
	void TryToSpawnAnotherObstacleSet();

	void DrawBeatFlash(sf::RenderWindow& window) const;
	void DrawBackground(sf::RenderWindow& window) const;
	void DrawForeground(sf::RenderWindow& window) const;

	Camera& _camera;
	BeatUnitManager& _beatUnitManager;
	BackgroundStripeManager _backgroundStripeManager;
	ObstacleManager _obstacleManager;
	ParticleSystem _particleSystem;
	WorldSetCreator _worldSetCreator;
	Player _player{ PLAYER_SPAWN_POS_X, PLAYER_SPAWN_POS_Y };
	ColorPaletteChanger _colorPaletteChanger{ COLOR_CHANGE_TIME };
	float _surfaceEnd = 0.f;
};

}