#pragma once

#include "GuiElement.hpp"
#include "Camera.hpp"
#include "BlinkingLabel.hpp"

namespace hexagon::gui {

class EndScreenHUD : public GuiElement {
public:

	enum class EndType {
		WIN,
		DEATH
	};

	EndScreenHUD(const Camera& camera, EndType type, size_t totalTime, const std::string& restartButton);
	~EndScreenHUD() = default;

	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float TEXT_SIZE = 50.f;
	static const sf::Color RESULT_MESSAGE_COLOR;
	static const sf::Color INFO_MESSAGE_COLOR;

	const Camera& _camera;
	EndType _type;
	size_t _totalTime;
	Label _timeInfoMessage;
	BlinkingLabel _resultMessage;
	BlinkingLabel _restartInfoMessage;
};

}