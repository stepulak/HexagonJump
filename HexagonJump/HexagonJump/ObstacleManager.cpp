#include "ObstacleManager.hpp"

namespace hexagon {

void ObstacleManager::Move(float horizontalDist)
{
	_pool.Iterate([&](auto& obstacle) { obstacle->Move(horizontalDist); });
}

void ObstacleManager::RemoveObstaclesPassedCamera(const Camera& camera)
{
	_pool.Iterate([&](auto& obstacle) { return obstacle->PassedCamera(camera); });
}

void ObstacleManager::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	_pool.Iterate([&](const auto& obstacle) { obstacle->Draw(window, camera, color); });
}

}