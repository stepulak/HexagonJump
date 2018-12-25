#include "Spike.hpp"
#include "Player.hpp"

namespace hexagon {

Spike::Spike(float x, float y, float width, float maximumHeight, Direction direction, const BeatUnit& beatUnit)
	: Obstacle(Obstacle::Type::SPIKE)
	, _position(x, y)
	, _width(width)
	, _maxHeight(maximumHeight)
	, _direction(direction)
	, _beatUnit(beatUnit)
{
}

void Spike::Move(float horizontalDistance)
{
	_position.x += horizontalDistance;
}

bool Spike::InCollision(const Player& player) const
{
	return CheckCollisionWithPlayer(player.GetPosition(), player.GetRadius());
}

float Spike::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	auto playerMoved = MoveVectorInDirection(player.GetPosition(), wantedDistance, direction);
	return CheckCollisionWithPlayer(playerMoved, player.GetRadius()) ? 0.f : wantedDistance;
}

void Spike::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	auto body = CountSpikeBody();
	body.setFillColor(color);
	body.setPosition(_position - sf::Vector2f(camera.GetPosition(), 0.f));
	window.draw(body);
}

bool Spike::CheckCollisionWithPlayer(const sf::Vector2f& position, float radius) const
{
	auto playerBody = CreateRectangleFromCircleBody(position, radius * 0.866f);
	auto body = CountSpikeBody();
	
	for (size_t i = 0u; i < body.getPointCount(); i++) {
		if (playerBody.contains(body.getPoint(i) + _position)) {
			return true;
		}
	}
	return false;
}

sf::ConvexShape Spike::CountSpikeBody() const
{
	return CountTriangleCoords(_width, _maxHeight * _beatUnit.Height(), _direction);
}

}