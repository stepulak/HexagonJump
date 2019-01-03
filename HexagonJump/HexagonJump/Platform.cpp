#include "Platform.hpp"
#include "Player.hpp"

namespace hexagon {

Platform::Platform(float leftX, float topY, float width, float height)
	: Obstacle(Obstacle::Type::PLATFORM)
	, _area(leftX, topY, width, height)
{
}

void Platform::Move(float horizontalDistance)
{
	_area.left += horizontalDistance;
}

bool Platform::PassedCamera(const Camera& camera) const
{
	return _area.left + _area.width - camera.GetPosition() <= 0.f;
}

bool Platform::InCollision(const Player& player) const
{
	return CreateRectangleFromCircleBody(player.GetPosition(), player.GetRadius()).intersects(_area);
}

float Platform::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	auto playerBody = CreateRectangleFromCircleBody(player.GetPosition(), player.GetRadius());
	return RectangleDistanceAfterMovement(playerBody, _area, wantedDistance, direction);
}

void Platform::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	auto x = _area.left - camera.GetPosition();
	DrawRectangle(window, { x, _area.top, _area.width, _area.height }, color);
}

}