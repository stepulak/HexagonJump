#include "Spike.hpp"
#include "Player.hpp"

namespace hexagon {

const sf::Color Spike::BORDER_COLOR = { 255,255,255 };

Spike::Spike(float x, 
	float y, 
	float width, 
	float maximumHeight,
	Direction direction,
	const BeatUnit& beatUnit)
	: Obstacle(Obstacle::Type::SPIKE)
	, _position(x, y)
	, _width(width)
	, _maxHeight(maximumHeight)
	, _direction(direction)
	, _beatUnit(beatUnit)
{
}

bool Spike::InCollision(const Player& player) const
{
	return CheckCollisionWithPlayer(player.GetPosition(), player.GetRadius());
}

float Spike::SaveDistanceToTravel(const Player& player,
	float wantedDistance, 
	Direction direction) const
{
	// Don't count distance, just the collision
	// Player explode anyway if collision occurs
	auto moved = MoveVectorInDirection(player.GetPosition(), wantedDistance, direction);
	return CheckCollisionWithPlayer(moved, player.GetRadius()) ? 0.f : wantedDistance;
}

void Spike::Draw(sf::RenderWindow& window, 
	const Camera& camera, 
	const sf::Color& color) const
{
	auto body = GetSpikeBody();
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

bool Spike::CheckCollisionWithPlayer(const sf::Vector2f& playerPosition, float playerRadius) const
{
	//auto playerBody = CreateRectangleFromCircleBody(position, radius * 0.866f);
	auto body = GetSpikeBody();
	
	//
	// TODO!!
	//

	for (size_t i = 0u; i < body.getPointCount(); i++) {
		auto spikeVertex = body.getPoint(i) + _position;
		auto difference = spikeVertex - playerPosition;
		if (std::sqrt(difference.x*difference.x + difference.y*difference.y) < playerRadius) {
			return true;
		}
		/*if (playerBody.contains(body.getPoint(i) + _position)) {
			return true;
		}*/
	}
	return false;
}


}