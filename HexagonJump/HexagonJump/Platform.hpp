#pragma once

#include "Obstacle.hpp"

namespace hexagon {

class Platform : public Obstacle {
public:

	Platform(float leftX, float topY, float width, float height);

	sf::Vector2f GetPosition() const { return { _area.left, _area.top }; }
	sf::Vector2f GetSize() const { return { _area.width, _area.height }; }

	void Move(float horizontalDistance) override { 
		_area.left += horizontalDistance; 
	}

	bool PassedCamera(const Camera& camera) const override {
		return _area.left + _area.width - camera.GetPosition() <= 0.f;
	}

	bool InCollision(const Player& player) const override {
		return CreateRectangleFromCircleBody(player.GetPosition(), player.GetRadius()).intersects(_area);
	}

	float SaveDistanceToTravel(const Player& player,
		float wantedDistance, 
		Direction direction) const override;

	void Draw(sf::RenderWindow& window, 
		const Camera& camera, 
		const sf::Color& color) const override;

private:

	sf::FloatRect _area;
};

}