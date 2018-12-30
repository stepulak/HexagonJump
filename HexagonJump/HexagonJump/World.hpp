#pragma once

#include "BeatUnitManager.hpp"
#include "BackgroundStripeManager.hpp"
#include "Obstacle.hpp"
#include "ParticleSystem.hpp"
#include "Player.hpp"
#include "WorldSetCreator.hpp"

#include <vector>

namespace hexagon {

class World {
public:

	using ObstacleContainer = std::vector<Obstacle::Ptr>;
	
	static constexpr float GRAVITY = 1962.f;

	World(Camera& camera, BeatUnitManager& manager);

	void AddObstacle(Obstacle::Ptr obstacle) { _obstacles.emplace_back(std::move(obstacle)); }
	const ObstacleContainer& GetObstacles() const { return _obstacles; }

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
	static constexpr float PLAYER_SPAWN_POS_X_RATIO = 0.2f;
	static constexpr float PLAYER_SPAWN_POS_Y_RATIO = 0.5f;
	static constexpr float BACKGROUND_COLOR_CHANGE_TIME = 10.f;

	float GetSurfaceWidth() const { return _camera.GetVirtualWidth() * SURFACE_WIDTH_RATIO; }
	float GetSurfaceHeight() const { return _camera.GetVirtualHeight() * SURFACE_HEIGHT_RATIO; }
	bool ShouldSpawnAnotherObstacleSet() const { return _surfaceEnd - _camera.GetPosition() < GetSurfaceWidth(); }

	void ExtendSurface();
	void TryToCutPositionAllElements();
	void RemoveObstaclesPassedCamera();
	void ProcessBackgroundColorChange(float deltaTime);

	void DrawBeatFlash(sf::RenderWindow& window) const;
	void DrawBackground(sf::RenderWindow& window) const;
	void DrawForeground(sf::RenderWindow& window) const;

	Camera& _camera;
	BeatUnitManager& _beatUnitManager;
	BackgroundStripeManager _backgroundStripeManager;
	ObstacleContainer _obstacles;
	ParticleSystem _particleSystem;
	Player _player;
	WorldSetCreator _worldSetCreator;
	float _surfaceEnd = 0.f;
	sf::Color _backgroundColor;
	std::optional<sf::Color> _nextBackgroundColor;
	float _backgroundColorChangeTimer = 0.f;
	float _nextBackgroundColorRatio = 0.f;
};

}