#pragma once

#include "Obstacle.hpp"

namespace hexagon {

class Platform : public Obstacle {
public:

	Platform(float leftX, float topY, float width, float height);

	bool InCollision(const Player& player) const override;
	float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const override;

private:

	sf::FloatRect _rectangle;
};

}