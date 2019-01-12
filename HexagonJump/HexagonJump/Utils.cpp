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
	return PointInLine(rect1.top + 1, rect2.top, rect2.height) ||
		PointInLine(rect1.top + rect1.height - 1, rect2.top, rect2.height) ||
		PointInLine(rect2.top, rect1.top, rect1.height);
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

float RectangleAbsoluteXDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	auto minDistance = rect1.width / 2 + rect2.width / 2 + 1;
	return std::abs(rect1.left + rect1.width / 2 - (rect2.left + rect2.width / 2)) - minDistance;
}

float RectangleAbsoluteYDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	auto minDistance = rect1.height / 2 + rect2.height / 2 + 1;
	return std::abs(rect1.top + rect1.height / 2 - (rect2.top + rect2.height / 2)) - minDistance;
}

} // namespace

sf::ConvexShape CountTriangle(float base, float height, Direction heightDirection)
{
	auto[dx, dy] = DirectionToVector(heightDirection);
	sf::ConvexShape shape(3);

	shape.setPoint(0, sf::Vector2f(-dy * base / 2, -dx * base / 2));
	shape.setPoint(1, sf::Vector2f(dy * base / 2, dx * base / 2));
	shape.setPoint(2, sf::Vector2f(dx * height, dy * height));
	
	return shape;
}

sf::Vector2f MoveVectorInDirection(const sf::Vector2f& vec, float distance, Direction direction)
{
	return vec + DirectionToVector(direction) * distance;
}

bool PointInsideCircle(const sf::Vector2f& center, float radius, const sf::Vector2f& point)
{
	auto vec = center - point;
	return std::sqrt(vec.x * vec.x + vec.y * vec.y) <= radius;
}

float RectangleDistanceAfterMovement(sf::FloatRect dynamicRect,
	const sf::FloatRect& staticRect,
	float distance, 
	Direction direction)
{
	if (!CanCollide(dynamicRect, staticRect, direction)) {
		return distance;
	}
	auto moved = MoveVectorInDirection({ dynamicRect.left, dynamicRect.top }, distance, direction);

	dynamicRect.left = moved.x;
	dynamicRect.top = moved.y;

	if ((direction == Direction::DOWN || direction == Direction::UP) &&
		dynamicRect.intersects(staticRect)) {
		return distance + RectangleAbsoluteYDistance(dynamicRect, staticRect);
	}
	if (dynamicRect.intersects(staticRect)) { // Direction::LEFT, Direction::RIGHT
		return distance + RectangleAbsoluteXDistance(dynamicRect, staticRect);
	}
	return distance;
}

void DrawRectangle(sf::RenderWindow& window, 
	const sf::FloatRect& rectangle,
	const sf::Color& color)
{
	sf::RectangleShape shape({ rectangle.width, rectangle.height });
	shape.setPosition({ rectangle.left, rectangle.top });
	shape.setFillColor(color);
	window.draw(shape);
}

sf::Color MixColors(const sf::Color& c1, const sf::Color& c2, float ratio)
{
	auto oneMinusRatio = 1.f - ratio;
	return {
		static_cast<uint8_t>(c1.r * ratio + c2.r * oneMinusRatio),
		static_cast<uint8_t>(c1.g * ratio + c2.g * oneMinusRatio),
		static_cast<uint8_t>(c1.b * ratio + c2.b * oneMinusRatio),
		static_cast<uint8_t>(c1.a * ratio + c2.a * oneMinusRatio),
	};
}

int Random(int from, int to)
{
	static std::random_device rd;
	static std::mt19937 eng(rd());
	return std::uniform_int_distribution<>(from, to)(eng);
}

}