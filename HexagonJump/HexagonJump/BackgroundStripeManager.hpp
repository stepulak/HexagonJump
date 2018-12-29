#pragma once

#include "BackgroundStripe.hpp"

#include <vector>

namespace hexagon {

class BackgroundStripeManager {
public:

	BackgroundStripeManager() { _stripes.reserve(STRIPES_DEFAULT_POOL_SIZE); }

	void SpawnStripe(float x, float y);
	void Update(const Camera& camera, float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	static constexpr size_t STRIPES_DEFAULT_POOL_SIZE = 16u;

	size_t _stripeIndex = 0u;
	std::vector<BackgroundStripe> _stripes;
};

}