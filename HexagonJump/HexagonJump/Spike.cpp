#include "Spike.hpp"
#include "Player.hpp"

namespace hexagon {

Spike::Spike(float width, float maximumHeight, Direction direction, const BeatUnit& beatUnit)
	: Obstacle(Obstacle::Type::SPIKE)
	, _width(width)
	, _maxHeight(maximumHeight)
	, _direction(direction)
	, _beatUnit(beatUnit)
{
}

bool Spike::InCollision(const Player& player) const
{
	return CheckCollisionWithCircle(player.GetPosition(), player.GetRadius());
}

float Spike::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	auto playerMoved = MoveVectorInDirection(player.GetPosition(), wantedDistance, direction);
	return CheckCollisionWithCircle(playerMoved, player.GetRadius()) ? 0.f : wantedDistance;
}

void Spike::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	auto spikeBody = CountTriangleCoords(_width, _maxHeight * _beatUnit.Height(), _direction);
	spikeBody.setFillColor(color);
	window.draw(spikeBody);
}

bool Spike::CheckCollisionWithCircle(const sf::Vector2f& position, float radius) const
{
	auto circleBody = CreateRectangleFromCircleBody(position, radius);
	auto spikeBody = CountTriangleCoords(_width, _maxHeight * _beatUnit.Height(), _direction);

	for (size_t i = 0u; i < spikeBody.getPointCount(); i++) {
		if (circleBody.contains(spikeBody.getPoint(i))) {
			return true;
		}
	}
	return false;
}

}