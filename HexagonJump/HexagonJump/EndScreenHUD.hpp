#pragma once

#include "GuiElement.hpp"
#include "Camera.hpp"

namespace hexagon::gui {

class EndScreenHUD : public GuiElement {
public:

	EndScreenHUD(const Camera& camera, 
		size_t totalTime, 
		const std::string& resultMessage,
		const std::string& restartKey);

	~EndScreenHUD() = default;

	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float TEXT_SIZE = 50.f;
	static const sf::Color RESULT_MESSAGE_COLOR;
	static const sf::Color INFO_MESSAGE_COLOR;

	const Camera& _camera;
	const size_t _totalTime;
	GuiElement::Ptr _timeInfoMessage;
	GuiElement::Ptr _resultMessage;
	GuiElement::Ptr _restartInfoMessage;
};

}