#pragma once

#include "WorldColorPalette.hpp"

#include <optional>

namespace hexagon {

class ColorPaletteChanger {
public:

	ColorPaletteChanger(float nextColorChangeTime);

	sf::Color GetActiveColor(ColorEntity entity) const;
	void Update(float deltaTime, float skipTime);

private:

	ColorPalette _colorPalette;
	std::optional<ColorPalette> _nextColorPalette;
	const float _nextColorChangeTime;
	float _nextColorPaletteTimer = 0.f;
	float _nextColorPaletteRatio = 0.f;
};

}