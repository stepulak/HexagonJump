#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <filesystem>

namespace hexagon {

static constexpr float PI = 3.141592f;

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

static constexpr float RadiusToDegree(float radius) {
	return radius * 180.f / PI;
}

static constexpr float DegreeToRadius(float degree) {
	return degree * PI / 180.f;
}

static inline sf::FloatRect CreateRectangleFromCircle(const sf::Vector2f& position, float radius) {
	return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}

sf::ConvexShape CountTriangle(float base, float height, Direction heightDirection);
sf::Vector2f MoveVectorInDirection(const sf::Vector2f& vec, float distance, Direction direction);
bool PointInsideCircle(const sf::Vector2f& center, float radius, const sf::Vector2f& point);

float RectangleDistanceAfterMovement(sf::FloatRect dynamicRect,
	const sf::FloatRect& staticRect, 
	float distance,
	Direction direction);

void DrawRectangle(sf::RenderWindow& window, 
	const sf::FloatRect& rectangle,
	const sf::Color& color);

// Mix two colors togerther according to given ratio.
sf::Color MixColors(const sf::Color& c1, const sf::Color& c2, float ratio);

// Random number from range <from;to>
int Random(int from, int to);

static std::string FilenameWithoutExtension(const std::string& path) {
	return std::filesystem::path(path).stem().string();
}

static std::string FilenameWithExtension(const std::string& path) {
	return std::filesystem::path(path).filename().string();
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