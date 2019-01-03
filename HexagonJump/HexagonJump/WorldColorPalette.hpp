#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon {

enum class ColorPalette : int {
	BLUE,
	RED,
	GREEN,
	YELLOW,
	PINK,
	VIOLET,
	GRAY,
	NONE
};

enum class ColorEntity {
	BACKGROUND,
	OBSTACLE,
	STRIPE,
	FLASH
};

sf::Color GetColor(ColorPalette palette, ColorEntity entity);
ColorPalette GetRandomColorPalette();

}