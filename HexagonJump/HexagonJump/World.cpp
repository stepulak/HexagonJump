#include "World.hpp"

namespace hexagon {

void World::Draw(sf::RenderWindow& window, const Camera& camera) const
{
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw(window, camera, sf::Color(255, 255, 255, 255));
	}
}

}