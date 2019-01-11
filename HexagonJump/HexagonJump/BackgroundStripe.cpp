#include "BackgroundStripe.hpp"
#include "Utils.hpp"

namespace hexagon {

void BackgroundStripe::Draw(sf::RenderWindow& window,
	const Camera& camera,
	const sf::Color& color) const
{
	auto transparentColor = sf::Color(color.r, color.g, color.b, 0);
	auto x = _position.x - camera.GetPosition();
	
	// Create rectangle with color gradient
	sf::Vertex vertices[] = {
		{ { x, _position.y }, transparentColor },
		{ { x + WIDTH, _position.y }, transparentColor },
		{ { x + WIDTH, _position.y + HEIGHT }, color },
		{ { x, _position.y + HEIGHT }, color },
	};

	window.draw(vertices, 4, sf::Quads);
}

}