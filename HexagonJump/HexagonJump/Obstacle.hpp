#pragma once

#include "Utils.hpp"

namespace hexagon {

class Player;

class Obstacle {
public:

	enum class Type {
		PLATFORM,
		SPIKE
	};

	Obstacle(Type type)
		: _type(type) {}

	virtual ~Obstacle() = default;

	Type ObstacleType() const { return _type; }
	
	virtual bool InCollision(const Player& player) const = 0;
	virtual float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const = 0;

private:

	Type _type;
};

}