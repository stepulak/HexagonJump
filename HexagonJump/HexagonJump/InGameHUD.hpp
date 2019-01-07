#pragma once

#include "Camera.hpp"
#include "GuiManager.hpp"
#include <SFML/Graphics.hpp>

namespace hexagon::gui {

class InGameHUD {
public:

	InGameHUD(const sf::Font& font);

	size_t GetTotalScore() const { return _totalScore; }

	void TriggerWin(float musicTime);
	void TriggerDeath(float musicTime);

	void Update(float deltaTime, size_t scoreAdd, float musicTime);
	void Draw(sf::RenderWindow& window, const Camera& camera) const;

private:

	static const sf::Color TEXT_COLOR;

	// No need for gui manager here
	const sf::Font& _font;
	std::optional<BlinkingLabel> _blinkingLabel;

	bool _ended = false;
	bool _died = false;
	size_t _totalScore = 0u;
};

}