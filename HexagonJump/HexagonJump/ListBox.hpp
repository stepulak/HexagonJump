#pragma once

#include "Button.hpp"
#include "Pool.hpp"

namespace hexagon::gui {

class ListBox : public GuiElement {
public:

	using Callback = std::function<void(const std::string&)>;

	ListBox(const sf::Vector2f& position,
		float fontSize,
		size_t numElementsScroll,
		const Callback& pressCallback);

	const std::string& GetActiveElement() const { return _pool[_activeElementIndex]; }
	void AddElement(const std::string& text) { _pool.Add(text); }

	bool MoveUp() override;
	bool MoveDown() override;
	bool Press() override;

	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr size_t DEFAULT_POOL_SIZE = 16u;
	static const sf::Color LISTBOX_COLOR;

	const sf::Vector2f _position;
	const float _fontSize;
	const size_t _numElementsScroll;
	size_t _activeElementIndex = 0u;
	Callback _callback;
	Pool<std::string> _pool{ DEFAULT_POOL_SIZE };
};

}