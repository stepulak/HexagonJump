#pragma once

#include "Label.hpp"

namespace hexagon::gui {

class BlinkingLabel : public Label {
public:

	BlinkingLabel(const std::string& text, 
		const sf::Color& color, 
		const sf::Vector2f& position,
		float fontSize,
		bool centered = false);

	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float LABEL_VISIBLE_TIME = 1.f;
	static constexpr float LABEL_HIDDEN_TIME = 1.f;

	float _timer = 0.f;
};

}