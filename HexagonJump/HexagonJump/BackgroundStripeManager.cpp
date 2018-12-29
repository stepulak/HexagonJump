#include "BackgroundStripeManager.hpp"

namespace hexagon {

void BackgroundStripeManager::SpawnStripe(float x, float y)
{
	_stripeIndex++;
	_stripes.emplace_back(x, y);
}

void BackgroundStripeManager::Update(const Camera& camera, float deltaTime)
{
	for (size_t i = 0u; i < _stripeIndex;) {
		auto& stripe = _stripes[i];
		stripe.Update(deltaTime);
		if (stripe.PassedCamera(camera)) {
			_stripeIndex--;
			if (i < _stripeIndex) {
				std::swap(stripe, _stripes[_stripeIndex]);
			}
		}
		else {
			i++;
		}
	}
}

void BackgroundStripeManager::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	for (size_t i = 0u; i < _stripeIndex; i++) {
		_stripes[i].Draw(window, camera, color);
	}
}

}
