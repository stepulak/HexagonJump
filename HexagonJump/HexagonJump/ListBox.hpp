#pragma once

#include "Button.hpp"
#include "Pool.hpp"

namespace hexagon::gui {

class ListBox : public GuiElement {
public:

	using ElementType = std::string;
	using Callback = std::function<void(const ElementType&)>;

	ListBox(const sf::Vector2f& position,
		float fontSize,
		size_t numElementsScroll,
		const Callback& pressCallback);

	const ElementType& GetActiveElement() const { return _pool[_activeElementIndex]; }
	void AddElement(const ElementType& text) { _pool.Add(text); }

	bool IsPressable() const override { return true; }
	bool IsMovable() const override { return true; }

	bool MoveUp() override;
	bool MoveDown() override;
	bool Press() override;

	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;
	void DrawMarker(sf::RenderWindow& window) const override;

private:

	static constexpr size_t DEFAULT_POOL_SIZE = 16u;

	size_t GetStartingIndexForDrawing() const;

	const sf::Vector2f _position;
	const float _fontSize;
	const size_t _numElementsScroll;
	const Callback _pressCallback;
	size_t _activeElementIndex = 0u;
	Pool<ElementType> _pool{ DEFAULT_POOL_SIZE };
};

}