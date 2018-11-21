#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon::utils {

static inline constexpr double pi() { return std::atan(1) * 4; }

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

sf::ConvexShape CountTriangleCoords(float width, float height, Direction topVertexDirection) {
	sf::ConvexShape shape(3);
	float dx = 0.f, dy = 0.f;

	switch (topVertexDirection)
	{
	case Direction::UP:
		dx = 0, dy = -1;
		break;
	case Direction::DOWN:
		dx = 0, dy = 1;
		break;
	case Direction::LEFT:
		dx = -1, dy = 0;
		break;
	case Direction::RIGHT:
		dx = 1, dy = 0;
		break;
	}

	shape.setPoint(0, sf::Vector2f(-dy * width / 2, -dx * width / 2));
	shape.setPoint(1, sf::Vector2f(dy * width / 2, dx * width / 2));
	shape.setPoint(2, sf::Vector2f(dx * height, dy * height));
	return shape;
}

}