#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <filesystem>

namespace hexagon {

static constexpr float PI = 3.141592f;

static constexpr float RadiusToDegree(float radius) {
	return radius * 180.f / PI;
}

static constexpr float DegreeToRadius(float degree) {
	return degree * PI / 180.f;
}

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

sf::ConvexShape CountTriangleCoords(float width, float height, Direction topVertexDirection);

// Collision functions
float RectangleAbsoluteXDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
float RectangleAbsoluteYDistance(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
float RectangleDistanceAfterMovement(sf::FloatRect dynamicRect, const sf::FloatRect& staticRect, float distance, Direction direction);
sf::Vector2f MoveVectorInDirection(sf::Vector2f vec, float distance, Direction direction);

static inline sf::FloatRect CreateRectangleFromCircleBody(const sf::Vector2f& position, float radius) {
	return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}

int Random(int from, int to);
sf::Color MixColors(const sf::Color& c1, const sf::Color& c2, float ratio);
void DrawRectangle(sf::RenderWindow& window, const sf::FloatRect& rectangle, const sf::Color& color);

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

static std::string FilenameWithoutExtension(const std::string& path) {
	return std::filesystem::path(path).stem().string();
}

}