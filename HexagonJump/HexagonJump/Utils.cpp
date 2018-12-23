#include "Utils.hpp"

namespace hexagon {

namespace {

inline bool PointInLine(float point, float lineBegin, float lineSize)
{
	return point >= lineBegin && point <= lineBegin + lineSize;
}

bool CanCollide(const sf::FloatRect& rect1, const sf::FloatRect& rect2, Direction direction)
{
	if (direction == Direction::DOWN || direction == Direction::UP) {
		return PointInLine(rect1.left + 1, rect2.left, rect2.width) ||
			PointInLine(rect1.left + rect1.width - 1, rect2.left, rect2.width) ||
			PointInLine(rect2.left, rect1.left, rect1.width);
	}
	else { // LEFT, RIGHT
		return PointInLine(rect1.top + 1, rect2.top, rect2.height) ||
			PointInLine(rect1.top + rect1.height - 1, rect2.top, rect2.height) ||
			PointInLine(rect2.top, rect1.top, rect1.height);
	}
}

sf::Vector2f DirectionToVector(Direction direction)
{
	switch (direction)
	{
	case Direction::UP:
		return { 0.f, -1.f };
	case Direction::DOWN:
		return { 0.f, 1.f };
	case Direction::LEFT:
		return { -1.f, 0.f };
	case Direction::RIGHT:
		return { 1.f, 0.f };
	}
	return {};
}

} // namespace

sf::ConvexShape CountTriangleCoords(float width, float height, Direction topVertexDirection)
{
	auto[dx, dy] = DirectionToVector(topVertexDirection);
	sf::ConvexShape shape(3);

	shape.setPoint(0, sf::Vector2f(-dy * width / 2, -dx * width / 2));
	shape.setPoint(1, sf::Vector2f(dy * width / 2, dx * width / 2));
	shape.setPoint(2, sf::Vector2f(dx * height, dy * height));
	
	return shape;
}

float RectangleAbsoluteXDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	return std::abs(rect1.left + rect1.width / 2 - (rect2.left + rect2.width / 2)) - rect1.width / 2 - rect2.width / 2;
}

float RectangleAbsoluteYDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	return std::abs(rect1.top + rect1.height / 2 - (rect2.top + rect2.height / 2)) - rect1.height / 2 - rect2.height / 2;
}

float RectangleDistanceAfterMovement(sf::FloatRect dynamicRect, const sf::FloatRect& staticRect, float distance, Direction direction)
{
	if (!CanCollide(dynamicRect, staticRect, direction)) {
		return distance;
	}

	float returnDistance = 0.f;
	auto moved = MoveVectorInDirection(sf::Vector2f(dynamicRect.left, dynamicRect.top), distance, direction);

	dynamicRect.left = moved.x;
	dynamicRect.top = moved.y;

	if (direction == Direction::DOWN || direction == Direction::UP) {
		if (dynamicRect.intersects(staticRect)) {
			returnDistance = RectangleAbsoluteYDistance(dynamicRect, staticRect);
		}
	}
	else { // LEFT, RIGHT
		if (dynamicRect.intersects(staticRect)) {
			returnDistance = RectangleAbsoluteXDistance(dynamicRect, staticRect);
		}
	}

	return distance + returnDistance;
}

sf::Vector2f MoveVectorInDirection(sf::Vector2f vec, float distance, Direction direction)
{
	switch (direction) {
	case Direction::DOWN:
		vec.y += distance;
		break;
	case Direction::UP:
		vec.y -= distance;
		break;
	case Direction::LEFT:
		vec.x -= distance;
		break;
	case Direction::RIGHT:
		vec.x += distance;
		break;
	}
	return vec;
}

int Random(int from, int to)
{
	static std::random_device rd;
	static std::mt19937 eng(rd());
	return std::uniform_int_distribution<>(from, to)(eng);
}

}