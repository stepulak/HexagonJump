#include "Spike.hpp"
#include "Player.hpp"

namespace hexagon {

Spike::Spike(float width, float maximumHeight, Direction topSpikeDirection, const BeatUnit& beatUnit)
	: Obstacle(Obstacle::Type::SPIKE)
	, _width(width)
	, _maxHeight(maximumHeight)
	, _topSpikeDirection(topSpikeDirection)
	, _beatUnit(beatUnit)
{
}

bool Spike::InCollision(const Player& player) const
{
	auto playerBody = RectangleFromCircleBody(player.GetPosition(), player.GetRadius());
	auto spike = CountTriangleCoords(_width, _maxHeight * _beatUnit.Height(), _topSpikeDirection);
	for (size_t i = 0u; i < spike.getPointCount(); i++) {
		if (playerBody.contains(spike.getPoint(i))) {
			return true;
		}
	}
	return false;
}

float Spike::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	return 0.f;
}

}