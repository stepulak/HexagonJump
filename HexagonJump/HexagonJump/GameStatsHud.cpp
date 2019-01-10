#include "GameStatsHUD.hpp"

namespace hexagon::gui {

const sf::Color GameStatsHUD::TEXT_COLOR = { 255, 255, 255 };

void GameStatsHUD::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	auto text = "Current time: " + std::to_string(_time);
	Label(text, TEXT_COLOR, { TEXT_WINDOW_OFFSET_X, TEXT_WINDOW_OFFSET_Y }, TEXT_SIZE)
		.Draw(window, font);
}

}