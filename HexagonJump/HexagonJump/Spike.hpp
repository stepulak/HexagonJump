#pragma once

#include "Obstacle.hpp"
#include "BeatUnit.hpp"

namespace hexagon {

class Spike : public Obstacle {
public:

	Spike(float x, float y, float width, float maximumHeight, Direction direction, const BeatUnit& beatUnit);

	void Move(float horizontalDistance) override;
	bool InCollision(const Player& player) const override;
	float SaveDistanceToTravel(const Player& player, float wantedDistance, Direction direction) const override;
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const override;

private:

	bool CheckCollisionWithPlayer(const sf::Vector2f& position, float radius) const;
	sf::ConvexShape CountSpikeBody() const;

	sf::Vector2f _position;
	float _width;
	float _maxHeight;
	Direction _direction;
	const BeatUnit& _beatUnit;
};

}