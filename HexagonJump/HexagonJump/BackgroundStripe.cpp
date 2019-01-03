#include "BackgroundStripe.hpp"
#include "Utils.hpp"
#include <iostream>

namespace hexagon {

bool BackgroundStripe::PassedCamera(const Camera& camera) const
{
	return _position.y >= camera.GetVirtualHeight();
}

void BackgroundStripe::Update(float deltaTime)
{
	_position.y += GRAVITY * deltaTime;
}

void BackgroundStripe::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	auto transparentColor = sf::Color(color.r, color.g, color.b, 0);
	auto x = _position.x - camera.GetPosition();
	
	sf::Vertex vertices[] = {
		{ { x, _position.y }, transparentColor },
		{ { x + WIDTH, _position.y }, transparentColor },
		{ { x + WIDTH, _position.y + HEIGHT }, color },
		{ { x, _position.y + HEIGHT }, color },
	};

	window.draw(vertices, 4, sf::Quads);
}

}