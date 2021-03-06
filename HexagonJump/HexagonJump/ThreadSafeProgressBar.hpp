#pragma once

#include "GuiElement.hpp"

#include <functional>
#include <mutex>

namespace hexagon::gui {

class ThreadSafeProgressBar : public GuiElement {
public:

	ThreadSafeProgressBar(const sf::FloatRect& area, float fontSize);
	~ThreadSafeProgressBar() = default;

	bool IsInvoked() const { return _invoked; }

	void Invoke() override;
	void Close() override;

	float GetProgress() const;
	void UpdateProgress(float progress);

	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float PROGRESS_TEXT_VERTICAL_POSITION_RATIO = 0.4f;
	static constexpr float BAR_VERTICAL_POSITION_RATIO = 0.7f;
	static constexpr float BAR_WIDTH_RATIO = 0.8f;
	static constexpr float BAR_HEIGHT = 10.f;

	static const sf::Color BACKGROUND_COLOR;
	static const sf::Color BAR_COMPLETED_COLOR;
	static const sf::Color BAR_UNCOMPLETED_COLOR;
	static const sf::Color TEXT_COLOR;
	
	void DrawProgress(sf::RenderWindow& window, const sf::Font& font) const;
	void DrawBar(sf::RenderWindow& window) const;

	mutable std::mutex _mutex;

	sf::FloatRect _area;
	float _fontSize;
	float _progress = 0.f;
	bool _invoked = false;
};

}