#pragma once

#include "WorldColorPalette.hpp"

#include <optional>

namespace hexagon {

class ColorPaletteChanger {
public:

	ColorPaletteChanger(float nextColorChangeTime);

	void Update(float deltaTime, float skipTime);
	sf::Color GetActiveColor(ColorEntity entity) const;

private:

	ColorPalette _colorPalette;
	std::optional<ColorPalette> _nextColorPalette;
	const float _nextColorChangeTime;
	float _nextColorPaletteTimer = 0.f;
	float _nextColorPaletteRatio = 0.f;
};

}