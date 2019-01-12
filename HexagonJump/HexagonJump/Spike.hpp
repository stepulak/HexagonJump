#pragma once

#include "Obstacle.hpp"
#include "BeatUnit.hpp"

namespace hexagon {

class Spike : public Obstacle {
public:

	Spike(float x, 
		float y, 
		float width, 
		float maximumHeight,
		Direction direction,
		const BeatUnit& beatUnit);

	void Move(float horizontalDistance) override { _position.x += horizontalDistance; }

	bool PassedCamera(const Camera& camera) const override {
		return _position.x + std::max(_maxHeight, _width / 2.f) - camera.GetPosition() <= 0.f;
	}

	bool InCollision(const Player& player) const override;

	float SaveDistanceToTravel(const Player& player,
		float wantedDistance, 
		Direction direction) const override;

	void Draw(sf::RenderWindow& window, 
		const Camera& camera,
		const sf::Color& color) const override;

private:
	
	static const sf::Color BORDER_COLOR;
	
	sf::ConvexShape GetSpikeBody() const {
		return CountTriangle(_width, _maxHeight * _beatUnit.Height(), _direction);
	}

	bool CheckCollisionWithPlayer(const sf::Vector2f& playerPosition, float playerRadius) const;

	void DrawBorder(sf::RenderWindow& window,
		const sf::Vector2f& position,
		const sf::ConvexShape& body) const;

	sf::Vector2f _position;
	const float _width;
	const float _maxHeight;
	const Direction _direction;
	const BeatUnit& _beatUnit;
};

}