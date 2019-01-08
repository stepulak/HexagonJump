#pragma once

#include "Camera.hpp"
#include "GuiManager.hpp"
#include <SFML/Graphics.hpp>

namespace hexagon::gui {

class GameStatsHUD : public GuiElement {
public:

	GameStatsHUD() = default;
	~GameStatsHUD() = default;

	void UpdateStats(size_t score) { _score = score; }
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float TEXT_WINDOW_OFFSET_X = 50.f;
	static constexpr float TEXT_WINDOW_OFFSET_Y = 25.f;
	static constexpr float TEXT_SIZE = 40.f;
	static const sf::Color TEXT_COLOR;

	size_t _score = 0.f;
};

}