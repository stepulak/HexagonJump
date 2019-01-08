#include "EndScreenHUD.h"

namespace hexagon::gui {

const sf::Color EndScreenHUD::RESULT_MESSAGE_COLOR = { 255, 128, 128 };
const sf::Color EndScreenHUD::INFO_MESSAGE_COLOR = { 255, 255, 255 };

EndScreenHUD::EndScreenHUD(const Camera& camera, 
	EndType type,
	size_t totalTime,
	const std::string& restartButton)
	: _camera(camera)
	, _type(type)
	, _totalTime(totalTime)
	, _timeInfoMessage("Total time: " + std::to_string(totalTime) + " seconds", 
			INFO_MESSAGE_COLOR,
			camera.GetVirtualProportions() / 2.f + sf::Vector2f{ 0.f, TEXT_SIZE * 2.5f },
			TEXT_SIZE, 
			true)
	, _resultMessage(type == EndType::DEATH ? "You died!" : "You won!", 
			RESULT_MESSAGE_COLOR, 
			camera.GetVirtualProportions() / 2.f,
			TEXT_SIZE, 
			true)
	, _restartInfoMessage("For restart press " + restartButton, 
			INFO_MESSAGE_COLOR,
			camera.GetVirtualProportions() / 2.f + sf::Vector2f{ 0.f, TEXT_SIZE * 5.f },
			TEXT_SIZE,
			true)
{
}

void EndScreenHUD::Update(float deltaTime)
{
	_resultMessage.Update(deltaTime);
}

void EndScreenHUD::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	_resultMessage.Draw(window, font);
	_timeInfoMessage.Draw(window, font);
	_restartInfoMessage.Draw(window, font);
}

}