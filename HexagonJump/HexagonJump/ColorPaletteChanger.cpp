#include "ColorPaletteChanger.hpp"
#include "Utils.hpp"

namespace hexagon {

ColorPaletteChanger::ColorPaletteChanger(float nextColorChangeTime)
	: _nextColorChangeTime(nextColorChangeTime)
	, _colorPalette(GetRandomColorPalette())
{
}

void ColorPaletteChanger::Update(float deltaTime, float skipTime)
{
	if (_nextColorPalette) {
		_nextColorPaletteRatio += deltaTime;

		if (_nextColorPaletteRatio >= 1.f) {
			_nextColorPaletteRatio = 0.f;
			_colorPalette = _nextColorPalette.value();
			_nextColorPalette.reset();
		}
		return;
	}
	_nextColorPaletteTimer += deltaTime;
	_nextColorPaletteTimer += skipTime * deltaTime;

	if (_nextColorPaletteTimer >= _nextColorChangeTime) {
		_nextColorPaletteTimer = 0.f;
		_nextColorPalette = GetRandomColorPalette();
	}
}

sf::Color ColorPaletteChanger::GetActiveColor(ColorEntity entity) const
{
	auto activeColor = GetColor(_colorPalette, entity);

	if (_nextColorPalette) {
		auto nextColor = GetColor(_nextColorPalette.value(), entity);
		return MixColors(nextColor, activeColor, _nextColorPaletteRatio);
	}
	return activeColor;
}

}