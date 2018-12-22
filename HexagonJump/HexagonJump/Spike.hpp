#pragma once

#include "Obstacle.hpp"
#include "BeatUnit.hpp"

namespace hexagon {

class Spike : public Obstacle {
public:

	Spike(float width, float maximumHeight, Direction topSpikeDirection, const BeatUnit& beatUnit);

	bool InCollision(const Player& player) const override;
	float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const override;

private:

	float _width;
	float _maxHeight;
	Direction _topSpikeDirection;
	const BeatUnit& _beatUnit;
};

}