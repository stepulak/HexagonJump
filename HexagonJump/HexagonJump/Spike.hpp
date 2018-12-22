#pragma once

#include "Obstacle.hpp"
#include "BeatUnit.hpp"

namespace hexagon {

class Spike : public Obstacle {
public:

	Spike(float width, float maximumHeight, Direction direction, const BeatUnit& beatUnit);

	bool InCollision(const Player& player) const override;
	float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const override;

private:

	bool CheckCollisionWithCircle(const sf::Vector2f& position, float radius) const;

	float _width;
	float _maxHeight;
	Direction _direction;
	const BeatUnit& _beatUnit;
};

}