#pragma once

#include <SFML/Graphics.hpp>
#include <random>

namespace hexagon {

static constexpr float PI = 3.141592f;

static constexpr float DegreeToRadius(float radius) {
	return radius * 180.f / PI;
}

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

static sf::ConvexShape CountTriangleCoords(float width, float height, Direction topVertexDirection) {
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

static int Random(int from, int to) {
	static std::random_device rd;
	static std::mt19937 eng(rd());
	return std::uniform_int_distribution<>(from, to)(eng);
}

template<typename TEnum>
static TEnum operator|(TEnum m1, TEnum m2)
{
	using UType = typename std::underlying_type<TEnum>::type;
	return static_cast<TEnum>(static_cast<UType>(m1) | static_cast<UType>(m2));
}

template<typename TEnum>
static bool operator&(TEnum m1, TEnum m2)
{
	using UType = typename std::underlying_type<TEnum>::type;
	return static_cast<bool>(static_cast<UType>(m1) & static_cast<UType>(m2));
}

}