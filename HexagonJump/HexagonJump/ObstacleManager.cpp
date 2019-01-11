#include "ObstacleManager.hpp"

namespace hexagon {

void ObstacleManager::Move(float horizontalDist)
{
	for (auto& obstacle : _pool) {
		obstacle->Move(horizontalDist);
	}
}

void ObstacleManager::RemoveObstaclesPassedCamera(const Camera& camera)
{
	_pool.RemoveAll([&](const auto& obstacle) { return obstacle->PassedCamera(camera); });
}

void ObstacleManager::Draw(sf::RenderWindow& window,
	const Camera& camera, 
	const sf::Color& color) const
{
	for (auto& obstacle : _pool) {
		obstacle->Draw(window, camera, color);
	}
}

}