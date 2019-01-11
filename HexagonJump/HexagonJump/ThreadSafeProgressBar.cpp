#include "ThreadSafeProgressBar.hpp"
#include "Label.hpp"
#include "Utils.hpp"

namespace hexagon::gui {

const sf::Color ThreadSafeProgressBar::BACKGROUND_COLOR = { 50, 50, 50 };
const sf::Color ThreadSafeProgressBar::BAR_COMPLETED_COLOR = { 0, 0, 255 };
const sf::Color ThreadSafeProgressBar::BAR_UNCOMPLETED_COLOR = { 80, 80, 80 };
const sf::Color ThreadSafeProgressBar::TEXT_COLOR = { 255, 255, 255 };

ThreadSafeProgressBar::ThreadSafeProgressBar(const sf::FloatRect& area,
	float fontSize)
	: _area(area)
	, _fontSize(fontSize)
{
}

void ThreadSafeProgressBar::Invoke()
{
	std::lock_guard lock(_mutex);
	_invoked = true;
	_progress = 0.f;
}

void ThreadSafeProgressBar::Close()
{
	std::lock_guard lock(_mutex);
	_invoked = false;
}

float ThreadSafeProgressBar::GetProgress() const
{
	std::lock_guard lock(_mutex);
	return _progress;
}

void ThreadSafeProgressBar::UpdateProgress(float progress)
{
	std::lock_guard lock(_mutex);
	_progress = std::max(0.f, std::min(1.f, progress));
}

void ThreadSafeProgressBar::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	std::lock_guard lock(_mutex);
	if (!_invoked) {
		return;
	}
	DrawRectangle(window, _area, BACKGROUND_COLOR);
	DrawProgress(window, font);
	DrawBar(window);
}

void ThreadSafeProgressBar::DrawProgress(sf::RenderWindow& window, const sf::Font& font) const
{
	float x = _area.left + (_area.width - _area.width * BAR_WIDTH_RATIO) / 2.f;
	float y = _area.top + _area.height * PROGRESS_TEXT_VERTICAL_POSITION_RATIO;
	auto progress = static_cast<size_t>(_progress * 100.f);
	Label(std::to_string(progress) + " %", TEXT_COLOR, { x, y }, _fontSize).Draw(window, font);
}

void ThreadSafeProgressBar::DrawBar(sf::RenderWindow& window) const
{
	float barWidth = _area.width * BAR_WIDTH_RATIO;
	sf::FloatRect barArea = {
		_area.left + (_area.width - barWidth) / 2.f,
		_area.top + _area.height * BAR_VERTICAL_POSITION_RATIO,
		barWidth,
		BAR_HEIGHT
	};

	DrawRectangle(window, barArea, BAR_UNCOMPLETED_COLOR);
	barArea.width *= _progress;
	DrawRectangle(window, barArea, BAR_COMPLETED_COLOR);
}

}