#include "WorldColorPalette.hpp"
#include "Utils.hpp"

#include <unordered_map>

namespace hexagon {

static std::unordered_map<ColorPalette, std::unordered_map<ColorEntity, sf::Color>> palettes = {
	{ 
		ColorPalette::BLUE, 
		{
			{ ColorEntity::BACKGROUND, sf::Color { 0, 102, 132, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 0, 190, 236, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 0, 56, 75, 128 } },
			{ ColorEntity::FLASH, sf::Color { 0, 255, 255, 80 } },
		}
	},
	{
		ColorPalette::RED,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 157, 19, 22, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 166, 54, 43, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 245, 141, 116, 128 } },
			{ ColorEntity::FLASH, sf::Color { 255, 0, 0, 80 } },
		}
	},
	{
		ColorPalette::GREEN,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 204, 255, 187, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 76, 121, 45, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 58, 95, 11, 128 } },
			{ ColorEntity::FLASH, sf::Color { 0, 255, 0, 80 } },
		}
	},
	{
		ColorPalette::YELLOW,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 174, 137, 36, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 230, 176, 18, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 211, 160, 46, 128 } },
			{ ColorEntity::FLASH, sf::Color { 255, 255, 0, 80 } },
		}
	},
	{
		ColorPalette::PINK,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 199, 55, 116, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 246, 168, 202, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 238, 98, 162, 128 } },
			{ ColorEntity::FLASH, sf::Color { 149, 19, 67, 80 } },
		}
	},
	{
		ColorPalette::VIOLET,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 141, 108, 171, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 202, 181, 213, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 89, 52, 130, 190 } },
			{ ColorEntity::FLASH, sf::Color { 123, 83, 157, 80 } },
		}
	},
	{
		ColorPalette::GRAY,
		{
			{ ColorEntity::BACKGROUND, sf::Color { 214, 216, 218, 255 } },
			{ ColorEntity::OBSTACLE, sf::Color { 115, 117, 119, 255 } },
			{ ColorEntity::STRIPE, sf::Color { 115, 117, 119, 128 } },
			{ ColorEntity::FLASH, sf::Color { 77, 79, 91, 80 } },
		}
	},
};

sf::Color GetColor(ColorPalette palette, ColorEntity entity)
{
	return palettes[palette][entity];
}

ColorPalette GetRandomColorPalette()
{
	return static_cast<ColorPalette>(Random(0, static_cast<std::underlying_type<ColorPalette>::type>(ColorPalette::NONE) - 1));
}

}