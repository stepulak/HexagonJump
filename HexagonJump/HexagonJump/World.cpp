#include "World.hpp"

namespace hexagon {

void World::Draw(const Camera& camera) const
{
	for (const auto& obstacle : _obstacles) {
		obstacle->Draw
	}
}

}