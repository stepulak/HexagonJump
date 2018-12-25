#include "Platform.hpp"
#include "Player.hpp"

namespace hexagon {

Platform::Platform(float leftX, float topY, float width, float height)
	: Obstacle(Obstacle::Type::PLATFORM)
	, _rectangle(leftX, topY, width, height)
{
}

void Platform::Move(float horizontalDistance)
{
	_rectangle.left += horizontalDistance;
}

bool Platform::InCollision(const Player& player) const
{
	return CreateRectangleFromCircleBody(player.GetPosition(), player.GetRadius()).intersects(_rectangle);
}

float Platform::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	auto playerBody = CreateRectangleFromCircleBody(player.GetPosition(), player.GetRadius());
	return RectangleDistanceAfterMovement(playerBody, _rectangle, wantedDistance, direction);
}

void Platform::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	auto platformBody = sf::RectangleShape(GetSize());
	platformBody.setPosition(GetPosition() - sf::Vector2f(camera.GetPosition(), 0.f));
	platformBody.setFillColor(color);
	window.draw(platformBody);
}

}