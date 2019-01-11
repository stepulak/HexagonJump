#include "Spike.hpp"
#include "Player.hpp"

namespace hexagon {

const sf::Color Spike::BORDER_COLOR = { 255,255,255 };

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

bool Spike::PassedCamera(const Camera& camera) const
{
	return _position.x + std::max(_maxHeight, _width / 2.f) - camera.GetPosition() <= 0.f;
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
	auto position = _position - sf::Vector2f(camera.GetPosition(), 0.f);
	body.setFillColor(color);
	body.setPosition(position);
	window.draw(body);
	DrawBorder(window, position, body);
}

void Spike::DrawBorder(sf::RenderWindow& window, const sf::Vector2f& position, const sf::ConvexShape& body) const
{
	sf::Vertex vertices[3] = {
		body.getPoint(0) + position,
		body.getPoint(2) + position,
		body.getPoint(1) + position
	};
	for (auto& vertex : vertices) {
		vertex.color = BORDER_COLOR;
	}
	window.draw(vertices, 3, sf::LinesStrip);
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