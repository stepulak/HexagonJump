#pragma once

#include "Obstacle.hpp"

namespace hexagon {

class Platform : public Obstacle {
public:

	Platform(float leftX, float topY, float width, float height);

	sf::Vector2f GetPosition() const { return sf::Vector2f(_rectangle.left, _rectangle.top); }
	sf::Vector2f GetSize() const { return sf::Vector2f(_rectangle.width, _rectangle.height); }

	bool InCollision(const Player& player) const override;
	float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const override;

private:

	sf::FloatRect _rectangle;
};

}