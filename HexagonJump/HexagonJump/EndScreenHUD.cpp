#include "EndScreenHUD.h"
#include "BlinkingLabel.hpp"

namespace hexagon::gui {

const sf::Color EndScreenHUD::RESULT_MESSAGE_COLOR = { 255, 128, 128 };
const sf::Color EndScreenHUD::INFO_MESSAGE_COLOR = { 255, 255, 255 };

EndScreenHUD::EndScreenHUD(const Camera& camera,
	size_t totalTime, 
	const std::string& resultMessage,
	const std::string& restartKey)
	: _camera(camera)
	, _totalTime(totalTime)
{
	_resultMessage = std::make_unique<BlinkingLabel>(resultMessage,
		RESULT_MESSAGE_COLOR,
		camera.GetVirtualProportions() / 2.f,
		TEXT_SIZE,
		true);

	_timeInfoMessage = std::make_unique<Label>("Total time: " + std::to_string(totalTime) + " seconds",
		INFO_MESSAGE_COLOR,
		camera.GetVirtualProportions() / 2.f + sf::Vector2f{ 0.f, TEXT_SIZE * 1.5f },
		TEXT_SIZE,
		true);

	_restartInfoMessage = std::make_unique<Label>("For restart press " + restartKey,
		INFO_MESSAGE_COLOR,
		camera.GetVirtualProportions() / 2.f + sf::Vector2f{ 0.f, TEXT_SIZE * 3.f },
		TEXT_SIZE,
		true);
}

void EndScreenHUD::Update(float deltaTime)
{
	_resultMessage->Update(deltaTime);
}

void EndScreenHUD::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	_resultMessage->Draw(window, font);
	_timeInfoMessage->Draw(window, font);
	_restartInfoMessage->Draw(window, font);
}

}