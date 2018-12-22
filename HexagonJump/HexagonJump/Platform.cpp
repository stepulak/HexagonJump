#include "Platform.hpp"
#include "Player.hpp"

namespace hexagon {

Platform::Platform(float leftX, float topY, float width, float height)
	: Obstacle(Obstacle::Type::PLATFORM)
	, _rectangle(leftX, topY, width, height)
{
}

bool Platform::InCollision(const Player& player) const
{
	return RectangleFromCircleBody(player.GetPosition(), player.GetRadius()).intersects(_rectangle);
}

float Platform::SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const
{
	auto playerBody = RectangleFromCircleBody(player.GetPosition(), player.GetRadius());
	return RectangleDistanceAfterMovement(playerBody, _rectangle, wantedDistance, direction);
}

}