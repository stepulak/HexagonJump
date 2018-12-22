#pragma once

#include "Utils.hpp"
#include "Camera.hpp"

namespace hexagon {

class Player;

class Obstacle {
public:

	using Ptr = std::unique_ptr<Obstacle>;

	enum class Type {
		PLATFORM,
		SPIKE
	};

	Obstacle(Type type)
		: _type(type) {}

	virtual ~Obstacle() = default;

	Type GetType() const { return _type; }
	
	virtual bool InCollision(const Player& player) const = 0;
	virtual float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const = 0;
	virtual void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const = 0;

private:

	Type _type;
};

}